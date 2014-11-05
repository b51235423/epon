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

#include "testIntraScheduler.h"

namespace epon {
Define_Module(testIntraScheduler);

testIntraScheduler::testIntraScheduler() {

}

testIntraScheduler::~testIntraScheduler() {
    delete sdEFDelay;
    delete sdAFDelay;
    delete sdBEDelay;
    delete sPower;
    delete sRatio;
    delete sAdjust;
    delete sFwdCycle;
}

void testIntraScheduler::initialize() {
    Scheduler::initialize();

    //parameters
    signal = 0;
    tStart = simTime();
    tWindow = 0;

    //proposed method parameter
    state = ACTIVE;
    tCheckpoint = simTime();
    tAvgCycle = 0;
    tNextFwd.assign(intpar("FramePriorities"),0);
    tFwdCycle.push_back(0);
    tFwdCycle.push_back(timepar("tEFDC") * 2);
    tFwdCycle.push_back(timepar("tAFDC") * 2);
    tFwdCycle.push_back(timepar("tBEDC") * 2);
    rAdjust.assign(intpar("FramePriorities"),1);
    sdEFDelay = new cStdDev("sdEFDelay");
    sdAFDelay = new cStdDev("sdAFDelay");
    sdBEDelay = new cStdDev("sdBEDelay");

    //components
    queue = check_and_cast<TwoStageQueue*>(getParentModule()->getSubmodule("queue"));

    //statistics
    std::vector<std::string> t;
    t.push_back("Active");
    t.push_back("CyclicSleep");
    t.push_back("DeepSleep");
    sRatio = new cStat("Ratio",t);
    t.clear();
    t.push_back("Average");
    t.push_back("Sleep");
    sPower = new cStat("Power",t);
    t.clear();
    t.push_back("Average");
    t.push_back("EF");
    t.push_back("AF");
    t.push_back("BE");
    sAdjust = new cStat("AdjustUp",t);
    sFwdCycle = new cStat("FwdCycleUp",t);

    //initial
    for(int i = intpar("EF");i < intpar("FramePriorities");++i) forward(i);

    //debug
    if(getLLID() == 0) std::cerr << getEnergy(0.002) << "|" << (0.002* dblpar("pActive")) << endl;
    if(getLLID() == 0) std::cerr << getEnergy(0.002297457287) << "|" << (0.002297457287 * dblpar("pActive")) << endl;
}

void testIntraScheduler::finish() {
    Scheduler::finish();

    if(getLLID() == intpar("debug")) outputLog();
    std::cerr << "[ONU" << getLLID() << "::finish]" << endl;

    simtime_t tLast = simTime() - tCheckpoint;
    if(state == ACTIVE) tActive.push_back(tLast);
    else if(tLast < tAvgCycle) tCSleep.push_back(tLast);
    else tDSleep.push_back(tLast);

    double energy = 0;
    simtime_t tActiveSum = 0,tCSleepSum = 0,tDSleepSum = 0;
    for(int i = 0;i < int(tCSleep.size());++i)
    {
        if(getEnergy(tCSleep[i]) > tCSleep[i].dbl() * dblpar("pActive"))
        {
            std::cerr << "ONU" << getLLID() << " DS" << i << "sleepTime=" << tCSleep[i] << " " << getEnergy(tCSleep[i])  << ">" <<  (tCSleep[i].dbl() * dblpar("pActive")) << endl;
        }
        energy += getEnergy(tCSleep[i]);
        tCSleepSum += tCSleep[i];
    }
    for(int i = 0;i < int(tDSleep.size());++i)
    {
        if(getEnergy(tDSleep[i]) > tDSleep[i].dbl() * dblpar("pActive"))
        {
            std::cerr << "ONU" << getLLID() << " DS" << i << "sleepTime=" << tDSleep[i] << " " << getEnergy(tDSleep[i])  << ">" <<  (tDSleep[i].dbl() * dblpar("pActive")) << endl;
        }
        energy += getEnergy(tDSleep[i]);
        tCSleepSum += tDSleep[i];
    }
    sPower->collect(1,energy / (tCSleepSum + tDSleepSum));
    for(int i = 0;i < int(tActive.size());++i)
    {
        energy += tActive[i].dbl() * dblpar("pActive");
        tActiveSum += tActive[i];
    }

    sPower->collect(0,energy / simTime());
    sRatio->collect(0,tActiveSum / simTime());
    sRatio->collect(1,tCSleepSum / simTime());
    sRatio->collect(2,tDSleepSum / simTime());

    //debug
    std::cerr << "\tSimulation Time=" << simTime() << "\tActive Time=" << tActiveSum << "\tSleep Time=" << (tCSleepSum + tDSleepSum) << endl;
    std::cerr << "\tqueue size=" << queue->getBitLength() << " s1=" << (queue->getBitLength() - queue->getS2Size()) << " s2=" << queue->getS2Size() << endl;
    std::cerr << "\tavgcycle=" << tAvgCycle << endl;
    std::cerr << "\tPR EF=" << rAdjust[intpar("EF")] << " AF=" << rAdjust[intpar("AF")] << " BE=" << rAdjust[intpar("BE")] << endl;
}

void testIntraScheduler::handleSelfMessage(cMessage* msg) {
    simtime_t sleepTime;
    if(msg->getKind() == intpar("WakeUp"))
    {
        if(getLLID() == intpar("debug"))
        {
            log << "[ONU" << getLLID() << "::wakeup] t=" << simTime() << " state=" << state << " sleepStart=" << tCheckpoint << " tWakeup=" << tWakeup << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
            if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::wakeup] t=" << simTime() << " state=" << state << " sleepStart=" << tCheckpoint << " tWakeup=" << tWakeup << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
        }

        state = ACTIVE;
        sleepTime = simTime() - tCheckpoint - timepar("tOverhead");
        //debug
        if(sleepTime < 0) throw cRuntimeError("wakeup with negative sleep time t=%f st=%f diff=%f",simTime().dbl(),tCheckpoint.dbl(),simTime().dbl() - tCheckpoint.dbl());
        if(simTime() > tWakeup) throw cRuntimeError("sleep time more than predetermined t=%f tWakeup=%f st=%f",simTime().dbl(),tWakeup.dbl(),tCheckpoint.dbl());
        tCheckpoint = simTime();

        //calculate energy consumption in sleep period and wake up period
        tActive.push_back(timepar("tOverhead"));
        if(sleepTime < timepar("tCycle")) tCSleep.push_back(sleepTime);
        else tDSleep.push_back(sleepTime);
    }
    else if(msg->getKind() == intpar("StartSend"))
    {
        //check bandwidth
        startTransmission();
    }
    else if(msg->getKind() == intpar("SendMPCP"))
    {
        sendMPCP();
    }
    else if(msg->getKind() == intpar("Forward"))
    {
        forward(int(msg->par("priority")));
    }
    else if(msg->getKind() == intpar("StartSleep"))
    {
        if(getLLID() == intpar("debug"))
        {
            log << "[ONU" << getLLID() << "::CYCLICSLEEP] t=" << simTime() << " sleepTime=" << double(msg->par("sleepLength")) << " tWakeup=" << tWakeup << endl;
            if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::CYCLICSLEEP] t=" << simTime() << " sleepTime=" << double(msg->par("sleepLength")) << " tWakeup=" << tWakeup << endl;
        }
        sleep(double(msg->par("sleepLength")));
    }
    delete msg;
}

void testIntraScheduler::sendMPCP() {
    CSLAReport* report = new CSLAReport("report",intpar("MPCP"));
    report->setBitLength(intpar("MPCPSize"));
    report->setSRC(getLLID());
    report->setDST(getLLID());
    report->setRequest1Length(queue->getS2Size());
    send(report,"txSplitter");

    if(getLLID() == intpar("debug"))
    {
        log << "[ONU" << getLLID() << "::sendReport] t=" << simTime() << " state=" << state << " up=" << queue->getS2Size() << endl;
        if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::sendReport] t=" << simTime() << " state=" << state << " up=" << queue->getS2Size() << endl;
    }
}

void testIntraScheduler::startTransmission() {
    if(simTime() >= tStart)
    {
        //check bandwidth
        while(queue->s1front(intpar("EF")) != NULL && (tWindow - getTransmissionTime(queue->getS2Size())) >= getTransmissionTime(queue->s1front(intpar("EF"))->getBitLength())) queue->stepForward(intpar("EF"));
        while(queue->s1front(intpar("AF")) != NULL && (tWindow - getTransmissionTime(queue->getS2Size())) >= getTransmissionTime(queue->s1front(intpar("AF"))->getBitLength())) queue->stepForward(intpar("AF"));
        while(queue->s1front(intpar("BE")) != NULL && (tWindow - getTransmissionTime(queue->getS2Size())) >= getTransmissionTime(queue->s1front(intpar("BE"))->getBitLength())) queue->stepForward(intpar("BE"));
        sendDataUntil(tStart + tWindow);
    }
}

void testIntraScheduler::forward(int priority) {
    //move frame of specific priority to stage-two queue and schedule next forward event
    queue->forward(priority);

    cMessage* msg = new cMessage("Forward",intpar("Forward"));
    cMsgPar* par = new cMsgPar("priority");
    par->setLongValue(priority);
    msg->addPar(par);
    simtime_t t = getFwdCycle(priority);
    tNextFwd[priority] += t;
    if(tNextFwd[priority] < simTime()) throw cRuntimeError("ONU%d schedule forward message to the past, current=%f, schedule=%f, t=%f, priority=%d, pr=%f",getLLID(),simTime().dbl(),tNextFwd[priority].dbl(),t.dbl(),priority,rAdjust[priority]);
    scheduleAt(tNextFwd[priority],msg);

    //wake up if ONU is SLEEP and has upstream frame in stage-two queue
    if(queue->getS2Size(priority) > 0) interrupt();

    //statistics
    sAdjust->collect(0,rAdjust[priority]);
    sAdjust->collect(priority,rAdjust[priority]);
    sFwdCycle->collect(0,t);
    sFwdCycle->collect(priority,t);
}

simtime_t testIntraScheduler::getFwdCycle(int priority) {
    simtime_t t = 0;
    if(priority == intpar("EF"))
    {
        t = 2 * (timepar("tEFDC")/* - getFwdDelay(LLID)*/);
        t *= getR(priority) * (getOfflineMaxWindow() / (getOfflineMaxWindow() + getUpONULambda(intpar("EF")) * tAvgCycle * getUpFirstMoment(intpar("EF"))));
    }
    else if(priority == intpar("AF"))
    {
        t = 2 * (timepar("tAFDC") - getFwdDelay());
        t *= getR(priority) * (getOfflineMaxWindow() / (getOfflineMaxWindow() + getUpONULambda(intpar("AF")) * tAvgCycle * getUpFirstMoment(intpar("AF"))));
    }
    else if(priority == intpar("BE"))
    {
        t = 2 * (timepar("tBEDC") - getFwdDelay());
        t *= getR(priority) * (getOfflineMaxWindow() / (getOfflineMaxWindow() + getUpONULambda(intpar("BE")) * tAvgCycle * getUpFirstMoment(intpar("BE"))));
    }
    tFwdCycle[priority] = t;
    return t;
}

double testIntraScheduler::getR(int priority) {
    double dc = 0,d = 0,r = rAdjust[priority],delta = dblpar("rDelta");
    if(priority == intpar("EF")) {dc = dblpar("tEFDC");d = sdEFDelay->getMean();}
    else if(priority == intpar("AF")) {dc = dblpar("tAFDC");d = sdAFDelay->getMean();}
    else if(priority == intpar("BE")) {dc = dblpar("tBEDC");d = sdBEDelay->getMean();}

    if(d > dc && r > dblpar("rMin")) r -= delta;
    else if(d < dc && r < dblpar("rMax")) r += delta;
    rAdjust[priority] = r;
    return std::max(std::min(r,dblpar("rMax")),dblpar("rMin"));
}

simtime_t testIntraScheduler::getRemainTime() {
    return std::min(std::min(tNextFwd[intpar("EF")],tNextFwd[intpar("AF")]),tNextFwd[intpar("BE")]) - simTime();
}


void testIntraScheduler::handleMessageFromSplitter(cMessage* msg) {
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
            DataFrame* data = check_and_cast<DataFrame*>(msg);
            //debug
            if(getLLID() == intpar("debug"))
            {
                log << "[ONU" << getLLID() << "::getDataFromSplitter] t=" << simTime() << " state=" << state << " kind=" << data->getKind() << " size=" << data->getBitLength() << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
                if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::getDataFromSplitter] t=" << simTime() << " state=" << state << " kind=" << data->getKind() << " size=" << data->getBitLength() << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
            }
            if(state == SLEEP) throw cRuntimeError("receive data from OLT while ONU is sleeping",getLLID());
            send(frame,"txHost");
        }
        else delete frame;
    }
    else delete frame;
}

void testIntraScheduler::handleMPCP(cMessage* msg) {
    CSLAGate* gate = check_and_cast<CSLAGate*>(msg);
    signal = gate->getGrant1TransceiverMode();
    tStart = gate->getGrant1StartTime();
    tAvgCycle = gate->getGrant2Length();
    if(signal == DS) tWindow = 0;
    else if(signal == CS) tWindow = gate->getGrant1Length();
    tWakeup = gate->getGrant2StartTime();

    if(getLLID() == intpar("debug"))
    {
        std::string s = "Gate";
        if(signal != CS) s = "SleepGate";
        log << "[ONU" << getLLID() << "::handle" << s << "] t=" << simTime() << " state=" << state << " s=" << tStart << " w=" << tWindow << " sl=" << (gate->getGrant2StartTime() - (tStart + tWindow)) << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
        if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::handle" << s << "] t=" << simTime() << " state=" << state << " s=" << tStart << " w=" << tWindow << " sl=" << (gate->getGrant2StartTime() - (tStart + tWindow)) << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
    }


    if(signal == DS && queue->getS2Size() == 0 && getRemainTime() > dblpar("remainThreshold") * tAvgCycle)
    //if(signal == DS && queue->getS2Size() == 0)
    //if(signal == DS && queue->getS2Size() == 0 && getRemainTime() > getFwdDelay())
    {
        tWakeup = simTime() + gate->getGrant1Length();
        if(getLLID() == intpar("debug"))
        {
            log << "[ONU" << getLLID() << "::DEEPSLEEP] t=" << simTime() << " sleepTime=" << gate->getGrant1Length() << " tWakeup=" << tWakeup << endl;
            if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::DEEPSLEEP] t=" << simTime() << " sleepTime=" << gate->getGrant1Length() << " tWakeup=" << tWakeup << endl;
        }
        sleep(gate->getGrant1Length());
    }
    else
    {
        //cyclic sleep before transmission
        scheduleToSleep(simTime(),tStart - simTime());

        //schedule upstream transmission
        scheduleAt(tStart,new cMessage("StartSend",intpar("StartSend")));

        //schedule to send report to OLT
        scheduleAt(tStart + tWindow + tAdjust,new cMessage("SendMPCP",intpar("SendMPCP")));

        //cyclic sleep after transmission
        scheduleToSleep(tStart + tWindow + tMPCP,tWakeup - (tStart + tWindow + tMPCP));
    }
    if(tWakeup == 0) throw cRuntimeError("t=%f sig=%d tStart=%f tWindow=%f tWakeup=%f",simTime().dbl(),int(gate->getGrant1TransceiverMode()),gate->getGrant1StartTime().dbl(),gate->getGrant1Length().dbl(),gate->getGrant2StartTime().dbl());
}

void testIntraScheduler::scheduleToSleep(simtime_t tStartSleep,simtime_t tSleepLength) {
    //schedule to sleep
    cMessage* msg = new cMessage("StartSleep",intpar("StartSleep"));
    cMsgPar* par = new cMsgPar("sleepLength");
    par->setDoubleValue(tSleepLength.dbl());
    msg->addPar(par);
    scheduleAt(tStartSleep,msg);
}

void testIntraScheduler::sleep(simtime_t tSleepLength) {
    if(tSleepLength > timepar("tOverhead"))
    {
        //calculate energy
        tActive.push_back(simTime() - tCheckpoint);
        tCheckpoint = simTime();

        //schedule wake up time
        transition = false;
        msgWakeup = new cMessage("WakeUp",intpar("WakeUp"));
        scheduleAt(simTime() + tSleepLength,msgWakeup);
        state = SLEEP;
    }
}

void testIntraScheduler::interrupt() {
    if(state == SLEEP && !transition && simTime() < (msgWakeup->getArrivalTime() - timepar("tOverhead")))
    {
        cancelAndDelete(msgWakeup);
        msgWakeup = new cMessage("WakeUp",intpar("WakeUp"));
        transition = true;
        scheduleAt(simTime() + timepar("tOverhead"),msgWakeup);
    }
}

double testIntraScheduler::getEnergy(simtime_t sleepTime) {
    if(!boolpar("nonUniformSleepPower")) return sleepTime.dbl() * dblpar("pSleep");

    //do integral approximation
    double energy,scale = 50000;
    for(int i = 0;i < sleepTime.dbl() * scale;i++) energy += getPower(double(i) / scale) / scale;
    return energy;
}

double testIntraScheduler::getPower(simtime_t sleepTime) {
    //change the unit of sleepTime to ms
    double t = sleepTime.dbl() * 1000;
    double a = 0.667,b = 14.472,c = 1.131;
    if(t <= 5) return -a * t + dblpar("pActive");
    else return b * pow(t,-c) + dblpar("pSleep");
}

void testIntraScheduler::handleMessageFromHost(cMessage* msg) {
    DataFrame* data = check_and_cast<DataFrame*>(msg);

    //debug
    if(getLLID() == intpar("debug"))
    {
        log << "[ONU" << getLLID() << "::getDataFromHost] t=" << simTime() << " state=" << state << " kind=" << data->getKind() << " size=" << data->getBitLength() << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
        if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::getDataFromHost] t=" << simTime() << " state=" << state << " kind=" << data->getKind() << " size=" << data->getBitLength() << " q=" << queue->getBitLength() << " up=" << queue->getS2Size() << endl;
    }

    queue->insert(data);

    //for constant and linear credit
    //startTransmission();
}

void testIntraScheduler::handleDataBeforeSend(DataFrame* data) {
    int priority = data->getKind();
    /*
    data->setAFwdDelay(getFwdDelay());
    data->setAS1Delay(getFwdCycle(priority) / 2);
    double b = (getUpONULambda(priority) * getFwdCycle(priority) * getUpFirstMoment(priority) * tAvgCycle.dbl()) / getMaxWindow();
    b *= 1 + getPR(priority);
    data->setAS2Delay(getFwdDelay() + b / 2);
    data->setABurstTime(b);
    */

    //update estimated delay of each class
    simtime_t d = simTime() - data->getRxTime();
    if(priority == intpar("EF")) sdEFDelay->collect(d);
    else if(priority == intpar("AF")) sdAFDelay->collect(d);
    else if(priority == intpar("BE")) sdBEDelay->collect(d);

    //if(getLLID() == 0) std::cerr << "ONU0 sendPacket p=" << priority << " d=" << d << endl;

    //debug
    if(getLLID() == intpar("debug"))
    {
        log << "[ONU" << getLLID() << "::sendData] t=" << simTime() << " kind=" << priority << " size=" << data->getBitLength() << " delay=" << (simTime() - data->getRxTime()) << " up=" << queue->getS2Size() << endl;
        if(boolpar("err")) std::cerr << "[ONU" << getLLID() << "::sendData] t=" << simTime() << " kind=" << priority << " size=" << data->getBitLength() << " delay=" << (simTime() - data->getRxTime()) << " up=" << queue->getS2Size() << endl;
    }
}

simtime_t testIntraScheduler::getFwdDelay() {
    return timepar("tOverhead") + 2 * tAvgCycle;
}

cPacket* testIntraScheduler::getNextDataFrame() {
    return queue->s2front();
}

cPacket* testIntraScheduler::popNextDataFrame() {
    return queue->popS2();
}

bool testIntraScheduler::queueEmpty() {
    return queue->isEmpty();
}

void testIntraScheduler::test() {

}

} /* namespace epon */
