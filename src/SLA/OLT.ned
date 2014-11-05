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

package epon.SLA;
import epon.EPONModule;
import epon.Queue.MultiQueue;
import epon.Queue.Queue;

module OLT extends EPONModule
{
    parameters:
        int LLID = default(0);
        scheduler.LLID = LLID;
    submodules:
        multiqueue: MultiQueue;
        queue: Queue;
        scheduler: SLAInterScheduler;
    connections:
        //connections to OLT
        rxSplitter --> scheduler.rxSplitter;
        scheduler.txSplitter --> txSplitter;

        //connections to user
        rxHost --> scheduler.rxHost;
        scheduler.txHost --> txHost;
}
