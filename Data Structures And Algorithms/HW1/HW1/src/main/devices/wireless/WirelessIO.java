package main.devices.wireless;

import main.core.Device;
import main.core.State;
import main.protocols.Protocol;

public abstract class WirelessIO extends Device { // WirelessIO abstract class extends Device class
    public WirelessIO(Protocol protocol) { // constructor
        super(protocol);
    }

    public void sendData(String data) { // sends the data
        if (state == State.ON) {
            protocol.write(data);
        }
    }

    public String recvData() { // receives the data
        if (state == State.ON) {
            return protocol.read();
        }
        return null;
    }
    
    @Override
    public String getDevType() { // returns the type of the device
        return "WirelessIO";
    }
}
