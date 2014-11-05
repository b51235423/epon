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
import epon.EPONModule;
import epon.Host.*;
import epon.GBA.OLT;
import epon.GBA.ONU;
import epon.Splitter;
import epon.Recorder;

network GBA extends TDMPON
{
    parameters:
        //paremeters
        int cPON @unit(bps) = default(0);

        int cEth @unit(bps) = default(0);

        int numONUs = default(0);

        double tCycle @unit(s) = default(0);

        int interScheduling = default(0);

        double tGuard @unit(s) = default(0);

        double tProp @unit(s) = default(0);

        double r = default(0);

        bool multicast = default(true);

        int maxFrameSize @unit(b) = default(0);

        int minFrameSize @unit(b) = default(0);

        int MPCPSize @unit(b) = default(0);

        int bufferSize @unit(b) = default(0);

        //GBA parameters
        double D @unit(s) = default(0);
        double tGBA @unit(s) = default(0);

        double pDoze @unit(W) = default(0);
        double tDozeOverhead @unit(s) = default(0);

        double pActive1C @unit(W) = default(0);
        double pSleep1C @unit(W) = default(0);
        double tOverhead1C @unit(s) = default(0);

        double pActive1A @unit(W) = default(0);
        double pSleep1A @unit(W) = default(0);
        double tOverhead1A @unit(s) = default(0);

        double pActive2 @unit(W) = default(0);
        double pSleep2 @unit(W) = default(0);
        double tOverhead2 @unit(s) = default(0);

        double pActive3 @unit(W) = default(0);
        double pSleep3 @unit(W) = default(0);
        double tOverhead3 @unit(s) = default(0);

		//load distribution
        bool unbalancedLoad = default(false);
        int heavyUsers = default(0);
        double heavyRatio = default(0);

        double serviceLoad = default(0);
        double serviceEFWeight = default(0);
        double serviceAFWeight = default(0);
        double serviceBEWeight = default(0);

        double userTotalLoad = default(0);
        double userEFWeight = default(0);
        double userAFWeight = default(0);
        double userBEWeight = default(0);

        //set submodule parameters
        coupler.numONUs = numONUs;
        splitter.numONUs = numONUs;

        @display("bgb=460,460");
    types:
        channel FiberBetweenOLTAndSplitter extends ned.DatarateChannel
        {
            delay = tProp * r;
            datarate = cPON;
        }
        channel FiberBetweenONUAndSplitter extends ned.DatarateChannel
        {
            delay = tProp * (1 - r);
            datarate = cPON;
        }
        channel SNICable extends ned.DatarateChannel
        {
            delay = 0;
            datarate = cEth;
        }
        channel UNICable extends ned.DatarateChannel
        {
            delay = 0;
            datarate = cEth;
        }
    submodules:
        service[numONUs]: Host {
            LLID = index;
            hostDatarate = 1Gbps;
            sourceNumber = 256 / numONUs;
            load = serviceLoad / numONUs;
            EFWeight = serviceEFWeight;
            AFWeight = serviceAFWeight;
            BEWeight = serviceBEWeight;
            @display("p=30,30");
        }
        coupler: Splitter {
            @display("p=110,110");
        }
        olt: OLT {
            multiqueue.size = bufferSize * numONUs;
            @display("p=190,190");
        }
        splitter: Splitter {
            @display("p=270,270");
        }
        onu[numONUs]: ONU {
            queue.size = bufferSize;
            @display("p=350,350");
        }
        user[numONUs]: Host {
            LLID = index;
            hostDatarate = 1Gbps;
            sourceNumber = 256 / numONUs;
            load = userTotalLoad / numONUs;
            EFWeight = userEFWeight;
            AFWeight = userAFWeight;
            BEWeight = userBEWeight;
            @display("p=430,430");
        }
        recorder: Recorder {
            @display("p=430,30");
        }
    connections:
        //service host connection
        for i=0..numONUs-1 {
            service[i].txSplitter --> SNICable --> coupler.rxONU[i];
            coupler.txONU[i] --> service[i].rxSplitter;
        }

        //connection from coupler to olt
        coupler.txOLT --> olt.rxHost;
        olt.txHost --> coupler.rxOLT;

        //channels connection from OLT to Splitter
        olt.txSplitter --> FiberBetweenOLTAndSplitter --> splitter.rxOLT;
        splitter.txOLT --> FiberBetweenOLTAndSplitter --> olt.rxSplitter;

        //channels connection from ONU to Splitter
        for i=0..numONUs-1 {
            onu[i].txSplitter --> FiberBetweenONUAndSplitter --> splitter.rxONU[i];
            splitter.txONU[i] --> FiberBetweenONUAndSplitter --> onu[i].rxSplitter;
        }

        //user host connection
        for i=0..numONUs-1 {
            user[i].txSplitter --> UNICable --> onu[i].rxHost;
            onu[i].txHost --> user[i].rxSplitter;
        }
}
