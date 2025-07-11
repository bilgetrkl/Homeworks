package main.devices.displays;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.core.State;

/**
 * OLED class that extends the {@link Display} class.
 * Represents an OLED display device that uses the SPI protocol.
 */
public class OLED extends Display { 

    /**
     * Constructs an OLED device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link SPI} for this OLED device.
     * @param protocol The protocol used by the OLED device (must be SPI).
     * @param devId The unique identifier for the OLED device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link SPI}.
     */
    public OLED(Protocol protocol, int devId) {
        super(protocol, devId);
        if(!(protocol instanceof SPI)) {
            throw new IllegalArgumentException("OLED requires SPI protocol");
        }
    }

    /**
     * Turns on the OLED display.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the OLED is already on.
     */   

    @Override
    public void turnOn() {
        if (state == State.ON) {
            System.err.println("OLED is already on");
            return;
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("OLED: Turning ON.");
        }
    }

    /**
     * Turns off the OLED display.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the OLED is already off.
     */
    @Override
    public void turnOff() { 
        if (state == State.OFF) {
            System.err.println("OLED is already off");
            return;
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("OLED: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the OLED device.
     * @return The device ID of the OLED.
     */    
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the OLED device.
     * @return The name of the OLED, which is "OLED" for this class.
     */
    @Override
    public String getName() { return "OLED"; }
}