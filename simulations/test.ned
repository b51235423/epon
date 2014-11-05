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
import epon.Host.*;
import epon.EPONModule;
import epon.Splitter;
import epon.Recorder;

network test extends TDMPON
{
    parameters:
        //debug
        bool err = default(false);

        //proposed method parameters
        double tEFDC @unit(s) = default(0);
        double tAFDC @unit(s) = default(0);
        double tBEDC @unit(s) = default(0);
        double remainThreshold = default(0);
        double pActive @unit(W) = default(0);
        double pSleep @unit(W) = default(0);
        double tOverhead @unit(s) = default(0);
        bool nonUniformSleepPower = default(true);
        double tSleep @unit(s) = default(0);
        double rDelta = default(0);
        double rMax = default(0);
        double rMin = default(0);

		//parameters
        int cPON @unit(bps) = default(0);

        int cEth @unit(bps) = default(0);

        int numONUs = default(0);

        double credit = default(0);

        double tCycle @unit(s) = default(0);

        double tGuard @unit(s) = default(0);

        double tProp @unit(s) = default(0);
        
        double tDBA @unit(s) = default(0);

        double r = default(0);

        bool multicast = default(true);

        int MPCPSize @unit(b) = default(0);

        int bufferSize @unit(b) = default(0);

        //load distribution
        bool unbalancedLoad = default(false);
        int heavyUsers = default(0);
        double heavyRatio = default(0);

        double downLoad = default(0);
        double downEFWeight = default(0);
        double downEFMax @unit(b) = default(0);
        double downEFMin @unit(b) = default(0);
        double downAFWeight = default(0);
        double downAFMax @unit(b) = default(0);
        double downAFMin @unit(b) = default(0);
        double downBEWeight = default(0);
        double downBEMax @unit(b) = default(0);
        double downBEMin @unit(b) = default(0);

        double upLoad = default(0);
        double upEFWeight = default(0);
        double upEFMax @unit(b) = default(0);
        double upEFMin @unit(b) = default(0);
        double upAFWeight = default(0);
        double upAFMax @unit(b) = default(0);
        double upAFMin @unit(b) = default(0);
        double upBEWeight = default(0);
        double upBEMax @unit(b) = default(0);
        double upBEMin @unit(b) = default(0);

        //set submodule parameters
        oltHostSplitter.numONUs = numONUs;
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
            hostDatarate = cEth;
            sourceNumber = 64;
            load = downLoad / numONUs;
            EFWeight = downEFWeight;
            EFMaxSize = downEFMax;
            EFMinSize = downEFMin;
            AFWeight = downAFWeight;
            AFMaxSize = downAFMax;
            AFMinSize = downAFMin;
            BEWeight = downBEWeight;
            BEMaxSize = downBEMax;
            BEMinSize = downBEMin;
            @display("p=30,30");
        }
        oltHostSplitter: Splitter {
            @display("p=110,110");

        }
        d: dummy {
            inputs = 1;
            outputs = numONUs;
            @display("p=110,110");
        }
        oltHostCombiner: Dispatcher {
            @display("p=110,110");
            inputs = numONUs;
        }
        olt: OLT {
            LLID = numONUs;
            @display("p=190,190");
        }
        splitter: Splitter {
            @display("p=270,270");
        }
        onu[numONUs]: ONU {
            LLID = index;
            @display("p=350,350");
        }
        user[numONUs]: Host {
            LLID = index;
            hostDatarate = cEth;
            sourceNumber = 64;
            load = upLoad / numONUs;
            EFWeight = upEFWeight;
            EFMaxSize = upEFMax;
            EFMinSize = upEFMin;
            AFWeight = upAFWeight;
            AFMaxSize = upAFMax;
            AFMinSize = upAFMin;
            BEWeight = upBEWeight;
            BEMaxSize = upBEMax;
            BEMinSize = upBEMin;
            @display("p=430,430");
        }
        recorder: Recorder {
           @display("p=430,30");
        }
    connections:
        //service host connection
        for i=0..numONUs-1 {
            service[i].txSplitter --> SNICable --> oltHostCombiner.in[i];
            oltHostSplitter.txONU[i] --> service[i].rxSplitter;
            d.out[i] --> oltHostSplitter.rxONU[i];
        }
        oltHostSplitter.txOLT --> d.in[0];

        //connection from coupler to olt
        oltHostCombiner.out --> SNICable --> olt.rxHost;
        olt.txHost --> oltHostSplitter.rxOLT;

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
