package main;

import main.devices.motors.MotorDriver;
import main.devices.motors.PCA9685;
import main.devices.motors.SparkFunMD;
import main.protocols.Protocol;

/**
 * The MotorDrivers class is a utility class that creates motordriver objects 
 * (PCA9685, SparkFunMD) based on the given type, protocol, and device ID.
 */
public class MotorDrivers {

    /**
     * Creates a motordriver object of the specified type.
     * @param deviceType  The type of motordriver to create ("pca9685" or "sparkfunmd").
     * @param protocol    The communication protocol for the motordriver.
     * @param devId       The device ID.
     * @return            A MotorDriver object if the type is valid, or null otherwise.
     */
    public static MotorDriver createMotor(String deviceType, Protocol protocol, int devId) { 
        try {
            switch (deviceType) {
                case "PCA9685":
                    return new PCA9685(protocol, devId);
                case "SparkFunMD":
                    return new SparkFunMD(protocol, devId);
                default:
                    return null;
            }
        } catch (IllegalArgumentException e) {
            System.err.println("Error creating device: " + e.getMessage());
            return null;
        }
    }
}