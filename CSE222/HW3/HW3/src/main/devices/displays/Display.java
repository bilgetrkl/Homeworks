package main.devices.displays;

import main.core.Device;
import main.core.State;
import main.protocols.Protocol;

/**
 * Abstract class representing a display device.
 * It extends from the {@link Device} class and provides functionality specific to display devices.
 */
public abstract class Display extends Device {

    /**
     * Constructs a new Display device with the specified protocol and device ID.
     * Initializes the display with the protocol provided.
     * @param protocol The protocol used by the display device.
     * @param devId The unique identifier for the display device.
     */
    public Display(Protocol protocol, int devId) { super(protocol, devId); }

    /**
     * Prints the provided data to the display device if it is turned on.
     * @param devName The name of the device.
     * @param data The data to be printed on the display.
     * @throws IllegalStateException if the display is turned off when trying to print.
     */
    public void printData(String devName, String data) { 
        if (state == State.ON) {
            protocol.write(data);
            System.out.println(devName + ": Printing \"" + data + "\".");
        } else { throw new IllegalStateException("Display is OFF"); }
    }

    /**
     * Returns the type of the device as a String.
     * @return The type of the device, which is "Display" for this class.
     */
    @Override
    public String getDevType() { return "Display"; }
}