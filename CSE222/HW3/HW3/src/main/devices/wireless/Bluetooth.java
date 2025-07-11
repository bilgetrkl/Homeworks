package main.devices.wireless;

import main.protocols.Protocol;
import main.protocols.UART;
import main.core.State;

/**
 * Bluetooth class that extends the {@link WirelessIO} class.
 * Represents an Bluetooth wirelessio device that uses UART protocol.
 */
public class Bluetooth extends WirelessIO { 

    /**
     * Constructs an Bluetooth device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link UART} for this Bluetooth device.
     * @param protocol The protocol used by the Bluetooth device (must be UART).
     * @param devId The unique identifier for the Bluetooth device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link UART}.
     */
    public Bluetooth(Protocol protocol, int devId) {
        super(protocol, devId);
        if (!(protocol instanceof UART)) {
            throw new IllegalArgumentException("Bluetooth requires UART protocol");
        }
    }

    /**
     * Turns on the bluetooth wirelessio.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the bluetooth is already on.
     */
    @Override
    public void turnOn() { // turns on the Bluetooth
        if (state == State.ON) {
            throw new IllegalStateException("Bluetooth is already on");
        } else {
            state = State.ON;
            protocol.write("turnON");
            System.out.println("Bluetooth: Turning ON.");
        }
    }
    
    /**
     * Turns off the bluetooth wirelessio.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the bluetooth is already off.
     */
    @Override
    public void turnOff() { // turns off the Bluetooth
        if (state == State.OFF) {
            throw new IllegalStateException("Bluetooth is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("Bluetooth: Turning OFF."); 
        }
    }

    /**
     * Returns the unique identifier of the Bleutooth device.
     * @return The device ID of the Bluetooth.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the Bluetooth device.
     * @return The name of the Bluetooth, which is "Bluetooth" for this class.
     */   
    @Override
    public String getName() { return "Bluetooth"; }
}