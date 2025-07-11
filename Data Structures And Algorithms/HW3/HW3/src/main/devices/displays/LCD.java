package main.devices.displays;

import main.protocols.Protocol;
import main.protocols.I2C;
import main.core.State;

/**
 * LCD class that extends the {@link Display} class.
 * Represents an LCD display device that uses the I2C protocol.
 */
public class LCD extends Display { 

    /**
     * Constructs an LCD device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link I2C} for this LCD device.
     * @param protocol The protocol used by the LCD device (must be I2C).
     * @param devId The unique identifier for the LCD device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link I2C}.
     */
    public LCD(Protocol protocol, int devId){ 
        super(protocol, devId);
        if(!(protocol instanceof I2C)){
            throw new IllegalArgumentException("LCD requires I2C protocol");
        }
    }

    /**
     * Turns on the LCD display.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the LCD is already on.
     */
    @Override
    public void turnOn() { 
        if (state == State.ON) {
            throw new IllegalStateException("LCD is already on");
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("LCD: Turning ON.");
        }
    }

    /**
     * Turns off the LCD display.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the LCD is already off.
     */
    @Override
    public void turnOff() { 
        if (state == State.OFF) {
            throw new IllegalStateException("LCD is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("LCD: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the LCD device.
     * @return The device ID of the LCD.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the LCD device.
     * @return The name of the LCD, which is "LCD" for this class.
     */
    @Override
    public String getName(){ return "LCD"; }
}