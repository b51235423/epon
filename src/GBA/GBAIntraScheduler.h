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

#ifndef GBAINTRASCHEDULER_H_
#define GBAINTRASCHEDULER_H_
#include <Scheduler.h>
#include <Queue.h>
#include <Recorder.h>

namespace epon {

class GBAIntraScheduler : public Scheduler {
public:
    GBAIntraScheduler();
    virtual ~GBAIntraScheduler();

    virtual void initialize();
    virtual void finish();

    virtual void handleSelfMessage(cMessage* msg);
    virtual void sendMPCP();

    virtual void handleMessageFromSplitter(cMessage* msg);
    virtual void handleMPCP(cMessage* msg);
    virtual void handleDataFromSplitter(Frame* data);

    virtual void handleMessageFromHost(cMessage* msg);
    virtual void handleDataFromHost(Frame* data);

    //query
    int getLLID();

    //intra scheduling
    //enum IntraSchduling{STRICT,NONSTRICT};

protected:
    //parameter
    simtime_t tMPCP;
    simtime_t tStart,tWindow,tONUSleep;

    //components
    Queue* queue;

    //statistics
    simtime_t tActive,tDoze,tSleep;
};

} /* namespace epon */
#endif /* GBAINTRASCHEDULER_H_ */
