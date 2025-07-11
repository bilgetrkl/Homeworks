package main.devices.sensors.imu;

import main.devices.sensors.Sensor;
import main.protocols.Protocol;

/**
 * Abstract class representing an IMU sensor.
 * This class extends the {@link Sensor} class and provides functionality specific to IMU sensors.
 */
public abstract class IMUSensor extends Sensor {

    /**
     * Initializes the IMU sensor with the protocol provided and assigns a unique device ID.
     * @param protocol The protocol used by the IMU sensor device for communication.
     * @param devId The unique identifier for the IMU sensor device.
     */
    public IMUSensor (Protocol protocol, int devId) { super(protocol, devId); }

    /**
     * Retrieves the acceleration of the IMU sensor.
     * @return The acceleration value as a float.
     */
    public Float getAccel() {
        protocol.read();
        return 1.00f;
    }

    /**
     * Retrieves the rotation of the IMU sensor.
     * @return The rotation value as a float.
     */
    public Float getRot() {
        protocol.read();
        return 0.50f;
    }

    /**
     * Returns the type of the IMU sensor.
     * @return The type of the sensor, which is "IMUSensor".
     */
    @Override
    public String getSensType() { return "IMUSensor "; }

    /**
     * Converts the sensor data to a string representation.
     * @return A string representation of the sensor data, including acceleration and rotation speed.
     */
    @Override
    public String data2String() { 
        return "Accel: " + String.format("%.2f", getAccel())  + ", Rot: " + String.format("%.2f", getRot()) ;
    }
}
