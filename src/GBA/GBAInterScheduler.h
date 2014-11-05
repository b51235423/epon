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

#ifndef GBAINTERSCHEDULER_H_
#define GBAINTERSCHEDULER_H_
#include <Scheduler.h>
#include <MultiQueue.h>
#include <Recorder.h>

namespace epon {

class GBAInterScheduler : public Scheduler {
public:
    GBAInterScheduler();
    virtual ~GBAInterScheduler();

    virtual void initialize();
    virtual void finish();

    virtual void handleSelfMessage(cMessage* msg);
    virtual void sendMPCP(int LLID);
    virtual void handleMessageFromSplitter(cMessage* msg);
    virtual void handleMPCP(cMessage* msg);
    virtual void handleMessageFromHost(cMessage* msg);

    virtual void scheduleDownstream();

    virtual void bandwidthAlloc(std::vector<int> upRequiredBitlength);
    virtual void sortAndShift();
    virtual void sort(std::vector<int>* index,std::vector<simtime_t>* value);

    virtual bool queueEmpty();
    virtual cPacket* getNextDataFrame();

    virtual void test();
protected:
    //parameters
    int currentONU,reported;
    simtime_t tMPCP,tGuard,RTT,tAvgFrame,tCycleStart;
    std::vector<int> required;
    std::vector<simtime_t> tSendGate,tStart,tWindow,tRequired;

    //GBA parameters
    //TS1:OLT sleep + OLT sleep overhead
    //TS2:ONU sleep + ONU sleep overhead
    simtime_t tGBA;
    std::vector<simtime_t> D,TS1,TS2;

    //components
    MultiQueue* queue;

    //statistics
    cStdDev* sdCycleTime;
    Recorder* r;

    //test
    std::vector<int> sorted;

    //const
    const static int msgkindSendGate = 0;
    //inter scheduling
    enum InterSchduling{GATED,LIMITED};

    //statistics
    cStdDev* cycleTimeRecord;
};

} /* namespace epon */
#endif /* GBAINTERSCHEDULER_H_ */
