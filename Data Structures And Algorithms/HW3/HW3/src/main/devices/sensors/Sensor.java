package main.devices.sensors;

import main.core.Device;
import main.protocols.Protocol;;

/**
 * Abstract class representing a sensor device.
 * It extends from the {@link Device} class and provides functionality specific to sensor devices.
 */
public abstract class Sensor extends Device {

    /**
     * Constructs a new Sensor device with the specified protocol and device ID.
     * Initializes the sensor with the protocol provided.
     * @param protocol The protocol used by the sensor device.
     * @param devId The unique identifier for the sensor device.
     */
    public Sensor (Protocol protocol, int devId) { super(protocol, devId); }

    /**
     * Abstract method to get the type of the sensor.
     * This method should be implemented by subclasses to return the specific sensor type.
     * @return The type of the sensor as a {@link String}.
     */
    public abstract String getSensType(); 

    /**
     * Abstract method to convert the sensor's data to a string representation.
     * This method should be implemented by subclasses to return a formatted string of the sensor's data.
     * @return A string representation of the sensor's data.
     */
    public abstract String data2String();

    /**
     * Returns the type of the device as a String.
     * @return The type of the device, which is "Sensor" for this class.
     */
    @Override
    public String getDevType() { return getSensType() + "Sensor"; }
}