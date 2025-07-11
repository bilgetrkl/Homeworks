package main.devices.motors;

import main.protocols.I2C;
import main.protocols.Protocol;
import main.core.State;

/**
 * PCA9685 class that extends the {@link MotorDriver} class.
 * Represents an PCA9685 motordriver device that uses the I2C protocol.
 */
public class PCA9685 extends MotorDriver {

    /**
     * Constructs an PCA9685 device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link I2C} for this PCA9685 device.
     * @param protocol The protocol used by the PCA9685 device (must be I2C).
     * @param devId The unique identifier for the PCA9685 device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link I2C}.
     */
    public PCA9685(Protocol protocol, int devId) {
        super(protocol, devId);
        if (!(protocol instanceof I2C)) {
            throw new IllegalArgumentException("PCA9685 requires I2C protocol");
        }
    }

    /**
     * Turns on the PCA9685 motordriver.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the PCA9685 is already on.
     */
    @Override
    public void turnOn() {
        if (state == State.ON) {
            throw new IllegalStateException("PCA9685 is already on");
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("PCA9685: Turning ON.");
        }
    }

    /**
     * Turns off the PCA9685 motordriver.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the PCA9685 is already off.
     */
    @Override
    public void turnOff() { 
        if (state == State.OFF) {
            throw new IllegalStateException("PCA9685 is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("PCA9685: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the PCA9685 device.
     * @return The device ID of the PCA9685.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the PCA9685 device.
     * @return The name of the PCA9685, which is "PCA9685" for this class.
     */
    @Override
    public String getName() { return "PCA9685"; }
}