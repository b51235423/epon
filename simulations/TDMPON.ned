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

package epon.simulations;
import epon.*;

network TDMPON
{
	parameters:
	    //scheduler events
	    int SendUntil = default(0);
	    int SendMPCP = default(1);
	    int CheckMPCP = default(2);
	    int WakeUp = default(3);
	    int StartSend = default(4);
	    int StartSleep = default(5);
	    int Forward = default(6);
	    int Interrupt = default(7);
	    
	    //frame priorities including MPCP
	    int FramePriorities = default(4);
	    int MPCP = default(0);
		int EF = default(1);
		int AF = default(2);
		int BE = default(3);
		int SLEEP = default(4);
	submodules:
        //recorder: Recorder {
        //    @display("p=430,30");
        //}
}