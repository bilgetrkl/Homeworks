package main.core;

import main.protocols.Protocol;

public class Port { // represents a port in the system
    private Protocol protocol;   
    private Device device;        
    private int portId;         

    public Port(Protocol protocol, int portId) { // constructor
        this.protocol = protocol;
        this.portId = portId;
        this.device = null;     
    }

    public Protocol getProtocol() { // returns the protocol of the port
        return protocol;
    }

    public Device getDevice() { // returns the device connected to the port
        return device;
    }

    public int getPortId() { // returns the port ID
        return portId;
    }

    public void setDevice(Device device) { // sets the device connected to the port
        this.device = device;
    }

    public boolean isOccupied() { // checks if the port is occupied
        return device != null;
    }

    public String getStatus() { // returns the status of the port
        StringBuilder status = new StringBuilder();
        status.append("Port ").append(portId).append(" ");
        status.append(protocol.getProtocolName()).append(" ");
        
        if (isOccupied()) {
            status.append("occupied ").append(device.getName());
        } else {
            status.append("empty");
        }
        
        return status.toString();
    }
}
