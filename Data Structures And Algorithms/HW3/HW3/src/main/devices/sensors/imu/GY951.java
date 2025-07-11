package main.devices.sensors.imu;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.protocols.UART;
import main.core.State;

/**
 * GY951 class that extends the {@link IMUSensor} class.
 * Represents an GY951 imusensor device that uses UART or SPI protocol.
 */
public class GY951 extends IMUSensor {
    
    /**
     * Constructs an GY951 device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link UART} or {@link SPI} for this GY951 device.
     * @param protocol The protocol used by the GY951 device (must be UART or SPI).
     * @param devId The unique identifier for the GY951 device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link UART} or {@link SPI}.
     */
    public GY951(Protocol protocol, int devId) {
        super(protocol, devId);
        if(!(protocol instanceof UART || protocol instanceof SPI)) {
            throw new IllegalArgumentException("GY951 requires UART or SPI protocols");
        }
    }

    /**
     * Turns on the GY951 imusensor.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the GY951 is already on.
     */
    @Override
    public void turnOn() { 
        if (state == State.ON) {
            throw new IllegalStateException("GY951 is already on");
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("GY951: Turning ON.");
        }
    }

    /**
     * Turns off the GY951 imusensor.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the GY951 is already off.
     */ 
    @Override
    public void turnOff() { 
        if (state == State.OFF) {
            throw new IllegalStateException("GY951 is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("GY951: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the GY951 device.
     * @return The device ID of the GY951.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the GY951 device.
     * @return The name of the GY951, which is "GY951" for this class.
     */    
    @Override
    public String getName() { return "GY951"; }
}