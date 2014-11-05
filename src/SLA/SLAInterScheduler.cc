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

#include "SLAInterScheduler.h"

namespace epon {
Define_Module(SLAInterScheduler);

SLAInterScheduler::SLAInterScheduler() {

}

SLAInterScheduler::~SLAInterScheduler() {
    for(int i = 0;i < intpar("numONUs");++i) delete sdCycleTime[i];
    delete sdPollingCycleTime;
}

void SLAInterScheduler::initialize() {
    Scheduler::initialize();

    //parameters
    currentONU = 0;
    tAvailable = simTime();
    tPollingCycleStart = simTime();
    tStart.assign(intpar("numONUs"),simTime());
    tWindow.assign(intpar("numONUs"),0);
    tRequired.assign(intpar("numONUs"),0);
    tCycleStart.assign(intpar("numONUs"),0);

    //SLA parameters
    tSleep = timepar("tSleep");
    tIdleStartInitial = 0;
    tIdleStart.assign(intpar("numONUs"),tIdleStartInitial);
    tRemain.assign(intpar("numONUs"),0);
    mSleepInitial = 1;
    mSleep.assign(intpar("numONUs"),mSleepInitial);
    state.assign(intpar("numONUs"),ACTIVE);
    times.assign(intpar("numONUs"),0);
    flagHandleReport.assign(intpar("numONUs"),false);

    //components
    switch(intpar("downScheduling"))
    {
    case UPSTREAMCENTRIC:
        queue = check_and_cast<MultiQueue*>(getParentModule()->getSubmodule("multiqueue"));
        //alternative DBA
        break;
    /*
    case BROADCAST:
        queue = check_and_cast<Queue*>(getParentModule()->getSubmodule("queue"));
        break;
    */
    }

    //statistics
    for(int i = 0;i < intpar("numONUs");++i) sdCycleTime.push_back(new cStdDev("sdCycleTime"));
    sdPollingCycleTime = new cStdDev("sdPollingCycleTime");

    //set run tag
    std::stringstream sstag;
    sstag << "ts" << dblpar("tSleep") << "_g" << dblpar("g") << "_M" << dblpar("M");
    std::string stag = sstag.str();
    rec()->setTag(stag);

    //initial schedule
    for(int i = 0;i < intpar("numONUs");i++) bandwidthAlloc(i,0,0);

    //debug
}

void SLAInterScheduler::finish() {
    Scheduler::finish();

    simtime_t tAvgCycle = 0,tAvgStdDevCycle = 0;
    for(int i = 0;i < intpar("numONUs");++i)
    {
        tAvgCycle += sdCycleTime[i]->getMean() / intpar("numONUs");
        tAvgStdDevCycle += sdCycleTime[i]->getStddev() / intpar("numONUs");
    }

    recordScalar("ONU cycle time avg",tAvgCycle);
    recordScalar("Polling cycle time avg",sdPollingCycleTime->getMean());

    std::vector<double> v = std::vector<double>();
    v.push_back(tAvgCycle.dbl());
    v.push_back(tAvgStdDevCycle.dbl());
    rec()->addDataRow("AvgONUCycle",v);

    std::vector<double> v1 = std::vector<double>();
    v1.push_back(sdPollingCycleTime->getMean());
    v1.push_back(sdPollingCycleTime->getStddev());
    rec()->addDataRow("AvgPollingCycle",v1);

    //debug
    if(log.str().length() > 0) outputLog();
    for(int i = 0;i < intpar("numONUs");i++) std::cerr << " times[" << i << "]=" << times[i] << "\tstate=" << state[i] << endl;
}

void SLAInterScheduler::updateState(int LLID,int signal) {
    //
    switch(state[LLID])
    {
    case ACTIVE:
        switch(signal)
        {
        case NOSIGNAL:
        case NONE:
            if(tWindow[LLID] == 0 && queue->getLength(LLID) == 0)
            {
                state[LLID] = IDLE;
                if(tIdleStart[LLID] == tIdleStartInitial) tIdleStart[LLID] = simTime();
            }
            break;

        case ACK:
        case NACK:
        default:
            std::cerr << "[OLT::updateState] exception LLID=" << LLID << " state=" << state[LLID] << " signal=" << signal << endl;
            break;
        }
        break;
    case IDLE:
        switch(signal)
        {
        case NOSIGNAL:
        case NONE:
        case NACK:
            //if((getIdleTime(LLID) < timepar("tExtraGuard") && (tWindow[LLID] > 0 || !queue->isEmpty(LLID))) || (getIdleTime(LLID) >= timepar("tExtraGuard") && (tWindow[LLID] > 0 || !queue->isEmptyByPriority(LLID,kindEF))))
            if(tWindow[LLID] > 0 || !queue->isEmpty(LLID))
            {
                state[LLID] = ACTIVE;
                tIdleStart[LLID] = tIdleStartInitial;
                mSleep[LLID] = mSleepInitial;
            }
            break;

        case ACK:
            state[LLID] = SLEEP;
            if(mSleep[LLID] == mSleepInitial || (mSleep[LLID] < intpar("M") && uniform(0,1) <= dblpar("g"))) ++mSleep[LLID];
            break;

        default:
            std::cerr << "[OLT::updateState] exception LLID=" << LLID << " state=" << state[LLID] << " signal=" << signal << endl;
            break;
        }
        break;
    case SLEEP:
        //
        switch(signal)
        {
        case NOSIGNAL:
            break;
        case NONE:
            state[LLID] = IDLE;
            tIdleStart[LLID] = simTime();
            break;

        case ACK:
        case NACK:
        default:
            std::cerr << "[OLT::updateState] exception LLID=" << LLID << " state=" << state[LLID] << " signal=" << signal << endl;
            break;
        }
        break;
    }
}

void SLAInterScheduler::updateStateTest(int LLID,int signal) {
    switch(state[LLID])
    {
    case ACTIVE:
        switch(signal)
        {
        case NOSIGNAL:
        case NONE:
            if(tWindow[LLID] == 0 && queue->getForwardSize(LLID) == 0)
            {
                state[LLID] = IDLE;
                if(tIdleStart[LLID] == tIdleStartInitial) tIdleStart[LLID] = simTime();
            }
            break;

        case ACK:
        case NACK:
        default:
            std::cerr << "[OLT::updateState] exception LLID=" << LLID << " state=" << state[LLID] << " signal=" << signal << endl;
            break;
        }
        break;
    case IDLE:
        switch(signal)
        {
        case NOSIGNAL:
        case NONE:
        case NACK:
            //if((getIdleTime(LLID) < timepar("tExtraGuard") && (tWindow[LLID] > 0 || !queue->isEmpty(LLID))) || (getIdleTime(LLID) >= timepar("tExtraGuard") && (tWindow[LLID] > 0 || !queue->isEmptyByPriority(LLID,kindEF))))
            if(tWindow[LLID] > 0 || queue->getForwardSize(LLID) > 0)
            {
                state[LLID] = ACTIVE;
                tIdleStart[LLID] = tIdleStartInitial;
                mSleep[LLID] = mSleepInitial;
            }
            break;

        case ACK:
            state[LLID] = SLEEP;
            //if(mSleep[LLID] == mSleepInitial || (mSleep[LLID] < intpar("M") && uniform(0,1) <= dblpar("g"))) ++mSleep[LLID];
            break;

        default:
            std::cerr << "[OLT::updateState] exception LLID=" << LLID << " state=" << state[LLID] << " signal=" << signal << endl;
            break;
        }
        break;
    case SLEEP:
        //
        switch(signal)
        {
        case NOSIGNAL:
            break;
        case NONE:
            state[LLID] = IDLE;
            tIdleStart[LLID] = simTime();
            break;

        case ACK:
        case NACK:
        default:
            std::cerr << "[OLT::updateState] exception LLID=" << LLID << " state=" << state[LLID] << " signal=" << signal << endl;
            break;
        }
        break;
    }
}

simtime_t SLAInterScheduler::getIdleTime(int LLID) {
    if(tIdleStart[LLID] == tIdleStartInitial) return 0;
    return simTime() - tIdleStart[LLID];
}

void SLAInterScheduler::handleSelfMessage(cMessage* msg) {
    int LLID = int(msg->par("LLID"));
    if(msg->getKind() == intpar("CheckMPCP"))
    {
        //When an ONU is sleeping, it can't respond to the OLT's sleep gate, and without the ONU
        //sending a sleep report to the OLT, the OLT can't schedule next sleep gate to the ONU.
        //The OLT should schedule a event at the time after the time OLT will get ONU's sleep
        //report to check if the OLT really get report from ONU or not. If the OLT don't get a
        //sleep report from the ONU, the OLT should do bandwidth allocation invoke next send gate
        //event for the sleeping ONU to send next sleep gate.
        if(!flagHandleReport[LLID]) bandwidthAlloc(LLID,0,0);
    }
    else if(msg->getKind() == intpar("SendMPCP"))
    {
        //Downstream transmission window start
        currentONU = int(msg->par("LLID"));
        sendMPCP(int(msg->par("LLID")));
        scheduled.pop_back();
        scheduleDownstream();
    }
    delete msg;
}

void SLAInterScheduler::sendMPCP(int LLID) {
    simtime_t tIdleTimer = 0;

    //alternative DBA
    tIdleTimer = mSleep[LLID] == mSleepInitial ? timepar("tExtraGuard") : timepar("tExtraGuard") + tSleep * (mSleep[LLID] - 1);
    updateState(LLID,NOSIGNAL);

    SLAGate* gate = new SLAGate("gate");
    gate->setSRC(getLLID());
    gate->setDST(LLID);
    gate->setBitLength(intpar("MPCPSize"));
    gate->setStartTime(tStart[LLID]);
    gate->setLength(0);
    gate->setSignal(NONE);
    gate->setSleepTime(0);


    //alternative DBA
    if(boolpar("enabled") && (state[LLID] == IDLE && (getIdleTime(LLID) >= tIdleTimer)))
    {
        //send sleep gate when idle time is more than extra guard
        gate->setSignal(ACK);
        gate->setSleepTime(tSleep * mSleep[LLID]);
        if(LLID == debug) log << "[OLT::sendSleepGate] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " st=" << gate->getSleepTime() << endl;
        send(gate,"txSplitter");
    }
    else if(boolpar("enabled") && state[LLID] == SLEEP)
    {
        //send sleep gate when ONU is not scheduled
        if(LLID == debug) log << "[OLT::sendSleepGate] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " st=" << gate->getSleepTime() << endl;
        send(gate,"txSplitter");
    }
    else
    {
        //send normal gate
        gate->setLength(tWindow[LLID]);
        send(gate,"txSplitter");
        if(LLID == debug) log << "[OLT::sendGate] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << " w=" << tWindow[LLID] << endl;
    }

    //check report if ONU is sleep
    if(state[LLID] == SLEEP)
    {
        flagHandleReport[LLID] = false;
        cMessage* msg = new cMessage("CheckReport",intpar("CheckMPCP"));
        cMsgPar* par = new cMsgPar("LLID");
        par->setLongValue(LLID);
        msg->addPar(par);
        scheduleAt(tStart[LLID] + tWindow[LLID] + tMPCP + RTT / 2,msg);
    }
}

void SLAInterScheduler::scheduleDownstream() {
    if(state[currentONU] == ACTIVE)
        switch(intpar("downScheduling"))
        {
        case UPSTREAMCENTRIC:
            sendDataUntil(tStart[currentONU] + tWindow[currentONU] + tGuard - RTT / 2);
            break;
        /*
        case BROADCAST:
            if(scheduled.size() > 0) sendDataUntil(tStart[scheduled.back()] - tMPCP - RTT / 2);
            else sendDataUntil(tAvailable - tMPCP - RTT);
            break;
        */
        }
}

void SLAInterScheduler::handleMessageFromSplitter(cMessage* msg) {
    Frame* frame = check_and_cast<Frame*>(msg);
    if(frame->getKind() == intpar("MPCP"))
    {
        handleMPCP(msg);
        delete msg;
    }
    else if(msg->getKind() > intpar("MPCP") && msg->getKind() < intpar("FramePriorities")) send(frame,"txHost");
    else delete frame;
}

void SLAInterScheduler::handleMPCP(cMessage* msg) {
    SLAReport* report = check_and_cast<SLAReport*>(msg);
    int LLID = report->getSRC();
    flagHandleReport[LLID] = true;

    //cycle
    sdCycleTime[LLID]->collect(simTime() - tCycleStart[LLID]);
    //queue->setCycleTime(LLID,sdCycleTime[LLID]->getMean());
    tCycleStart[LLID] = simTime();

    //bandwidth allocation
    int down = 0;

    //alternative DBA
    //down = getIdleTime(LLID) < timepar("tExtraGuard") ? queue->getBitLength(LLID) : queue->getBitLengthByPriority(LLID,kindEF);
    down = queue->getBitLength(LLID);

    switch(intpar("downScheduling"))
    {
        case UPSTREAMCENTRIC:
            bandwidthAlloc(LLID,report->getRequiredBandwidth(),down);
            break;
        /*
        case BROADCAST:
            r = std::min(int(queue->getLength() / dblpar("numONUs")) + intpar("dcredit"),queue->getLength());
            if(!queue->isEmpty()) for(int i = 0;i < r;++i) bits += queue->get(i)->getBitLength();
            //std::cerr << "OLT::handleReport u=" << report->getRequiredBitlength() << " d=" << bits << " f=" << front << endl;
            bandwidthAlloc(report->getLLID(),report->getRequiredBitlength(),bits);
            break;
        */
    }

    if(LLID == debug)
    {
        if(report->getSignal() == NONE) log << "[OLT::handleReport] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << "->";
        if(report->getSignal() == ACK) log << "[OLT::handleACK] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << "->";
        if(report->getSignal() == NACK) log << "[OLT::handleNACK] t=" << simTime() << " LLID=" << LLID << " state=" << state[LLID] << "->";
    }

    //alternative DBA
    updateState(LLID,report->getSignal());

    if(LLID == debug) log << state[LLID] << " w=" << tWindow[LLID] << " q=" << queue->getBitLength(LLID) << " up=" << report->getRequiredBandwidth() << " down=" << down << endl;
}

void SLAInterScheduler::handleMessageFromHost(cMessage* msg) {
    Frame* frame = check_and_cast<Frame*>(msg);
    if(state[frame->getDST()] == SLEEP) frame->setTimestamp(intpar("SLEEP"));
    queue->insert(frame);
    scheduleDownstream();
}

void SLAInterScheduler::handleDataBeforeSend(DataFrame* data) {

    if(data->getDST() == debug)
        log << "[OLT::sendData] t=" << simTime() << " LLID=" << data->getDST() << " kind=" << data->getKind() << " size=" << data->getBitLength() << " delay=" << (simTime() - data->getRxTime()) << " down=" << queue->getForwardSize(data->getDST()) << endl;

}

void SLAInterScheduler::bandwidthAlloc(int LLID,int upRequiredBitlength,int downRequiredBitlength) {
    if(int(times.size()) == intpar("numONUs")) ++times[LLID];

    //polling cycle
    if(LLID == 0)
    {
        sdPollingCycleTime->collect(simTime() - tPollingCycleStart);
        tPollingCycleStart = simTime();
    }

    //required bandwidth
    tRequired[LLID] = getTransmissionTime(upRequiredBitlength);
    simtime_t tDownRequired = getTransmissionTime(downRequiredBitlength);
    tRequired[LLID] = std::max(tDownRequired,tRequired[LLID]);

    //update tStart and tWindow
    //if transmission window is unable to cover the RTT
    simtime_t tNextAvailable = simTime() + RTT + tMPCP;
    if(tAvailable < tNextAvailable) tAvailable = tNextAvailable;
    tStart[LLID] = tAvailable + tProcess - RTT / 2;

    //linear credit service
    simtime_t tCredit = tRequired[LLID] * (1 + dblpar("credit"));
    tWindow[LLID] = tCredit <= getOnlineMaxWindow() ? tCredit : getOnlineMaxWindow();
    tAvailable += tMPCP + tWindow[LLID] + tGuard;

    //schedule to send gate
    cMessage* msg = new cMessage("SendGate",intpar("SendMPCP"));
    cMsgPar* par = new cMsgPar("LLID");
    par->setLongValue(LLID);
    msg->addPar(par);
    scheduleAt(tStart[LLID] - tMPCP - RTT / 2,msg);
    scheduled.insert(scheduled.begin(),LLID);
}

cPacket* SLAInterScheduler::getNextDataFrame() {
    switch(intpar("downScheduling"))
    {
    case UPSTREAMCENTRIC:
        return queue->front(currentONU);
        break;
    /*
    case BROADCAST:
        for(int i = 0;i < intpar("numONUs");i++) if(state[i] == ACTIVE && queue->front(i) != NULL) return queue->front(i);
        return NULL;
        break;
    */
    }
    throw cRuntimeError("undefined downstream scheduling mode");
}

cPacket* SLAInterScheduler::popNextDataFrame() {
    switch(intpar("downScheduling"))
    {
    case UPSTREAMCENTRIC:
        return queue->pop(currentONU);
        break;
    }
    throw cRuntimeError("undefined downstream scheduling mode");
}

bool SLAInterScheduler::queueEmpty() {
    switch(intpar("downScheduling"))
    {
    case UPSTREAMCENTRIC:
        return queue->isEmpty(currentONU);
        break;
    /*
    case BROADCAST:
        return Scheduler::queueEmpty();
        break;
    */
    }
    throw cRuntimeError("undefined downstream scheduling mode");
}

void SLAInterScheduler::showPollingTable() {
    std::cerr << "[polling table]" << endl;
    for(int i = 0;i < intpar("numONUs");i++) std::cerr << "[" << i << "]\ttStart=" << tStart.at(i) << "\ttWindow=" << tWindow.at(i) << "\tstate=" << state.at(i) << endl;
}

void SLAInterScheduler::test() {
    /*
    std::cerr << "[SLAInterScheduler::test]" << endl;
    MultiQueue* q = new MultiQueue();
    q->initialize();
    DataFrame* d1 = new DataFrame("d1",kindBE);
    d1->setDST(L2,0);
    q->insert(d1);

    std::cerr << "front null=" << (q->front(0) == NULL) << endl;
    cPacket* p = q->pop(0);
    std::cerr << "pop n=" << p->getName() << " k=" << p->getKind() << endl;
    */
}

} /* namespace epon */
