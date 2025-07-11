package main.devices.sensors.imu;

import main.protocols.Protocol;
import main.protocols.I2C;
import main.core.State;

/**
 * MPU6050 class that extends the {@link IMUSensor} class.
 * Represents an MPU6050 imusensor device that uses I2C protocol.
 */
public class MPU6050 extends IMUSensor {

    /**
     * Constructs an MPU6050 device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link I2C} for this MPU6050 device.
     * @param protocol The protocol used by the MPU6050 device (must be I2C).
     * @param devId The unique identifier for the MPU6050 device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link I2C}.
     */
    public MPU6050(Protocol protocol, int devId) { 
        super(protocol, devId);
        if(!(protocol instanceof I2C)) {
            throw new IllegalArgumentException("MPU6050 requires I2C protocol");
        }
    }

    /**
     * Turns on the MPU6050 imusensor.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the MPU6050 is already on.
     */
    @Override
    public void turnOn() { 
        if (state == State.ON) {
            throw new IllegalStateException("MPU6050 is already on");
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("MPU6050: Turning ON.");  
        }
    }

    /**
     * Turns off the MPU6050 imusensor.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the MPU6050 is already off.
     */
    @Override
    public void turnOff() { 
        if (state == State.OFF) {
            throw new IllegalStateException("MPU6050 is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("MPU6050: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the MPU6050 device.
     * @return The device ID of the MPU6050.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the MPU6050 device.
     * @return The name of the MPU6050, which is "MPU6050" for this class.
     */    
    @Override
    public String getName() { return "MPU6050"; }
}