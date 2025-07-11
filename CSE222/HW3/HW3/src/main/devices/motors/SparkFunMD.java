package main.devices.motors;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.core.State;

/**
 * SparkFunMD class that extends the {@link MotorDriver} class.
 * Represents an SparkFunMD motordriver device that uses the SPI protocol.
 */
public class SparkFunMD extends MotorDriver {

    /**
     * Constructs an SparkFunMD device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link SPI} for this SparkFunMD device.
     * @param protocol The protocol used by the SparkFunMD device (must be SPI).
     * @param devId The unique identifier for the SparkFunMD device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link SPI}.
     */
    public SparkFunMD(Protocol protocol, int devId) { 
        super(protocol, devId); 
        if (!(protocol instanceof SPI)) {
            throw new IllegalArgumentException ("SparkFunMD requires SPI protocol");
        }
    }

    /**
     * Turns on the SparkFunMD motordriver.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the SparkFunMD is already on.
     */
    @Override
    public void turnOn() {
        if (state == State.ON) {
            throw new IllegalStateException("SparkFunMD is already on");
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("SparkFunMD: Turning ON.");
        }
    }

    /**
     * Turns off the SparkFunMD motordriver.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the SparkFunMD is already off.
     */
    @Override
    public void turnOff() { 
        if (state == State.OFF) {
            throw new IllegalStateException("SparkFunMD is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("SparkFunMD: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the SparkFunMD device.
     * @return The device ID of the SparkFunMD.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the SparkFunMD device.
     * @return The name of the SparkFunMD, which is "SparkFunMD" for this class.
     */
    @Override
    public String getName() { return "SparkFunMD"; }
}