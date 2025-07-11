package main;

import main.devices.sensors.Sensor;
import main.devices.sensors.temp.DHT11;
import main.devices.sensors.temp.BME280;
import main.devices.sensors.imu.MPU6050;
import main.devices.sensors.imu.GY951;
import main.protocols.Protocol;

/**
 * The Sensors class is a utility class that creates sensor objects 
 * (DHT11, BME280, MPU6050, GY951) based on the given type, protocol, and device ID.
 */
public class Sensors {

    /**
     * Creates a sensor object of the specified type.
     *
     * @param deviceType  The type of sensor to create ("dht11", "bme280", "mpu6050", or "gy951").
     * @param protocol    The communication protocol for the sensor.
     * @param devId       The device ID.
     * @return            A Sensor object if the type is valid, or null otherwise.
     */
    public static Sensor createSensor(String deviceType, Protocol protocol, int devId) { 
        try {
            switch (deviceType) {
                case "DHT11":
                    return new DHT11(protocol, devId);
                case "BME280":
                    return new BME280(protocol, devId);
                case "MPU6050":
                    return new MPU6050(protocol, devId);
                case "GY951":
                    return new GY951(protocol, devId);
                default:
                    return null;
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Error creating device: " + e.getMessage());
            return null;
        }
    }
}