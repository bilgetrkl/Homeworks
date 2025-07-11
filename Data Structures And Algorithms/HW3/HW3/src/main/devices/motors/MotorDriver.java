package main.devices.motors;

import main.core.Device;
import main.core.State;
import main.protocols.Protocol;

/**
 * Abstract class representing a motordriver device.
 * It extends from the {@link Device} class and provides functionality specific to motordriver devices.
 */
public abstract class MotorDriver extends Device { 

    /**
     * Constructs a new MotorDriver device with the specified protocol and device ID.
     * Initializes the motordriver with the protocol provided.
     * @param protocol The protocol used by the motordriver device.
     * @param devId The unique identifier for the motordriver device.
     */ 
    public MotorDriver(Protocol protocol, int devId) { super(protocol, devId); }

    /**
     * Sets the speed of the motor if it is turned on.
     * @param devName The name of the device.
     * @param speed The speed to set the motor to.
     * @throws IllegalStateException if the motordriver is turned off when trying to set motor speed.
     */
    public void setMotorSpeed(String devName, int speed) { 
        if (state == State.ON) {
            String data = String.valueOf(speed);
            protocol.write(data);
            System.out.println(devName + ": Setting speed to " + speed + ".");
        }
        else {
            throw new IllegalStateException("MotorDriver is OFF");
        }
    }

    /**
     * Returns the type of the device as a String.
     * @return The type of the device, which is "MotorDriver" for this class.
     */
    @Override
    public String getDevType() { return "MotorDriver"; }
}
