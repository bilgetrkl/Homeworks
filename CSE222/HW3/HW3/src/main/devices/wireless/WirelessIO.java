package main.devices.wireless;

import main.core.Device;
import main.core.State;
import main.protocols.Protocol;

/**
 * Abstract class representing a wirelessio device.
 * It extends from the {@link Device} class and provides functionality specific to wirelessio devices.
 */
public abstract class WirelessIO extends Device { 

    /**
     * Constructs a new WirelessIO device with the specified protocol and device ID.
     * Initializes the wirelessio with the protocol provided.
     * @param protocol The protocol used by the wirelessio device.
     * @param devId The unique identifier for the wirelessio device.
     */
    public WirelessIO(Protocol protocol, int devId) { super(protocol, devId); }

    /**
     * Sends the provided data if the device is turned on.
     * @param devName The name of the device.
     * @param data The data to be sent.
     * @throws IllegalStateException if the device is turned off when trying to send data.
     */
    public void sendData(String devName, String data) { 
        if (state == State.ON) {
            protocol.write(data);
            System.out.println(devName + ": Sending \"" + data + "\".");
        }
        else {
            throw new IllegalStateException("WirelessIO is OFF");
        }
    }

    /**
     * Receives data if the device is turned on.
     * @param devName The name of the device.
     * @throws IllegalStateException if the device is turned off when trying to receive data.
     */
    public void recvData(String devName) { 
        if (state == State.ON) {
            protocol.read();
            System.out.println(devName + ": Received \"Some Data\"." );
        }
        else {
            throw new IllegalStateException("WirelessIO is OFF");
        }
    }

    /**
     * Returns the type of the device as a String.
     * @return The type of the device, which is "WirelessIO" for this class.
     */
    @Override
    public String getDevType() { return "WirelessIO"; }
}