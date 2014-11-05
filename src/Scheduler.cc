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

#include "Scheduler.h"

namespace epon {
Define_Module(Scheduler);

Scheduler::Scheduler() {

}

Scheduler::~Scheduler() {
    delete sdPacketSize;
}

void Scheduler::initialize() {
    //parameters
    tGuard = timepar("tGuard");
    RTT = 2 * timepar("tProp");

    //components
    //queue component is not initialized!
    txSplitter = check_and_cast<cDatarateChannel*>(getParentModule()->gate("txSplitter")->getChannel());
    tMPCP = getTransmissionTime(intpar("MPCPSize"));

    //statistics
    sdPacketSize = new cStdDev("packetSize");
}

void Scheduler::finish() {
    recordScalar("sent bits",sdPacketSize->getSum(),"b");
    recordScalar("frame departure rate",sdPacketSize->getCount() / simTime().dbl(),"");
}

void Scheduler::handleMessage(cMessage* msg) {
    //handle message according to where it is from
    if(msg->isSelfMessage())
    {
        //send data event and self event
        if(msg->getKind() == intpar("SendUntil"))
        {
            sendDataUntil((simtime_t)msg->par("tUntil"));
            delete msg;
        }
        else handleSelfMessage(msg);
    }
    else if(msg->arrivedOn("rxSplitter"))
    {
        handleMessageFromSplitter(msg);
    }
    else if(msg->arrivedOn("rxHost"))
    {
        //add reveice timestamp to incoming packet from host
        DataFrame* data = check_and_cast<DataFrame*>(msg);
        setRxTimeStamp(data);

        //handle packet
        handleMessageFromHost(msg);
    }
    else delete msg;
}

void Scheduler::handleSelfMessage(cMessage* msg) {

}

void Scheduler::handleMessageFromSplitter(cMessage* msg) {

}

void Scheduler::handleMessageFromHost(cMessage* msg) {

}

void Scheduler::send(cMessage* msg,const char *gatename) {
    cSimpleModule::send(msg,gatename);
}

void Scheduler::sendDataUntil(simtime_t tUntil) {
    simtime_t tRemain = tUntil - getTxSplitterSchedTime();
    if(getNextDataFrame() != NULL && tRemain >= getTransmissionTime(getNextDataFrame()->getBitLength()))
    {
        //tx to splitter is not busy
        if(getTxSplitterSchedTime() == simTime())
        {
            DataFrame* data = check_and_cast<DataFrame*>(popNextDataFrame());
            setTxTimeStamp(data,simTime());
            sdPacketSize->collect(data->getBitLength());
            handleDataBeforeSend(data);
            send(data,"txSplitter");
        }

        //schedule next transmission if queue is not empty
        //schedule to send when tx is not busy
        msgSendUntil = new cMessage("msgSendUntil",intpar("SendUntil"));
        cMsgPar* par = new cMsgPar("tUntil");
        par->setDoubleValue(tUntil.dbl());
        msgSendUntil->addPar(par);
        scheduleAt(getTxSplitterSchedTime(),msgSendUntil);
    }
}

void Scheduler::handleDataBeforeSend(DataFrame* data)
{

}

//query
simtime_t Scheduler::getTransmissionTime(int bitLength) {
    simtime_t t = (1.0 * bitLength) / txSplitter->getDatarate();
    return t;
}

bool Scheduler::isTxSplitterBusy() {
    return txSplitter->isBusy();
}

simtime_t Scheduler::getTxSplitterSchedTime() {
    simtime_t t = txSplitter->getTransmissionFinishTime();
    return std::max(t,simTime());
}

int Scheduler::getLLID() {
    return intpar("LLID");
}

cPacket* Scheduler::getNextDataFrame() {
    throw cRuntimeError("getNextDataFrame()");
}

cPacket* Scheduler::popNextDataFrame() {
    throw cRuntimeError("popNextDataFrame()");
}

bool Scheduler::queueEmpty() {
    throw cRuntimeError("queueEmpty()");
}

//set
void Scheduler::setRxTimeStamp(DataFrame* data) {
    data->setRxTime(simTime());
}

void Scheduler::setTxTimeStamp(DataFrame* data,simtime_t txTimeStamp) {
    data->setTxTime(txTimeStamp);
}

} /* namespace epon */
