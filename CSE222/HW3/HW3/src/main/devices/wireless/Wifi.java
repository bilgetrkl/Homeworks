package main.devices.wireless;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.protocols.UART;
import main.core.State;

/**
 * Wifi class that extends the {@link WirelessIO} class.
 * Represents an Wifi wirelessio device that uses SPI or UART protocol.
 */
public class Wifi extends WirelessIO { 

    /**
     * Constructs an Wifi device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link SPI} or {@link UART} for this Wifi device.
     * @param protocol The protocol used by the Wifi device (must be SPI or UART).
     * @param devId The unique identifier for the Wifi device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link SPI} or {@link UART}.
     */
    public Wifi(Protocol protocol, int devId) {
        super(protocol, devId);
        if (!(protocol instanceof SPI || protocol instanceof UART)) {
            throw new IllegalArgumentException("Wifi requires SPI or UART protocols");
        }
    }

    /**
     * Turns on the wifi wirelessio.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the wifi is already on.
     */
    @Override
    public void turnOn() { 
        if (state == State.ON) {
            throw new IllegalStateException("Wifi is already on");
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("Wifi: Turning ON.");
        }
    }

    /**
     * Turns off the wifi wirelessio.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the wifi is already off.
     */
    @Override
    public void turnOff() {
        if (state == State.OFF) {
            throw new IllegalStateException("Wifi is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("Wifi: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the Wifi device.
     * @return The device ID of the Wifi.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the wifi device.
     * @return The name of the Wifi, which is "Wifi" for this class.
     */ 
    @Override
    public String getName() { return "Wifi"; }
}