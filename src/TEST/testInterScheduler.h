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

#ifndef TESTINTERSCHEDULER_H_
#define TESTINTERSCHEDULER_H_
#include <Scheduler.h>
#include <MultiTwoStageQueue.h>

namespace epon {

class testInterScheduler : public Scheduler {
public:
    testInterScheduler();
    virtual ~testInterScheduler();

    virtual void initialize();
    virtual void finish();

    virtual void handleSelfMessage(cMessage* msg);
    virtual void sendMPCP(int LLID);
    virtual void startTransmission(int LLID);

    virtual void forward(int LLID,int priority);
    virtual simtime_t getFwdCycle(int LLID,int priority);
    virtual double getR(int LLID,int priority);
    virtual simtime_t getRemainTime(int LLID);

    virtual void handleMessageFromSplitter(cMessage* msg);
    virtual void handleMPCP(cMessage* msg);
    virtual void handleMessageFromHost(cMessage* msg);
    virtual void handleDataBeforeSend(DataFrame* data);

    virtual void bandwidthAlloc(std::vector<int> upRequiredBandwidth);

    virtual simtime_t getFwdDelay(int LLID);
    virtual cPacket* getNextDataFrame();
    virtual cPacket* popNextDataFrame();
    virtual bool queueEmpty();

    //debug
    void test();
    void showPollingTable();

    //enum
    //enum InterSchduling{FIXED,GATED,LIMITED,CONSTANT,LINEAR};
    enum SystemState {CYCLICSLEEP,DEEPSLEEP};

    //const
    const static double tProcess = 0.000000000001;
protected:
    //parameters
    int currentONU;
    simtime_t tAvailable,tOLTCycleStart;
    std::vector<bool> getMPCP;
    std::vector<int> bRequired,scheduled,state,times;
    std::vector<simtime_t> tRequired,tStart,tWindow,tWakeup;
    std::vector<simtime_t> tEFDC,tAFDC,tBEDC,tOverhead;
    std::vector< std::vector<simtime_t> > tNextFwd,tFwdCycle;
    std::vector< std::vector<double> > rAdjust;
    cStdDev* sdCycle;
    std::vector<cStdDev*> sdEFDelay,sdAFDelay,sdBEDelay;

    //components
    MultiTwoStageQueue* queue;

    //statistics
    cStat* sCycle;
    cStat* sAdjust;
    cStat* sFwdCycle;

    //debug
};

} /* namespace epon */
#endif /* testInterScheduler_H_ */
