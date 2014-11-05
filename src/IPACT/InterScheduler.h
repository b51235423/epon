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

#ifndef INTERSCHEDULER_H_
#define INTERSCHEDULER_H_
#include <Scheduler.h>
#include <MultiQueue.h>
#include <Recorder.h>

namespace epon {

class InterScheduler : public Scheduler {
public:
    InterScheduler();
    virtual ~InterScheduler();

    virtual void initialize();
    virtual void finish();

    virtual void handleSelfMessage(cMessage* msg);
    virtual void sendMPCP(int LLID);
    virtual void scheduleDownstream();
    virtual void handleMessageFromSplitter(cMessage* msg);
    virtual void handleMPCP(cMessage* msg);
    virtual void handleMessageFromHost(cMessage* msg);

    virtual void bandwidthAlloc(int LLID,int upRequiredBitlength,int downRequiredBitlength);

    virtual bool queueEmpty();
    virtual cPacket* getNextDataFrame();
    virtual cPacket* getNextDataFrameFIFO();
    virtual cPacket* getNextDataFrameTDM();

    //const
    const static double tProcess = 0.000000000001;

    //enum
    enum InterSchduling{FIXED,GATED,LIMITED,CONSTANT,LINEAR,ELASTIC};
    enum DownstreamScheduling{FIFO,TDM};

protected:
    //parameters
    //tScheduled: Rx from splitter scheduled time
    simtime_t tAvailable,tMPCP,tCycleStart,tCycleRemain;
    std::vector<simtime_t> tStart,tWindow,tRequired,RTT;
    std::vector<int> scheduled;
    int currentONU;

    //components
    MultiQueue* queue;

    //statistics
    cStdDev* sdCycleTime;
};

} /* namespace epon */
#endif /* INTERSCHEDULER_H_ */
