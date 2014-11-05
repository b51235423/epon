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

package epon.GBA;
import epon.EPONModule;
import epon.GBA.GBAIntraScheduler;
import epon.Queue.Queue;

module ONU extends EPONModule
{
    parameters:
        double pActive @unit(W) = default(0);
        double pDoze @unit(W) = default(0);
        double pSleep @unit(W) = default(0);
        double tOverhead @unit(s) = default(0);
        double tDozeOverhead @unit(s) = default(0);
    submodules:
        scheduler: GBAIntraScheduler {}
        queue: Queue;
    connections:
        //connections to OLT
        rxSplitter --> scheduler.rxSplitter;
        scheduler.txSplitter --> txSplitter;

        //connections to user
        rxHost --> scheduler.rxHost;
        scheduler.txHost --> txHost;
}
