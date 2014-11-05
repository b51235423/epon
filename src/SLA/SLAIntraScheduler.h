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

#ifndef SLAINTRASCHEDULER_H_
#define SLAINTRASCHEDULER_H_
#include <Scheduler.h>
#include <Queue.h>

namespace epon {

class SLAIntraScheduler : public Scheduler {
public:
    SLAIntraScheduler();
    virtual ~SLAIntraScheduler();

    virtual void initialize();
    virtual void finish();

    virtual void updateState(int signal);
    //new method
    virtual void updateStateTest(int signal);
    virtual simtime_t getIdleTime();

    virtual void handleSelfMessage(cMessage* msg);
    virtual void sendMPCP();
    virtual void handleMessageFromSplitter(cMessage* msg);
    virtual void handleMPCP(cMessage* msg);
    virtual double getEnergy(simtime_t sleepTime);
    virtual double getPower(simtime_t sleepTime);
    virtual void handleMessageFromHost(cMessage* msg);
    virtual void handleDataBeforeSend(DataFrame* data);

    virtual cPacket* getNextDataFrame();
    virtual cPacket* popNextDataFrame();
    virtual bool queueEmpty();

    //debug
    void test();

    //intra scheduling
    //enum IntraSchduling{STRICT,NONSTRICT};
protected:
    //parameter
    int signal;
    simtime_t tStart,tWindow;

    //SLA parameter
    bool waken;
    int state;
    simtime_t tCheckpoint,tSim,tIdleStartInitial,tIdleStart;
    cMessage* msgWakeup;

    //components
    Queue* queue;

    //statistics
    std::vector<simtime_t> tActive;
    std::vector<simtime_t> tSleep;
    cStdDev* sdCycleTime;
    cStdDev* sdControl;
    cStdDev* sdWindow;
    cStdDev* sdSleepTime;
    cStdDev* sdSleepOverhead;
    cStdDev* sdBurstSize;
    cStdDev* sdBurstLength;

    //const
    enum SleepState {ACTIVE,IDLE,SLEEP};

    //debug
    static const int debug = -1;
};

} /* namespace epon */
#endif /* SLAINTRASCHEDULER_H_ */
