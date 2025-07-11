package main.devices.displays;

import main.core.Device;
import main.core.State;
import main.protocols.Protocol;

public abstract class Display extends Device { // Display abstract class extends Device class
    public Display(Protocol protocol) {  // constructor 
        super(protocol);
    }

    public void printData(String data) { // prints the given parameter
        if (state == State.ON) {
            protocol.write(data);
        }
    }

    @Override
    public String getDevType() { // returns the type of the device
        return "Display";
    }
    
}
