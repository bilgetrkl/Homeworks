package main.core;

import main.protocols.Protocol;

/**
 * Represents a port in the system that can be occupied by a device and uses a specific protocol.
 */
public class Port {
    private Protocol protocol;
    private Device device;
    private int portId;
    private String file;
    
    /**
     * Constructs a new Port with the specified protocol, port ID, and log directory.
     * Initializes the port and sets up logging for the protocol.
     * @param protocol The communication protocol used by the port.
     * @param portId The unique identifier of the port.
     * @param logDir The directory where the port logs will be stored.
     */
    public Port(Protocol protocol, int portId, String logDir) {
        this.protocol = protocol;
        this.portId = portId;
        this.device = null;
        this.file = logDir + "/" + protocol.getProtocolName() + "_" + portId + ".log";
        this.protocol.setProtocolFile(file); // set the protocol file
        this.protocol.write("Port Opened.");
    }

    /**
     * Returns the protocol associated with this port.
     * @return The {@link Protocol} used by this port.
     */
    public Protocol getProtocol() { return protocol; }

    /**
     * Returns the device connected to this port, if any.
     * @return The {@link Device} connected to this port, or null if the port is empty.
     */    
    public Device getDevice() { return device; }

    /**
     * Returns the unique identifier of this port.
     * @return The port ID as an integer.
     */
    public int getPortId() { return portId; }

    /**
     * Sets the device connected to this port.
     * @param device The {@link Device} to be assigned to this port.
     */
    public void setDevice(Device device) { this.device = device; }

    /**
     * Checks if the port is currently occupied by a device.
     * @return true if the port has a connected device, false otherwise.
     */
    public boolean isOccupied() { return device != null; }

    /**
     * Returns the status of the port as a formatted string.
     * The status includes the port ID, protocol name, and whether it is occupied or empty.
     * @return A String representing the status of the port.
     */
    public String getStatus() { 
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