package main.devices.sensors.temp;

import main.devices.sensors.Sensor;
import main.protocols.Protocol;


/**
 * Abstract class representing a Temperature sensor.
 * This class extends the {@link Sensor} class and provides functionality specific to temperature sensors.
 */
public abstract class TempSensor extends Sensor {

    /**
     * Initializes the Temperature sensor with the protocol provided and assigns a unique device ID.
     * @param protocol The protocol used by the Temperature sensor device for communication.
     * @param devId The unique identifier for the Temperature sensor device.
     */    
    public TempSensor(Protocol protocol, int devId) { super(protocol, devId); }

    /**
     * Retrieves the temperature from the Temperature sensor.
     * @return The temperature value as a float.
     */
    public Float getTemp() { 
        protocol.read();
        return 24.00f;
    }

    /**
     * Returns the type of the Temperature sensor.
     * @return The type of the sensor, which is "TempSensor".
     */
    @Override
    public String getSensType() { return "TempSensor "; }

    /**
     * Converts the temperature data to a string representation.
     * @return A string representation of the temperature, including the "C" unit.
     */
    @Override
    public String data2String() {
        return "Temperature: " + String.format("%.2f", getTemp()) + "C.";
    } 
}