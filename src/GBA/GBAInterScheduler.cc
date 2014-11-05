//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "GBAInterScheduler.h"

namespace epon {
Define_Module(GBAInterScheduler);

GBAInterScheduler::GBAInterScheduler() {

}

GBAInterScheduler::~GBAInterScheduler() {

}

void GBAInterScheduler::initialize() {
    Scheduler::initialize();

    //parameters
    currentONU = -1;
    reported = 0;
    tMPCP = getTransmissionTime(intpar("MPCPSize"));
    tGuard = timepar("tGuard");
    RTT = 2 * timepar("tProp");
    tAvgFrame = (intpar("maxFrameSize") + intpar("minFrameSize")) / (2 * intpar("cPON"));
    tCycleStart = 0;
    required.assign(intpar("numONUs"),0);
    tSendGate.assign(intpar("numONUs"),0);
    tStart.assign(intpar("numONUs"),0);
    tWindow.assign(intpar("numONUs"),0);
    tRequired.assign(intpar("numONUs"),0);

    //GBA parameters
    tGBA = timepar("tGBA");
    D.assign(intpar("numONUs"),timepar("D"));
    TS1.assign(intpar("numONUs"),0);
    TS2.assign(intpar("numONUs"),RTT + tGBA + (intpar("numONUs") - 1) * (tGuard + tMPCP));

    //components
    queue = check_and_cast<MultiQueue*>(getParentModule()->getSubmodule("multiqueue"));

    //statistics
    sdCycleTime = new cStdDev("sdCycleTime");
    r = check_and_cast<Recorder*>(getParentModule()->getParentModule()->getSubmodule("recorder"));
    std::stringstream sstag;
    sstag << "D=" << dblpar("D");
    std::string stag = sstag.str();
    r->setTag(stag);

    //start simulation
    bandwidthAlloc(required);

    //random bandwidth test
    //test();
}

void GBAInterScheduler::finish() {
    Scheduler::finish();
    recordScalar("cycle time avg",sdCycleTime->getMean());
    recordScalar("cycle time stddev",sdCycleTime->getStddev());

    std::vector<double> v = std::vector<double>();
    v.push_back(sdCycleTime->getMean());
    v.push_back(sdCycleTime->getStddev());
    r->addDataRow("AvgCycleTime",v);

    std::cerr << "OLT " << endl;
    std::cerr << "\tavg cycle time=" << sdCycleTime->getMean() << endl;

    delete queue;
    delete sdCycleTime;
}

void GBAInterScheduler::handleSelfMessage(cMessage* msg) {
    //std::cerr << "GBAInterScheduler::handleSelfMessage t=" << simTime() << endl;

    int LLID = int(msg->par("LLID"));
    if(msg->getKind() == intpar("SendMPCP")) sendMPCP(LLID);
    else if(msg->getKind() == intpar("StartSend"))
    {
        //Downstream transmission window start
        currentONU = LLID;
        scheduleDownstream();
    }
    delete msg;
}

void GBAInterScheduler::sendMPCP(int LLID) {
    //std::cerr << "GBAInterScheduler::sendGate t=" << simTime() << endl;

    GBAGate* gate = new GBAGate("gate");
    gate->setDST(LLID);
    gate->setBitLength(intpar("MPCPSize"));
    gate->setStartTime(tStart[LLID]);
    gate->setLength(tWindow[LLID]);
    gate->setONUSleepTime(TS2[LLID]);
    send(gate,"txSplitter");
}

void GBAInterScheduler::scheduleDownstream() {
    //std::cerr << "GBAInterScheduler::scheduleDownstream t=" << simTime() << endl;

    //upstream centric downstream scheduling
    if(currentONU >= 0 && currentONU < intpar("numONUs"))
    {
        sendDataUntil(tStart[currentONU] + tWindow[currentONU] - RTT / 2);
    }
    else throw cRuntimeError("GBAInterScheduler::scheduleDownstream currentONU=%d",currentONU);
}

void GBAInterScheduler::handleMessageFromSplitter(cMessage* msg) {
    //std::cerr << "GBAInterScheduler::handleMessageFromSplitter t=" << simTime() << endl;

    Frame* frame = check_and_cast<Frame*>(msg);
    if(frame->getKind() == intpar("MPCP"))
    {
        handleMPCP(msg);
        delete msg;
    }
    else if(msg->getKind() > intpar("MPCP") && msg->getKind() < intpar("FramePriorities")) send(frame,"txHost");
    else delete frame;
}

void GBAInterScheduler::handleMPCP(cMessage* msg) {
    //std::cerr << "GBAInterScheduler::handleReport t=" << simTime() << endl;

    GBAReport* report = check_and_cast<GBAReport*>(msg);
    required[report->getSRC()] = report->getRequiredBandwidth();
    ++reported;

    //offline dynamic bandwidth scheduling
    if(reported == intpar("numONUs"))
    {
        bandwidthAlloc(required);
        reported = 0;
    }
}

void GBAInterScheduler::handleMessageFromHost(cMessage* msg) {
    //std::cerr << "GBAInterScheduler::handleMessageFromHost t=" << simTime() << endl;

    Frame* frame = check_and_cast<Frame*>(msg);
    queue->insert(frame);
}

void GBAInterScheduler::bandwidthAlloc(std::vector<int> upRequiredBitlength) {
    //std::cerr << "GBAInterScheduler::bandwidthAlloc t=" << simTime() << endl;

    //calculate cycle time
    sdCycleTime->collect(simTime() - tCycleStart);
    tCycleStart = simTime();

    //calculate required bandwidth according upstream and downstream buffer size
    for(int i = 0;i < intpar("numONUs");++i)
    {
        tRequired[i] = std::max(getTransmissionTime(upRequiredBitlength[i]),getTransmissionTime(queue->getBitLength(i)));

        //inter scheduling service
        switch(intpar("interScheduling"))
        {
        case GATED:
            tWindow[i] = tRequired[i];
            break;
        }
    }

    //run sort and shift
    sortAndShift();

    //schedule to send gate and send downstream
    for(int i = 0;i < intpar("numONUs");++i)
    {
        cMessage* msg = new cMessage("SendMPCP",intpar("SendMPCP"));
        cMsgPar* par = new cMsgPar("LLID");
        par->setLongValue(i);
        msg->addPar(par);
        scheduleAt(tSendGate[i],msg);

        msg = new cMessage("StartSend",intpar("StartSend"));
        par = new cMsgPar("LLID");
        par->setLongValue(i);
        msg->addPar(par);
        scheduleAt(tStart[i] - RTT / 2,msg);
    }

    //
    currentONU = -1;
}

void GBAInterScheduler::sortAndShift() {
    //std::cerr << "GBAInterScheduler::sortAndShift t=" << simTime() << endl;

    //update OLT sleep TS1 and start time for each ONU
    simtime_t tWindowSum = 0;
    std::vector<int> vectorLLID;
    std::vector<simtime_t> vectorStartTime;
    for(int i = 0;i < intpar("numONUs");++i)
    {
        tWindowSum += tWindow[i];
        tSendGate[i] = simTime() + tGBA + i * (tMPCP + tGuard);
        TS1[i] = (2.0/3.0) * (D[i] - RTT / 2 - tAvgFrame) - tWindow[i] - tMPCP - TS2[i];
        vectorLLID.push_back(i);
        vectorStartTime.push_back(simTime() + tGBA + RTT / 2 + (i + 1) * tMPCP + i * tGuard + TS1[i]);
    }

    //sort OLT sleep TS1
    sort(&vectorLLID,&vectorStartTime);

    //adjust transmission start time from the largest TS1 ONU to the smallest TS1 ONU
    tStart[vectorLLID[intpar("numONUs") - 1]] = vectorStartTime[intpar("numONUs") - 1];
    for(int i = intpar("numONUs") - 2;i >= 0;--i)
    {
        int LLID = vectorLLID[i];
        simtime_t h = vectorStartTime[i] - (tStart[vectorLLID[i + 1]] - tGuard - tMPCP - tWindow[LLID]);
        if(h < 0) h = 0;
        TS1[LLID] -= h;
        tStart[LLID] = vectorStartTime[i] - h;
        if(tStart[LLID] <= tSendGate[LLID] + RTT / 2 + tMPCP)
        {
            //SAS overflow
            tWindow[LLID] = 0;
            tStart[LLID] = tSendGate[LLID] + RTT / 2 + tMPCP;
        }
    }

    //update ONU TS2 for each sleep
    for(int i = 0;i < intpar("numONUs");++i)
    {
        int LLID = vectorLLID[i];
        TS2[LLID] = RTT + tGBA;
        for(int j = i + 1;j < intpar("numONUs");++j) TS2[LLID] += tGuard + tWindow[vectorLLID[j]] + tMPCP;
        TS2[i] += i * (tMPCP + tGuard);
    }

/*
    std::cerr << "SAS" << endl;
    for(int i = 0;i < vectorLLID.size();++i)
    {
        std::cerr << i << ">ONU" << vectorLLID[i] << endl;
        std::cerr << "\ts1=" << TS1[vectorLLID[i]] << endl;
        std::cerr << "\tst=" << vectorStartTime[i] << endl;
        std::cerr << "\tw=" << tWindow[vectorLLID[i]] << endl;
        std::cerr << "\tts2=" << TS2[vectorLLID[i]] << endl;
    }
*/

    //for test
    sorted = vectorLLID;
}

void GBAInterScheduler::sort(std::vector<int>* index,std::vector<simtime_t>* value) {
    //std::cerr << "GBAInterScheduler::sort t=" << simTime() << endl;

    //insertion sort with index
    int length = (*index).size();

    //std::cerr << "before sort" << endl;
    //for(int i = 0;i < length;++i) std::cerr << i << ">\t" << index[i] << ">\t" << value[i] << endl;

    for(int i = 1;i < length;++i)
    {
        int tempIndex = (*index)[i];
        simtime_t temp = (*value)[i];
        for(int j = i - 1;j >= 0;--j)
        {
            if((*value)[j] > temp)
            {
                (*index)[j + 1] = (*index)[j];
                (*value)[j + 1] = (*value)[j];
                if(j == 0)
                {
                    (*index)[j] = tempIndex;
                    (*value)[j] = temp;
                }
            }
            else
            {
                (*index)[j + 1] = tempIndex;
                (*value)[j + 1] = temp;
                j = 0;
            }
        }
    }

    //std::cerr << "after sort" << endl;
    //for(int i = 0;i < length;++i) std::cerr << i << ">\t" << (*index)[i] << ">\t" << (*value)[i] << endl;
}

bool GBAInterScheduler::queueEmpty() {
    //std::cerr << "GBAInterScheduler::queueEmpty t=" << simTime() << endl;

    if(currentONU >= 0 && currentONU < intpar("numONUs"))
    {
        return queue->isEmpty(currentONU);
    }
    else throw cRuntimeError("GBAInterScheduler::queueEmpty currentONU=%d",currentONU);
}

cPacket* GBAInterScheduler::getNextDataFrame() {
    //std::cerr << "GBAInterScheduler::getNextDataFrame t=" << simTime() << endl;

    if(currentONU >= 0 && currentONU < intpar("numONUs"))
    {
        return queue->front(currentONU);
    }
    else throw cRuntimeError("GBAInterScheduler::getNextDataFrame currentONU=%d",currentONU);
}

void GBAInterScheduler::test() {
    //std::cerr << "GBAInterScheduler::getNextDataFrame t=" << simTime() << endl;

    //test and verify
    simtime_t tRxAvailable = 0,tTxAvailable = 0;
    int windowSum = 0;

    //test bandwidth for each ONU
    for(int i = 0;i < intpar("numONUs");++i)
    {
        required[i] = (int)(uniform(0,1) * 1000000);
        windowSum += required[i];
    }
    std::cerr << "total test bandwidth=" << this->getTransmissionTime(windowSum) << endl;
    for(int i = 0;i < intpar("numONUs");++i)
    {
        for(int i = 0;i < intpar("numONUs");++i)
        {
            TS2[i] = RTT + tGBA;
            for(int j = 0;j < intpar("numONUs");++j)
                if(j > i) TS2[i] += tGuard + tWindow[i] + tMPCP;
                else if(j < i) TS2[i] += tMPCP + tGuard;
        }
    }

    //test bandwidth allocation
    bandwidthAlloc(required);

    for(int i = 0;i < intpar("numONUs");++i)
    {
        int LLID = sorted[i];
        std::cerr << "ONU" << LLID << endl;
        std::cerr << "\tStart=" << tStart[LLID] << endl;
        std::cerr << "\tSendGate=" << tSendGate[LLID] << endl;

        std::cerr << "\t@TS1=" << (tStart[LLID] - tSendGate[LLID] - tMPCP - RTT / 2) << endl;
        std::cerr << "\tTS1=" << TS1[LLID] << endl;
        std::cerr << "\tWindow=" << tWindow[LLID] << endl;
        std::cerr << "\tTS2=" << TS2[LLID] << endl;
        std::cerr << "\tct=" << (tMPCP + TS1[LLID] + tWindow[LLID] + tMPCP + TS2[LLID]) << endl;
        if(tRxAvailable > tStart[LLID] + RTT / 2) std::cerr << "\tONU " << LLID << " collide with ONU" << sorted[i - 1] << " overlap=" << (tStart[LLID] + RTT / 2 - tRxAvailable) << endl;
        else std::cerr << "\tdist=" << (tStart[LLID] + RTT / 2 - tRxAvailable) << endl;
        tRxAvailable = tStart[LLID] + tWindow[LLID] + tMPCP + RTT / 2;
    }
    tTxAvailable = tRxAvailable + tGBA + RTT / 2;
    for(int i = 0;i < intpar("numONUs");++i)
    {
        std::cerr << " TS2[" << i << "]=" << TS2[i] << endl;
        std::cerr << "@TS2[" << i << "]=" << (tTxAvailable - (tStart[i] + tWindow[i] + tMPCP)) << endl;
        tTxAvailable += tMPCP + tGuard;
    }

}

} /* namespace epon */
