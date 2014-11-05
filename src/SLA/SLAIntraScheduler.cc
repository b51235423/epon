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

#include "SLAIntraScheduler.h"

namespace epon {
Define_Module(SLAIntraScheduler);

SLAIntraScheduler::SLAIntraScheduler() {

}

SLAIntraScheduler::~SLAIntraScheduler() {
    delete sdCycleTime;
    delete sdControl;
    delete sdWindow;
    delete sdSleepTime;
    delete sdSleepOverhead;
    delete sdBurstSize;
    delete sdBurstLength;
}

void SLAIntraScheduler::initialize() {
    Scheduler::initialize();

    //parameters
    signal = 0;
    tStart = simTime();
    tWindow = 0;

    //SLA paremeter
    state = ACTIVE;
    tCheckpoint = simTime();
    tSim = 0;
    tIdleStartInitial = 0;
    tIdleStart = tIdleStartInitial;

    //components
    queue = check_and_cast<Queue*>(getParentModule()->getSubmodule("queue"));

    //alternative DBA

    //statistics
    sdCycleTime = new cStdDev("sdCycleTime");
    sdControl = new cStdDev("sdControl");
    sdWindow = new cStdDev("sdWindow");
    sdSleepTime = new cStdDev("sdSleepTime");
    sdSleepOverhead = new cStdDev("sdSleepOverhead");
    sdBurstSize = new cStdDev("sdBurstSize");
    sdBurstLength = new cStdDev("sdBurstLength");

    //analyze
    //upAnalyzer()->setONU(getLLID(),queue,txSplitter);

    //debug
    //test();
}

void SLAIntraScheduler::finish() {
    Scheduler::finish();

    tActive.push_back(simTime() - tCheckpoint);
    tSim += (simTime() - tCheckpoint);

    //
    double energy = 0;
    simtime_t tActiveSum = 0,tSleepSum = 0;
    for(int i = 0;i < int(tActive.size());++i)
    {
        energy += tActive[i].dbl() * dblpar("pActive");
        tActiveSum += tActive[i];
        if(getLLID() == debug) log << "active" << i << "t=" << tActive[i]  << " a=" << tActive[i].dbl() * dblpar("pActive") << endl;
    }
    for(int i = 0;i < int(tSleep.size());++i)
    {
        //energy += std::min(getEnergy(tSleep[i]),tSleep[i].dbl() * dblpar("pActive"));
        energy += tActive[i].dbl() * dblpar("pSleep");
        tSleepSum += tSleep[i];
        if(getLLID() == debug) log << "sleep" << i << "t=" << tSleep[i] << " s=" << getEnergy(tSleep[i]) << " a=" << tSleep[i].dbl() * dblpar("pActive") << endl;
    }

    double pSleep = (energy - (tSim.dbl() - tSleepSum.dbl()) * dblpar("pActive")) / tSleepSum.dbl();

    recordScalar("sleep times",sdSleepTime->getCount(),"");
    recordScalar("sleep time avg",sdSleepTime->getMean(),"s");
    recordScalar("sleep time stddev",sdSleepTime->getStddev(),"s");
    recordScalar("sleep time min",sdSleepTime->getMin(),"s");
    recordScalar("sleep time max",sdSleepTime->getMax(),"s");
    recordScalar("sleep time total",sdSleepTime->getSum(),"s");
    recordScalar("sleep time ratio",sdSleepTime->getSum() / simTime(),"s");
    recordScalar("energy consumed",energy,"");
    recordScalar("power avg",energy / simTime(),"W");
    recordScalar("power sleep avg",pSleep,"W");

    std::vector<double> v1 = std::vector<double>();
    v1.push_back(energy / simTime());
    v1.push_back(pSleep);
    rec()->addDataRow("AvgPower",v1);

    std::vector<double> v2 = std::vector<double>();
    v2.push_back(sdSleepTime->getMean());
    v2.push_back(sdWindow->getMean());
    rec()->addDataRow("AvgSleepAndWindowTime",v2);

    std::vector<double> v3 = std::vector<double>();
    v3.push_back(sdSleepTime->getCount() / simTime());
    v3.push_back(sdWindow->getCount() / simTime());
    rec()->addDataRow("AvgSleepAndWindowTPS",v3);

    std::vector<double> v4 = std::vector<double>();
    v4.push_back(getTransmissionTime(sdBurstSize->getMean()).dbl());
    rec()->addDataRow("AvgBurst",v4);

    std::vector<double> v5 = std::vector<double>();
    v5.push_back(getTransmissionTime(sdBurstSize->getMean()) / getOnlineMaxWindow());
    v5.push_back(sdWindow->getMean() / getOnlineMaxWindow());
    rec()->addDataRow("AvgBurstAndWindowMaxWindowRatio",v5);

    std::vector<double> v6 = std::vector<double>();
    double sleep = sdSleepTime->getSum() / simTime();
    double overhead = sdSleepOverhead->getSum() / simTime();
    double control = sdControl->getSum() / simTime();
    double window = sdWindow->getSum() / simTime();
    double other = 1.0 - sleep - overhead - control - window;
    v6.push_back(sleep);
    v6.push_back(overhead);
    v6.push_back(control);
    v6.push_back(window);
    v6.push_back(other);
    rec()->addDataRow("AvgDistribution",v6);

    //debug
    if(getLLID() == debug) outputLog();
    std::cerr << "[ONU" << getLLID() << "::finish]" << endl;
    std::cerr << "\tAverage Power=" << (energy / simTime()) << endl;
    std::cerr << "\tSimulation Time=" << tSim << endl;
    std::cerr << "\tActive Time=" << tActiveSum << endl;
    std::cerr << "\tSleep Time=" << tSleepSum << endl;
    std::cerr << "\tstate=" << state << endl;
}


void SLAIntraScheduler::updateState(int signal) {
    switch(state)
    {
    case ACTIVE:
        switch(signal)
        {
        case NONE:
            if(tWindow == 0 && queue->getLength() == 0)
            {
                state = IDLE;
                if(tIdleStart == tIdleStartInitial) tIdleStart = simTime();
            }
            else tIdleStart = tIdleStartInitial;
            break;

        case ACK:
        case NACK:
        default:
            std::cerr << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
            break;
        }
        break;
    case IDLE:
        switch(signal)
        {
        case NONE:
            //if((getIdleTime() < timepar("tExtraGuard") && (tWindow > 0 || !queue->isEmpty())) || (getIdleTime() >= timepar("tExtraGuard") && (tWindow > 0 || !queue->isEmptyByPriority(intpar("EF")))))
            if(tWindow > 0 || !queue->isEmpty())
            {
                state = ACTIVE;
                tIdleStart = tIdleStartInitial;
            }
            break;

        case ACK:
            if(queue->isEmptyByPriority(intpar("EF"))) state = SLEEP;
            break;

        case NACK:
        default:
            std::cerr << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
            break;
        }
        break;
    case SLEEP:
        std::cerr << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
        break;
    }
}

void SLAIntraScheduler::updateStateTest(int signal) {
    switch(state)
    {
    case ACTIVE:
        switch(signal)
        {
        case NONE:
            if(tWindow == 0 && queue->getForwardSize() == 0)
            {
                state = IDLE;
                if(tIdleStart == tIdleStartInitial) tIdleStart = simTime();
            }
            else tIdleStart = tIdleStartInitial;
            break;

        case ACK:
            if(queue->getForwardSize() == 0) state = SLEEP;
            break;

        case NACK:
        default:
            std::cerr << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
            log << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
            break;
        }
        break;
    case IDLE:
        switch(signal)
        {
        case NONE:
            if(tWindow > 0 || queue->getForwardSize() > 0)
            {
                state = ACTIVE;
                tIdleStart = tIdleStartInitial;
            }
            break;

        case ACK:
            if(queue->getForwardSize() == 0) state = SLEEP;
            break;

        case NACK:
        default:
            std::cerr << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
            log << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
            break;
        }
        break;
    case SLEEP:
        std::cerr << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
        log << "[ONU" << getLLID() << "::updateState] exception state=" << state << " signal=" << signal << endl;
        break;
    }
}

simtime_t SLAIntraScheduler::getIdleTime() {
    if(tIdleStart == 0) return 0;
    return simTime() - tIdleStart;
}

void SLAIntraScheduler::handleSelfMessage(cMessage* msg) {
    simtime_t sleepTime;
    if(msg->getKind() == intpar("WakeUp"))
    {
        if(getLLID() == debug) log << "[ONU" << getLLID() << "::wakeup] t=" << simTime() << " state=" << state << " sleepStart=" << tCheckpoint << " l=" << queue->getLength() << endl;

        state = ACTIVE;

        //calculate energy consumption in sleep period and wake up period
        sleepTime = simTime() - tCheckpoint - timepar("tWakeup");
        if(sleepTime < 0) throw cRuntimeError("wakeup with negative sleep time t=%f st=%f",simTime().dbl(),tCheckpoint.dbl());

        tSim += (sleepTime + timepar("tWakeup"));
        tCheckpoint = simTime();
        tActive.push_back(timepar("tWakeup"));
        tSleep.push_back(sleepTime);
        sdSleepTime->collect(sleepTime);
        sdSleepOverhead->collect(timepar("tWakeup"));
        if(queue->getBitLength() > 0)
        {
            sdBurstSize->collect(queue->getBitLength());
            sdBurstLength->collect(queue->getLength());
        }
    }
    else if(msg->getKind() == intpar("SendMPCP"))
    {
        sendMPCP();
    }
    delete msg;
}

void SLAIntraScheduler::sendMPCP() {
    //upAnalyzer()->setCurrentONU(Analyzer::noCurrentONU,0);

    SLAReport* report = new SLAReport("report",intpar("MPCP"));
    report->setBitLength(intpar("MPCPSize"));
    report->setSRC(getLLID());
    report->setDST(getLLID());

    //alternative DBA

    int require = 0;
    switch(signal)
    {
    case NONE:
        report->setSignal(NONE);

        //alternative DBA
        require = queue->getBitLength();

        if(getLLID() == debug)
            log << "[ONU" << getLLID() << "::sendReport] t=" << simTime() << " state=" << state << " up=" << queue->getForwardSize() << endl;
        break;
    case ACK:
        //alternative DBA
        if(state == SLEEP)
        {
            report->setSignal(ACK);
            if(getLLID() == debug) log << "[ONU" << getLLID() << "::sendACK] t=" << simTime() << " state=" << state << endl;
        }
        else
        {
            report->setSignal(NACK);
            //require = queue->getBitLengthByPriority(intpar("EF"));
            require = queue->getBitLength();
            if(getLLID() == debug)
                log << "[ONU" << getLLID() << "::sendNACK] t=" << simTime() << " state=" << state << " idle=" << getIdleTime()
                << " require=" << require << " q=" << queue->getLength() << " hq=" << queue->getLengthByPriority(intpar("EF")) << endl;
        }
        break;
    case NACK:
        std::cerr << "[ONU" << getLLID() << "::sendMPCP] signal exception, state=" << state << " signal=" << signal << endl;
        break;
    }
    report->setRequiredBandwidth(require);
    send(report,"txSplitter");
    sdControl->collect(tMPCP);
}

void SLAIntraScheduler::handleMessageFromSplitter(cMessage* msg) {
    Frame* frame = check_and_cast<Frame*>(msg);
    if(frame->getDST() == getLLID())
    {
        if(frame->getKind() == intpar("MPCP"))
        {
            if(state != SLEEP) handleMPCP(msg);
            delete msg;
        }
        else if(msg->getKind() > intpar("MPCP") && msg->getKind() < intpar("FramePriorities"))
        {
            if(state == SLEEP) throw cRuntimeError("receive data from OLT while ONU is sleeping",getLLID());
            send(frame,"txHost");
        }
        else delete frame;
    }
    else delete frame;
}

void SLAIntraScheduler::handleMPCP(cMessage* msg) {
    //cycle
    sdCycleTime->collect(simTime() - tStart);
    //queue->setCycleTime(sdCycleTime->getMean());

    SLAGate* gate = check_and_cast<SLAGate*>(msg);
    signal = gate->getSignal();
    tStart = gate->getStartTime();
    tWindow = gate->getLength();

    //upAnalyzer()->setCurrentONU(getLLID(),tStart);

    if(getLLID() == debug)
    {
        if(signal == NONE) log << "[ONU" << getLLID() << "::handleGate] t=" << simTime() << " state=" << state << "->";
        else log << "[ONU" << getLLID() << "::handleSleepGate] t=" << simTime() << " state=" << state << "->";
    }

    //alternative DBA
    updateState(signal);

    if(getLLID() == debug)
        log << state << " s=" << tStart << " w=" << tWindow << " q=" << queue->getBitLength() << " up=" << queue->getForwardSize() << endl;

    switch(state)
    {
    case ACTIVE:
        //update tStart and tWindow
        sendDataUntil(tStart + tWindow);
        sdWindow->collect(tWindow);
        break;
    case IDLE:
        break;
    case SLEEP:
        //calculate energy
        tActive.push_back(simTime() - tCheckpoint);
        tSim += (simTime() - tCheckpoint);
        tCheckpoint = simTime();

        //schedule wake up time
        waken = false;
        msgWakeup = new cMessage("Wakeup",intpar("WakeUp"));
        scheduleAt(simTime() + gate->getSleepTime(),msgWakeup);

        if(getLLID() == debug) log << "[ONU" << getLLID() << "::sleep] t=" << simTime() << " sleepTime=" << gate->getSleepTime() << endl;
        break;
    }
    scheduleAt(tStart + tWindow,new cMessage("SendMPCP",intpar("SendMPCP")));
}

double SLAIntraScheduler::getEnergy(simtime_t sleepTime) {
    //do integral
    double energy,scale = 10000;
    for(int i = 0;i < sleepTime.dbl() * scale;i++)
    {
        energy += getPower(double(i) / scale) / scale;
    }
    return energy;
}

double SLAIntraScheduler::getPower(simtime_t sleepTime) {
    //change the unit of sleepTime to ms
    double t = sleepTime.dbl() * 1000;
    double a = 0.667,b = 14.472,c = 1.131;
    if(t <= 5) return -a * t + dblpar("pActive");
    else return b * pow(t,-c) + dblpar("pSleep");
}

void SLAIntraScheduler::handleMessageFromHost(cMessage* msg) {
    DataFrame* data = check_and_cast<DataFrame*>(msg);
    //upAnalyzer()->addResidualTime(data);
    //upAnalyzer()->addQueueTime(data);
    queue->insert(data);

    if(state == SLEEP)
    {
        data->setTimestamp(intpar("SLEEP"));
        //wake up by upstream EF packets
        if(!waken && data->getKind() == intpar("EF") && (msgWakeup->getArrivalTime() - simTime()) > timepar("tWakeup"))
        {
            waken = true;
            cancelAndDelete(msgWakeup);
            msgWakeup = new cMessage("Wakeup",intpar("WakeUp"));
            scheduleAt(simTime() + timepar("tWakeup"),msgWakeup);
        }
    }

    //for constant and linear credit
    sendDataUntil(tStart + tWindow);

    //debug
    if(getLLID() == debug && data->getKind() == intpar("BE") && state == ACTIVE)
        log << "[ONU" << getLLID() << "::receiveBE] t=" << simTime() << " size=" << data->getBitLength() << " rx=" << data->getRxTime() << " state=" << state << " q=" << queue->getLength() << endl;
    if(getLLID() == debug && data->getKind() == intpar("BE") && state == SLEEP)
        log << "[ONU" << getLLID() << "::receiveSleepBE] t=" << simTime() << " size=" << data->getBitLength() << " rx=" << data->getRxTime() << " state=" << state << " q=" << queue->getLength() << endl;
    if(getLLID() == debug && data->getKind() == intpar("EF") && state == ACTIVE)
        log << "[ONU" << getLLID() << "::receiveEF] t=" << simTime()
        << " size=" << data->getBitLength()
        << " rx=" << data->getRxTime()
        << " state=" << state
        << " qs=" << queue->getBitLength()
        << " tStart=" << tStart
        << " tWindow=" << tWindow
        << endl;
    if(getLLID() == debug && data->getKind() == intpar("EF") && state == SLEEP)
        log << "[ONU" << getLLID() << "::receiveSleepEF] t=" << simTime() << " size=" << data->getBitLength() << " rx=" << data->getRxTime() << " state=" << state << " q=" << queue->getLength() << endl;

}

void SLAIntraScheduler::handleDataBeforeSend(DataFrame* data) {
    //debug
    if(getLLID() == debug)
        log << "[ONU" << getLLID() << "::sendData] t=" << simTime() << " kind=" << data->getKind() << " size=" << data->getBitLength() << " delay=" << (simTime() - data->getRxTime()) << " up=" << queue->getForwardSize() << endl;
}

cPacket* SLAIntraScheduler::getNextDataFrame() {
    return queue->front();
}

cPacket* SLAIntraScheduler::popNextDataFrame() {
    return queue->pop();
}

bool SLAIntraScheduler::queueEmpty() {
    return queue->isEmpty();
}

void SLAIntraScheduler::test() {
    if(getLLID() == 0)
    {
        std::cerr << "[SLAIntraScheduler::test]" << endl;
        Queue* queue = new Queue();
        queue->initialize();
        queue->setNonStrict(true);
        DataFrame* d1 = new DataFrame("d1",intpar("EF"));
        d1->setRxTime(-0.005);
        d1->setBitLength(512);
        queue->insert(d1);
        DataFrame* d2 = new DataFrame("d2",intpar("EF"));
        d2->setRxTime(-0.004);
        d2->setBitLength(512);
        queue->insert(d2);
        DataFrame* d3 = new DataFrame("d3",intpar("AF"));
        d3->setRxTime(-0.003);
        d3->setBitLength(512);
        queue->insert(d3);
        DataFrame* d4 = new DataFrame("d4",intpar("AF"));
        d4->setRxTime(-0.002);
        d4->setBitLength(512);
        queue->insert(d4);
        DataFrame* d5 = new DataFrame("d5",intpar("BE"));
        d5->setRxTime(-0.001);
        d5->setBitLength(512);
        queue->insert(d5);

        queue->list();

        //queue->reportByDelay();

        std::cerr << "[SLAIntraScheduler::test] reportLength=" << queue->getForwardLength() << endl;
        std::cerr << "[SLAIntraScheduler::test] reportSize=" << queue->getForwardSize() << endl;

        for(int i = 0;i < queue->getLength();++i)
        {
            queue->pop();
            queue->list();
            std::cerr << "[SLAIntraScheduler::test] reportLength=" << queue->getForwardLength() << endl;
            std::cerr << "[SLAIntraScheduler::test] reportSize=" << queue->getForwardSize() << endl;
        }
    }
}

} /* namespace epon */
