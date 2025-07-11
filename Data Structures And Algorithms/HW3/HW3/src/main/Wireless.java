package main;

import main.protocols.Protocol;
import main.devices.wireless.WirelessIO;
import main.devices.wireless.Bluetooth;
import main.devices.wireless.Wifi;

/**
 * The Wireless class is a utility class that creates wireless objects
 * (Bluetooth, Wifi) based on the given type, protocol, and device ID.
 */
public class Wireless { 

    /**
     * Creates a wireless object of the specified type.
     *
     * @param deviceType  The type of wireless device to create ("bluetooth" or "wifi").
     * @param protocol    The communication protocol for the wireless device.
     * @param devId       The device ID.
     * @return            A WirelessIO object if the type is valid, or null otherwise.
     */
    public static WirelessIO createWireless(String deviceType, Protocol protocol, int devId) { 
        try {
            switch (deviceType) {
                case "Bluetooth":
                    return new Bluetooth(protocol, devId);
                case "Wifi":
                    return new Wifi(protocol, devId);
                default:
                    return null;
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Error creating device: " + e.getMessage());
            return null;
        }
    }
}