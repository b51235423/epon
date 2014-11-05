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

#include "testInterScheduler.h"

namespace epon {
Define_Module(testInterScheduler);

testInterScheduler::testInterScheduler() {

}

testInterScheduler::~testInterScheduler() {
    delete sdCycle;
    for(int i = 0;i < intpar("numONUs");++i)
    {
        delete sdEFDelay[i];
        delete sdAFDelay[i];
        delete sdBEDelay[i];
    }

    delete sCycle;
    delete sAdjust;
    delete sFwdCycle;
}

void testInterScheduler::initialize() {
    Scheduler::initialize();

    //parameters
    currentONU = 0;
    tAvailable = simTime();
    tOLTCycleStart = simTime();
    getMPCP.assign(intpar("numONUs"),false);
    bRequired.assign(intpar("numONUs"),0);
    state.assign(intpar("numONUs"),CYCLICSLEEP);
    times.assign(intpar("numONUs"),0);
    tRequired.assign(intpar("numONUs"),0);
    tStart.assign(intpar("numONUs"),simTime());
    tWindow.assign(intpar("numONUs"),0);
    tWakeup.assign(intpar("numONUs"),0);
    for(int i = 0;i < intpar("numONUs");++i)
    {
        tEFDC.push_back(getParentModule()->getParentModule()->getSubmodule("onu",i)->par("tEFDC").doubleValue());
        tAFDC.push_back(getParentModule()->getParentModule()->getSubmodule("onu",i)->par("tAFDC").doubleValue());
        tBEDC.push_back(getParentModule()->getParentModule()->getSubmodule("onu",i)->par("tBEDC").doubleValue());
        tOverhead.push_back(getParentModule()->getParentModule()->getSubmodule("onu",i)->par("tOverhead").doubleValue());
        std::vector<simtime_t> v0;
        v0.assign(intpar("FramePriorities"),0);
        tNextFwd.push_back(v0);
        std::vector<simtime_t> v1;
        v1.push_back(0);
        v1.push_back(tEFDC[i] * 2);
        v1.push_back(tAFDC[i] * 2);
        v1.push_back(tBEDC[i] * 2);
        tFwdCycle.push_back(v1);
        std::vector<double> v2;
        v2.assign(intpar("FramePriorities"),1);
        rAdjust.push_back(v2);
    }
    sdCycle = new cStdDev("sdCycle");
    for(int i = 0;i < intpar("numONUs");++i)
    {
        sdEFDelay.push_back(new cStdDev("sdEFDelay"));
        sdAFDelay.push_back(new cStdDev("sdAFDelay"));
        sdBEDelay.push_back(new cStdDev("sdBEDelay"));
    }

    //components
    queue = check_and_cast<MultiTwoStageQueue*>(getParentModule()->getSubmodule("multiqueue"));

    //statistics
    std::vector<std::string> t;
    t.push_back("Average");
    sCycle = new cStat("Cycle",t);
    t.clear();
    t.push_back("Average");
    t.push_back("EF");
    t.push_back("AF");
    t.push_back("BE");
    sAdjust = new cStat("AdjustDown",t);
    sFwdCycle = new cStat("FwdCycleDown",t);

    //initialize
    bandwidthAlloc(bRequired);
    for(int i = intpar("EF");i < intpar("FramePriorities");++i) for(int j = 0;j < intpar("numONUs");++j) forward(j,i);

    //set tag
    std::stringstream sstag;
    sstag << "EF" << dblpar("tEFDC") << "_AF" << dblpar("tAFDC") << "_BE" << dblpar("tBEDC");
    std::string stag = sstag.str();
    rec()->setTag(stag);

    //debug
}

void testInterScheduler::finish() {
    Scheduler::finish();

    //debug
    if(log.str().length() > 0) outputLog();
    std::cerr << "[OLT::finish]" << endl;
    std::cerr << "\tpolling cycle=" << sdCycle->getMean() << endl;
    for(int i = 0;i < intpar("numONUs");i++)
    {
        std::cerr << "\ttimes[" << i << "]=" << times[i] << "\tstate=" << state[i] << endl;
        std::cerr << "\tqueue[" << i << "] size=" << queue->getBitLength(i) << " s1=" << (queue->getBitLength(i) - queue->getS2Size(i)) << " s2=" << queue->getS2Size(i) << endl;
        std::cerr << "\tPR EF=" << rAdjust[i].at(intpar("EF")) << " AF=" << rAdjust[i].at(intpar("AF")) << " BE=" << rAdjust[i].at(intpar("BE")) << endl;
    }
}

void testInterScheduler::handleSelfMessage(cMessage* msg) {
    if(msg->getKind() == intpar("CheckMPCP"))
    {
        //When an ONU is sleeping, it can't respond to the OLT's sleep gate, and without the ONU
        //sending a sleep report to the OLT, the OLT can't schedule next sleep gate to the ONU.
        //The OLT should schedule a event at the time after the time OLT will get ONU's sleep
        //report to check if the OLT really get report from ONU or not. If the OLT don't get a
        //sleep report from the ONU, the OLT should do bandwidth allocation invoke next send gate
        //event for the sleeping ONU to send next sleep gate.
        if(int(msg->par("LLID")) == intpar("debug")) log << "[OLT::checkMPCP] t=" << simTime() << " LLID=" << int(msg->par("LLID")) << " state=" << state[int(msg->par("LLID"))] << " getMPCP=" << getMPCP[int(msg->par("LLID"))] << endl;
        if(!getMPCP[int(msg->par("LLID"))])
        {
            bRequired[int(msg->par("LLID"))] = queue->getS2Size(int(msg->par("LLID")));
            if(int(msg->par("LLID")) == intpar("numONUs") - 1)
            {
                if(int(msg->par("LLID")) == intpar("debug"))
                {
                    log << "[OLT::checkMPCP bandwidthAlloc] t=" << simTime() << endl;
                    if(boolpar("err")) std::cerr << "[OLT::bandwidthAlloc] t=" << simTime() << endl;
                }
                bandwidthAlloc(bRequired);
            }
        }
    }
    else if(msg->getKind() == intpar("SendMPCP"))
    {
        //Downstream transmission window start
        sendMPCP(int(msg->par("LLID")));
        scheduled.pop_back();
    }
    else if(msg->getKind() == intpar("StartSend"))
    {
        currentONU = int(msg->par("LLID"));
        startTransmission(int(msg->par("LLID")));
    }
    else if(msg->getKind() == intpar("Forward"))
    {
        forward(int(msg->par("LLID")),int(msg->par("priority")));
    }
    delete msg;
}

void testInterScheduler::sendMPCP(int LLID) {
    //update state according to transmission window and remain time
    switch(state[LLID])
    {
    case CYCLICSLEEP:
        //if(tWindow[LLID] == 0 && getRemainTime(LLID) >= getFwdDelay(LLID)) state[LLID] = DEEPSLEEP;
        if(tWindow[LLID] == 0 && getRemainTime(LLID) >= dblpar("remainThreshold") * sdCycle->getMean()) state[LLID] = DEEPSLEEP;
        //if(tWindow[LLID] == 0) state[LLID] = DEEPSLEEP;
        break;
    case DEEPSLEEP:
        if(getMPCP[LLID] && tWindow[LLID] > 0) state[LLID] = CYCLICSLEEP;
        break;
    }

    CSLAGate* gate = new CSLAGate("gate");
    gate->setSRC(getLLID());
    gate->setDST(LLID);
    gate->setBitLength(intpar("MPCPSize"));
    gate->setGrant1StartTime(tStart[LLID]);
    gate->setGrant1Length(tWindow[LLID]);
    gate->setGrant2StartTime(tWakeup[LLID]);
    gate->setGrant2Length(sdCycle->getMean());

    //set the gate message to ONU according to system state and ONU transmission window
    std::string s;
    if(state[LLID] == CYCLICSLEEP || (state[LLID] == DEEPSLEEP && tRequired[LLID] > 0))
    {
        gate->setGrant1TransceiverMode(CS);
        if(state[LLID] == CYCLICSLEEP) s = "Gate";
        else s = "CSGate";
    }
    else
    {
        gate->setGrant1TransceiverMode(DS);
        if(timepar("tSleep") != 0) gate->setGrant1Length(timepar("tSleep"));
        else gate->setGrant1Length(getRemainTime(LLID));
        s = "DSGate";
    }

    if(LLID == intpar("debug"))
    {
        bool b = getRemainTime(LLID) >= getFwdDelay(LLID);
        log << "[OLT::send" << s << "] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " ts=" << tStart[LLID] << " tw=" << tWindow[LLID] << " sl=" << (gate->getGrant2StartTime() - (tStart[LLID] + tWindow[LLID])) << " q=" << queue->getBitLength(LLID) << " down=" << queue->getS2Size(LLID) << " r=" << b << endl;
        if(boolpar("err")) std::cerr << "[OLT::send" << s << "] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " ts=" << tStart[LLID] << " tw=" << tWindow[LLID] << " sl=" << (gate->getGrant2StartTime() - (tStart[LLID] + tWindow[LLID])) << " q=" << queue->getBitLength(LLID) << " down=" << queue->getS2Size(LLID) << " r=" << b << endl;
    }

    send(gate,"txSplitter");

    //check report if ONU is sleep
    if(state[LLID] == DEEPSLEEP)
    {
        cMessage* msg = new cMessage("CheckMPCP",intpar("CheckMPCP"));
        cMsgPar* par = new cMsgPar("LLID");
        par->setLongValue(LLID);
        msg->addPar(par);
        msg->setSchedulingPriority(1);
        scheduleAt(tStart[LLID] + tWindow[LLID] + tMPCP + RTT / 2,msg);
        getMPCP[LLID] = false;

        //debug
        if(tWindow[LLID] > 0) throw cRuntimeError("transmission window is not 0 while ONU%d is in DEEPSLEEP state",getLLID());
    }
}

void testInterScheduler::startTransmission(int LLID) {
    if(simTime() >= tStart[LLID] - RTT / 2 && state[LLID] == CYCLICSLEEP)
    {
        //check bandwidth
        while(queue->s1front(LLID,intpar("EF")) != NULL && (tWindow[LLID] - getTransmissionTime(queue->getS2Size(LLID))) >= getTransmissionTime(queue->s1front(LLID,intpar("EF"))->getBitLength())) queue->stepForward(LLID,intpar("EF"));
        while(queue->s1front(LLID,intpar("AF")) != NULL && (tWindow[LLID] - getTransmissionTime(queue->getS2Size(LLID))) >= getTransmissionTime(queue->s1front(LLID,intpar("AF"))->getBitLength())) queue->stepForward(LLID,intpar("AF"));
        while(queue->s1front(LLID,intpar("BE")) != NULL && (tWindow[LLID] - getTransmissionTime(queue->getS2Size(LLID))) >= getTransmissionTime(queue->s1front(LLID,intpar("BE"))->getBitLength())) queue->stepForward(LLID,intpar("BE"));

        if(LLID == intpar("debug"))
        {
            log << "[OLT::scheduleDown] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " start=" << (tStart[LLID] - RTT / 2) << " end=" << (tStart[LLID] + tWindow[LLID] + tMPCP - RTT / 2) << endl;
            if(boolpar("err")) std::cerr << "[OLT::scheduleDown] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " start=" << (tStart[LLID] - RTT / 2) << " end=" << (tStart[LLID] + tWindow[LLID] + tMPCP - RTT / 2) << endl;
        }
        sendDataUntil(tStart[LLID] + tWindow[LLID] + tMPCP - RTT / 2);
    }
}

void testInterScheduler::forward(int LLID,int priority) {
    //move frame of specific priority to stage-two queue and schedule next forward event
    queue->forward(LLID,priority);

    cMessage* msg = new cMessage("Forward",intpar("Forward"));
    cMsgPar* par = new cMsgPar("priority");
    par->setLongValue(priority);
    msg->addPar(par);
    par = new cMsgPar("LLID");
    par->setLongValue(LLID);
    msg->addPar(par);
    simtime_t t = getFwdCycle(LLID,priority);
    tNextFwd[LLID].at(priority) += t;
    if(tNextFwd[LLID].at(priority) < simTime()) throw cRuntimeError("OLT schedule forward message to the past, current=%f, schedule=%f, t=%f, priority=%d, pr=%f, fwdd=%f, cycle=%f",simTime().dbl(),tNextFwd[LLID].at(priority).dbl(),t.dbl(),priority,rAdjust[LLID].at(priority),getFwdDelay(LLID).dbl(),sdCycle->getMean());
    scheduleAt(tNextFwd[LLID].at(priority),msg);

    sAdjust->collect(0,rAdjust[LLID].at(priority));
    sAdjust->collect(priority,rAdjust[LLID].at(priority));
    sFwdCycle->collect(0,t);
    sFwdCycle->collect(priority,t);
}

simtime_t testInterScheduler::getFwdCycle(int LLID,int priority) {
    simtime_t t = 0;
    if(priority == intpar("EF"))
    {
        t = 2 * (tEFDC[LLID]/* - getFwdDelay(LLID)*/);
        t *= getR(LLID,priority) * (getOfflineMaxWindow() / (getOfflineMaxWindow() + getDownONULambda(intpar("EF")) * sdCycle->getMean() * getDownFirstMoment(intpar("EF"))));
    }
    else if(priority == intpar("AF"))
    {
        t = 2 * (tAFDC[LLID] - getFwdDelay(LLID));
        t *= getR(LLID,priority) * (getOfflineMaxWindow() / (getOfflineMaxWindow() + getDownONULambda(intpar("AF")) * sdCycle->getMean() * getDownFirstMoment(intpar("AF"))));
    }
    else if(priority == intpar("BE"))
    {
        t = 2 * (tBEDC[LLID] - getFwdDelay(LLID));
        t *= getR(LLID,priority) * (getOfflineMaxWindow() / (getOfflineMaxWindow() + getDownONULambda(intpar("BE")) * sdCycle->getMean() * getDownFirstMoment(intpar("BE"))));
    }
    tFwdCycle[LLID].at(priority) = t;
    return t;
}

double testInterScheduler::getR(int LLID,int priority) {
    double dc = 0,d = 0,r = rAdjust[LLID].at(priority),delta = dblpar("rDelta");
    if(priority == intpar("EF")) {dc = tEFDC[LLID].dbl();d = sdEFDelay[LLID]->getMean();}
    else if(priority == intpar("AF")) {dc = tAFDC[LLID].dbl();d = sdAFDelay[LLID]->getMean();}
    else if(priority == intpar("BE")) {dc = tBEDC[LLID].dbl();d = sdBEDelay[LLID]->getMean();}
    if(d > dc && r > dblpar("rMin")) r -= delta;
    else if(d < dc && r < dblpar("rMax")) r += delta;
    rAdjust[LLID].at(priority) = r;
    return std::max(std::min(r,dblpar("rMax")),dblpar("rMin"));
}

simtime_t testInterScheduler::getRemainTime(int LLID) {
    //tNextFwd[LLID].at(0) is the next forward time of ONU
    simtime_t m1 = std::min(tNextFwd[LLID].at(intpar("EF")),tNextFwd[LLID].at(intpar("AF")));
    simtime_t m2 = tNextFwd[LLID].at(intpar("BE"));
    return std::min(m1,m2) - simTime();
}

void testInterScheduler::handleMessageFromSplitter(cMessage* msg) {
    Frame* frame = check_and_cast<Frame*>(msg);
    if(frame->getKind() == intpar("MPCP"))
    {
        handleMPCP(msg);
        delete msg;
    }
    else if(msg->getKind() > intpar("MPCP") && msg->getKind() < intpar("FramePriorities")) send(frame,"txHost");
    else delete frame;
}

void testInterScheduler::handleMPCP(cMessage* msg) {
    CSLAReport* report = check_and_cast<CSLAReport*>(msg);
    int LLID = report->getSRC();
    getMPCP[LLID] = true;
    bRequired[LLID] = std::max(report->getRequest1Length(),queue->getS2Size(LLID));

    //bandwidth allocation
    if(LLID == intpar("numONUs") - 1)
    {
        if(LLID == intpar("debug"))
        {
            log << "[OLT::handleMPCP bandwidthAlloc] t=" << simTime() << endl;
            if(boolpar("err")) std::cerr << "[OLT::bandwidthAlloc] t=" << simTime() << endl;
        }
        bandwidthAlloc(bRequired);
    }

    if(LLID == intpar("debug"))
    {
        log << "[OLT::handleReport] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " w=" << tWindow[LLID] << " q=" << queue->getBitLength(LLID) << " up=" << report->getRequest1Length() << " down=" << queue->getS2Size(LLID) << " remain=" << getRemainTime(LLID) << endl;
        if(boolpar("err")) std::cerr << "[OLT::handleReport] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " w=" << tWindow[LLID] << " q=" << queue->getBitLength(LLID) << " up=" << report->getRequest1Length() << " down=" << queue->getS2Size(LLID) << " remain=" << getRemainTime(LLID) << endl;
    }
}

void testInterScheduler::handleMessageFromHost(cMessage* msg) {
    Frame* frame = check_and_cast<Frame*>(msg);
    queue->insert(frame->getDST(),frame);
}

void testInterScheduler::handleDataBeforeSend(DataFrame* data) {
    int LLID = data->getDST(),priority = data->getKind();
    /*
    data->setAFwdDelay(getFwdDelay(LLID));
    data->setAS1Delay(getFwdCycle(LLID,priority) / 2);
    double b = (getDownONULambda(priority) * getFwdCycle(LLID,priority) * getDownFirstMoment(priority) * sdAvgCycle->getMean()) / getMaxWindow();
    b *= 1 + getPR(LLID,priority);
    data->setAS2Delay(getFwdDelay(LLID) + b / 2);
    data->setABurstTime(b);
    */

    //update estimated delay of each class
    simtime_t d = simTime() - data->getRxTime();
    if(priority == intpar("EF")) sdEFDelay[LLID]->collect(d);
    else if(priority == intpar("AF")) sdAFDelay[LLID]->collect(d);
    else if(priority == intpar("BE")) sdBEDelay[LLID]->collect(d);

    //debug
    if(LLID == intpar("debug"))
    {
        if(data->getTag() == TagFront) std::cerr << "[OLT::sendDataFront] t=" << simTime() << " LLID=" << LLID << " kind=" << priority << endl;
        if(data->getTag() == TagBack) std::cerr << "[OLT::sendDataBack] t=" << simTime() << " LLID=" << LLID << " kind=" << priority << endl;
        log << "[OLT::sendData] t=" << simTime() << " LLID=" << LLID << " kind=" << priority << " size=" << data->getBitLength() << " rx=" << (data->getRxTime()) << " down=" << queue->getS2Size(LLID) << endl;
        if(boolpar("err")) std::cerr << "[OLT::sendData] t=" << simTime() << " LLID=" << LLID << " kind=" << priority << " size=" << data->getBitLength() << " delay=" << (simTime() - data->getRxTime()) << " down=" << queue->getS2Size(LLID) << endl;
    }
}

void testInterScheduler::bandwidthAlloc(std::vector<int> upRequiredBandwidth) {
    //polling cycle
    sdCycle->collect(simTime() - tOLTCycleStart);
    sCycle->collect(0,simTime() - tOLTCycleStart);
    tOLTCycleStart = simTime();

    //required bandwidth from maximum of upstream and downstream
    //update tStart and tWindow by limited service
    simtime_t t = simTime() + timepar("tDBA") + intpar("numONUs") * tMPCP + RTT / 2;
    for(int i = 0;i < intpar("numONUs");++i)
    {
        tRequired[i] = getTransmissionTime(bRequired[i]);
        if(getMPCP[i]) tWindow[i] = std::min(getOfflineMaxWindow(),tRequired[i] * (1 + dblpar("credit")));
        else tWindow[i] = 0;
        tStart[i] = t;
        t += tWindow[i] + tMPCP + tGuard;

        //schedule send gate time
        cMessage* msg = new cMessage("SendMPCP",intpar("SendMPCP"));
        cMsgPar* par = new cMsgPar("LLID");
        par->setLongValue(i);
        msg->addPar(par);
        msg->setSchedulingPriority(2);
        scheduleAt(simTime() + timepar("tDBA") + i * tMPCP,msg);
        scheduled.insert(scheduled.begin(),i);

        //schedule send downstream time
        msg = new cMessage("StartSend",intpar("StartSend"));
        par = new cMsgPar("LLID");
        par->setLongValue(i);
        msg->addPar(par);
        scheduleAt(tStart[i] - RTT / 2,msg);
    }

    //update wake up time
    t -= tGuard;
    t += RTT + timepar("tDBA");
    for(int i = 0;i < intpar("numONUs");++i)
    {
        tWakeup[i] = t;
        t += tMPCP;
    }

}

simtime_t testInterScheduler::getFwdDelay(int LLID) {
    return tOverhead[LLID] + 2 * sdCycle->getMean();
    //return sdFwdDelay[LLID]->getMean();
}

cPacket* testInterScheduler::getNextDataFrame() {
    return queue->s2front(currentONU);
}

cPacket* testInterScheduler::popNextDataFrame() {
    return queue->popS2(currentONU);
}

bool testInterScheduler::queueEmpty() {
    return queue->isEmpty(currentONU);
}

void testInterScheduler::showPollingTable() {
    std::cerr << "[polling table]" << endl;
    for(int i = 0;i < intpar("numONUs");i++) std::cerr << "[" << i << "]\ttStart=" << tStart.at(i) << "\ttWindow=" << tWindow.at(i) << "\tstate=" << state.at(i) << endl;
}

void testInterScheduler::test() {
    /*
    std::cerr << "[testInterScheduler::test]" << endl;
    MultiQueue* q = new MultiQueue();
    q->initialize();
    DataFrame* d1 = new DataFrame("d1",intpar("BE"));
    d1->setDST(L2,0);
    q->insert(d1);

    std::cerr << "front null=" << (q->front(0) == NULL) << endl;
    cPacket* p = q->pop(0);
    std::cerr << "pop n=" << p->getName() << " k=" << p->getKind() << endl;
    */
}

} /* namespace epon */
