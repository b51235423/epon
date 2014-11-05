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

#include "GBAIntraScheduler.h"

namespace epon {
Define_Module(GBAIntraScheduler);

GBAIntraScheduler::GBAIntraScheduler() {

}

GBAIntraScheduler::~GBAIntraScheduler() {

}

void GBAIntraScheduler::initialize() {
    Scheduler::initialize();

    //parameters
    tMPCP = getTransmissionTime(intpar("MPCPSize"));
    tStart = simTime();
    tWindow = 0;

    //statistics
    tActive = 0;
    tDoze = 0;
    tSleep = 0;

    //components
    queue = check_and_cast<Queue*>(getParentModule()->getSubmodule("queue"));
}

void GBAIntraScheduler::finish() {
    double p = dblpar("pActive") * (tActive / simTime()) +
            dblpar("pDoze") * (tDoze / simTime()) +
            dblpar("pSleep") * (tSleep / simTime());

    Recorder* r = check_and_cast<Recorder*>(getParentModule()->getParentModule()->getSubmodule("recorder"));
    std::vector<double> v1 = std::vector<double>();
    v1.push_back(p);
    r->addDataRow("AvgPower",v1);

    std::cerr << "ONU " << getLLID() << endl;
    std::cerr << "\tavg power=" << p << endl;

    Scheduler::finish();
    delete queue;
}

void GBAIntraScheduler::handleSelfMessage(cMessage* msg) {
    //std::cerr << "GBAIntraScheduler::handleSelfMessage() t=" << simTime() << endl;
    if(msg->getKind() == intpar("SendMPCP")) sendMPCP();
    else if(msg->getKind() == intpar("StartSend")) sendDataUntil(tStart + tWindow);
    delete msg;
}

void GBAIntraScheduler::sendMPCP() {
    //std::cerr << "GBAIntraScheduler::sendReport() t=" << simTime() << " LLID=" << getLLID() << " tStart=" << tStart << " tWindow=" << tWindow << endl;

    GBAReport* report = new GBAReport("report",intpar("MPCP"));
    report->setBitLength(intpar("MPCPSize"));
    report->setSRC(getLLID());
    report->setDST(getLLID());
    report->setRequiredBandwidth(queue->getBitLength());
    send(report,"txSplitter");
}

void GBAIntraScheduler::handleMessageFromSplitter(cMessage* msg) {
    //std::cerr << "GBAIntraScheduler::handleMessageFromSplitter() t=" << simTime() << endl;

    Frame* frame = check_and_cast<Frame*>(msg);
    if(frame->getDST() == getLLID())
    {
        if(frame->getKind() == intpar("MPCP"))
        {
            handleMPCP(msg);
            delete msg;
        }
        else if(msg->getKind() > intpar("MPCP") && msg->getKind() < intpar("FramePriorities")) handleDataFromSplitter(frame);
        else delete frame;
    }
    else delete frame;
}

void GBAIntraScheduler::handleMPCP(cMessage* msg) {
    //std::cerr << "GBAIntraScheduler::handleGate() t=" << simTime() << endl;

    //update tStart tWindow and tONUSleep
    GBAGate* gate = check_and_cast<GBAGate*>(msg);
    tStart = gate->getStartTime();
    tWindow = gate->getLength();
    tONUSleep = gate->getONUSleepTime();

    //std::cerr << "GBAIntraScheduler::handleGate() t=" << simTime() << " LLID=" << getLLID() << " tStart=" << tStart << " tWindow=" << tWindow << endl;

    //
    tActive += tWindow + tMPCP;
    tDoze += tMPCP;
    simtime_t TS1 = tStart - simTime(),TS2 = tONUSleep;
    if(TS1 - timepar("tOverhead") <= 0)
    {
        tActive += TS1;
        TS1 = 0;
    }
    if(TS2 - timepar("tDozeOverhead") <= 0)
    {
        tActive += TS2 ;
        TS2 = 0;
    }
    tSleep += TS1 + TS2;

    //schedule
    scheduleAt(tStart,new cMessage("StartSend",intpar("StartSend")));
    scheduleAt(tStart + tWindow,new cMessage("SendReport",intpar("SendMPCP")));
}

void GBAIntraScheduler::handleDataFromSplitter(Frame* data) {
    //std::cerr << "GBAIntraScheduler::handleDataFromSplitter() t=" << simTime() << endl;

    if(simTime() < tStart || simTime() > (tStart + tWindow))
    {
        std::cerr << "tStart=" << tStart << " tWindow=" << tWindow << endl;
        throw cRuntimeError("GBAIntraScheduler::handleDataFromSplitter ONU receive frame while sleep");
    }
    send(data,"txHost");
}

void GBAIntraScheduler::handleMessageFromHost(cMessage* msg) {
    //std::cerr << "GBAIntraScheduler::handleMessageFromHost() t=" << simTime() << endl;

    handleDataFromHost(check_and_cast<DataFrame*>(msg));
}

void GBAIntraScheduler::handleDataFromHost(Frame* data) {
    //std::cerr << "GBAIntraScheduler::handleDataFromHost() t=" << simTime() << endl;

    queue->insert(data);
}

int GBAIntraScheduler::getLLID() {
    //std::cerr << "GBAIntraScheduler::getLLID() t=" << simTime() << endl;

    return getParentModule()->getIndex();
}

} /* namespace epon */
