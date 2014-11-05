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

#ifndef SLAINTERSCHEDULER_H_
#define SLAINTERSCHEDULER_H_
#include <Scheduler.h>
#include <MultiQueue.h>
#include <math.h>

namespace epon {

class SLAInterScheduler : public Scheduler {
public:
    SLAInterScheduler();
    virtual ~SLAInterScheduler();

    virtual void initialize();
    virtual void finish();

    virtual void updateState(int LLID,int signal);
    //new method
    virtual void updateStateTest(int LLID,int signal);
    virtual simtime_t getIdleTime(int LLID);

    virtual void handleSelfMessage(cMessage* msg);
    virtual void sendMPCP(int LLID);
    virtual void scheduleDownstream();
    virtual void handleMessageFromSplitter(cMessage* msg);
    virtual void handleMPCP(cMessage* msg);
    virtual void handleMessageFromHost(cMessage* msg);
    virtual void handleDataBeforeSend(DataFrame* data);

    virtual void bandwidthAlloc(int LLID,int upRequiredBitlength,int downRequiredBitlength);

    virtual cPacket* getNextDataFrame();
    virtual cPacket* popNextDataFrame();
    virtual bool queueEmpty();

    //debug
    void test();
    void showPollingTable();

    //const
    const static double tProcess = 0.000000000001;

    //enum
    enum InterSchduling{FIXED,GATED,LIMITED,CONSTANT,LINEAR};
    enum DownstreamScheduling{UPSTREAMCENTRIC};
    enum SleepState {ACTIVE,IDLE,SLEEP};
protected:
    //parameters
    int currentONU;
    simtime_t tAvailable,tPollingCycleStart;
    std::vector<simtime_t> tStart,tWindow,tRequired,tCycleStart;
    std::vector<int> scheduled;

    //SLA parameters
    simtime_t tSleep,tIdleStartInitial;
    std::vector<simtime_t> tIdleStart,tRemain;
    int mSleepInitial;
    std::vector<int> mSleep,state,times;
    std::vector<bool> flagHandleReport;

    //components
    MultiQueue* queue;

    //statistics
    std::vector<cStdDev*> sdCycleTime;
    cStdDev* sdPollingCycleTime;

    //debug
    static const int debug = -1;
};

} /* namespace epon */
#endif /* SLAINTERSCHEDULER_H_ */
