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

#ifndef TESTINTRASCHEDULER_H_
#define TESTINTRASCHEDULER_H_
#include <Scheduler.h>
#include <TwoStageQueue.h>

namespace epon {

class testIntraScheduler : public Scheduler {
public:
    testIntraScheduler();
    virtual ~testIntraScheduler();

    virtual void initialize();
    virtual void finish();

    virtual void handleSelfMessage(cMessage* msg);
    virtual void sendMPCP();
    virtual void startTransmission();

    virtual void forward(int priority);
    //virtual simtime_t getNextFwdTime(int priority);
    virtual simtime_t getFwdCycle(int priority);
    virtual double getR(int priority);
    virtual simtime_t getRemainTime();
    virtual void handleMessageFromSplitter(cMessage* msg);
    virtual void handleMPCP(cMessage* msg);
    virtual void scheduleToSleep(simtime_t tStartSleep,simtime_t tSleepLength);
    virtual void sleep(simtime_t tSleepLength);
    virtual void interrupt();
    virtual double getEnergy(simtime_t sleepTime);
    virtual double getPower(simtime_t sleepTime);
    virtual void handleMessageFromHost(cMessage* msg);
    virtual void handleDataBeforeSend(DataFrame* data);

    virtual simtime_t getFwdDelay();
    virtual cPacket* getNextDataFrame();
    virtual cPacket* popNextDataFrame();
    virtual bool queueEmpty();

    //debug
    void test();

    //const
    //The time from the OLT send MPCP to the OLT receive MPCP from ONU should be 0.000201024 (2 * RTT + 2 * tMPCP).
    //However the real time is 0.000201023999, so here an adjust time is added in ONU.
    const static double tAdjust = 0.000000000001;
protected:
    //parameter
    bool transition;
    int state,signal;
    simtime_t tStart,tWindow,tWakeup,tCheckpoint,tAvgCycle;
    std::vector<simtime_t> tNextFwd,tFwdCycle;
    std::vector<double> rAdjust;
    cMessage* msgWakeup;
    cStdDev* sdEFDelay;
    cStdDev* sdAFDelay;
    cStdDev* sdBEDelay;

    //components
    TwoStageQueue* queue;

    //statistics
    std::vector<simtime_t> tActive,tCSleep,tDSleep;
    //cStat* sPeriod;
    //cStat* sEnergy;
    cStat* sPower;
    cStat* sRatio;
    cStat* sAdjust;
    cStat* sFwdCycle;

    //const
    enum SystemState {ACTIVE,SLEEP};
};

} /* namespace epon */
#endif /* testIntraScheduler_H_ */
