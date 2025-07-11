package main;

import main.devices.displays.Display;
import main.devices.displays.LCD;
import main.devices.displays.OLED;
import main.protocols.Protocol;

/**
 * The Displays class is a utility class that creates display objects 
 * (LCD, OLED) based on the given type, protocol, and device ID.
 */
public class Displays {

    /**
     * Creates a display object of the specified type.
     *
     * @param deviceType  The type of display to create ("oled" or "lcd").
     * @param protocol    The communication protocol for the display.
     * @param devId       The device ID.
     * @return            A Display object if the type is valid, or null otherwise.
     */ 
    public static Display createDisplay(String deviceType, Protocol protocol, int devId) {
        try {
            switch (deviceType) {
                case "OLED":
                    return new OLED(protocol, devId);
                case "LCD":
                    return new LCD(protocol, devId);
                default:
                    return null;
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Error creating display: " + e.getMessage());
            return null;
        }
    }
}