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

#ifndef TWOSTAGEQUEUE_H_
#define TWOSTAGEQUEUE_H_
#include <cTDMPON.h>

namespace epon {

class TwoStageQueue : public cTDMPON {
public:
    TwoStageQueue();
    virtual ~TwoStageQueue();

    void initialize();
    void handleMessage(cMessage* msg);

    //forward functions
    void setCycle(simtime_t t);
    void setDC(int priority,simtime_t t);
    void forward(int priority);
    bool stepForward(int priority);
    virtual simtime_t getFwdCycle(int priority);
    virtual simtime_t getRemainTime();

    //
    int getS2Length();
    int getS2Length(int priority);
    int getS2Size();
    int getS2Size(int priority);

    //insertion and removal functions
    void insert(cPacket *packet);
    cPacket* remove(cPacket *packet);
    cPacket* pop();
    cPacket* pop(int priority);
    cPacket* popS2();
    cPacket* popS2(int priority);

    //query functions
    bool isEmpty();
    bool isEmpty(int priority);
    int getLength();
    int getLength(int priority);
    int64_t getBitLength();
    int64_t getBitLength(int priority);
    cPacket* front();
    cPacket* front(int priority);
    cPacket* s2front();
    cPacket* s1front();
    cPacket* s1front(int priority);
    cPacket* back();
    cPacket* s2back();

    //debug
    void priorityCheck(int priority);
    bool testFwd;
protected:
    //parameters
    int p;
    int* s2Length;
    int* s2Size;

    //forward
    simtime_t tCycle;
    std::vector<simtime_t> tDC;
    std::vector<simtime_t> tNextFwd;
    std::vector<double> rAdjust;
    std::vector<cStdDev*> sdDelay;

    //components
    std::vector<cPacketQueue*> queue;
};

} /* namespace epon */
#endif /* TWOSTAGEQUEUE_H_ */
