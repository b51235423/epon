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

#include "Source.h"

namespace epon {
Define_Module(Source);

Source::Source() {

}

Source::~Source() {

}

void Source::initialize() {
    //parameters
    load = dblpar("load");
    bits = 0;

    //unbalanced load
    if(boolpar("unbalancedLoad"))
    {
        if(intpar("LLID") < intpar("heavyUsers")) load *= intpar("numONUs") * dblpar("heavyRatio") / intpar("heavyUsers");
        else load *= intpar("numONUs") * (1 - dblpar("heavyRatio")) / (intpar("numONUs") - intpar("heavyUsers"));
    }
    //if(strcmp("user",getParentModule()->getName()) == 0) std::cerr << getParentModule()->getName() << getParentModule()->getIndex() << " load=" << load << endl;
    //if(strcmp("service",getParentModule()->getName()) == 0) std::cerr << getParentModule()->getName() << getParentModule()->getIndex() << " load=" << load << endl;

    //components
    txOut = check_and_cast<cDatarateChannel*>(gate("out")->getChannel());
    if(load > 0)
    {
        //wait off periods
        /*
        double d = (getOffLength() * getOffPacketBitLength()) / txOut->getDatarate();
        std::cerr << "getOffLength()=" << getOffLength()
                << " getOffPacketBitLength()=" << getOffPacketBitLength()
                << " txOut->getDatarate()=" << txOut->getDatarate()
                << " d=" << d << endl;
                */
        simtime_t t = (getOffLength() * getOffPacketBitLength()) / txOut->getDatarate();
        scheduleAt(t,new cMessage("SendPacket"));
    }

    //debug
}

void Source::handleMessage(cMessage* msg) {
    //send on period packets
    if(msg->isSelfMessage()) sendPackets();
    delete msg;
}

void Source::finish() {
    //debug
    //std::cerr << getParentModule()->getName() << getParentModule()->getIndex() << "::" << getName() << getIndex() << " bits=" << bits << " load=" << load << endl;
}

void Source::sendPackets() {
    int numPackets = getOnLength();
    for(int i = 0;i < numPackets;i++)
    {
        DataFrame* frame = new DataFrame("data");
        int length = getOnPacketBitLength();
        frame->setBitLength(length);
        bits += length;

        //
        if(intpar("Priority") >= 0 && intpar("Priority") < intpar("FramePriorities")) frame->setKind(intpar("Priority"));
        else throw cRuntimeError("[Source::sendPackets] wrong Priority value(Priority=%d)",intpar("Priority"));

        if(intpar("LLID") == -1) throw cRuntimeError("[Source::sendPackets] wrong LLID.");
        else
        {
            frame->setSRC(intpar("LLID"));
            frame->setDST(intpar("LLID"));
        }
        simtime_t tScheduled = txOut->getTransmissionFinishTime();
        if(tScheduled < simTime()) tScheduled = simTime();
        sendDelayed(frame,tScheduled - simTime(),"out");
    }

    //wait off periods
    simtime_t t = txOut->getTransmissionFinishTime();
    if(t < simTime()) t = simTime();
    t += (getOffLength() * getOffPacketBitLength()) / txOut->getDatarate();
    scheduleAt(t, new cMessage("SendPacket"));
}

int64_t Source::getOnLength() {
    return 0;
}

int64_t Source::getOffLength() {
    return 0;
}

int64_t Source::getOnPacketBitLength() {
    return 0;
}

int64_t Source::getOffPacketBitLength() {
    return 0;
}

} /* namespace epon */
