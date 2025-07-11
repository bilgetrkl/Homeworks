package main.devices.sensors.temp;

import main.protocols.Protocol;
import main.core.State;
import main.protocols.OneWire;

/**
 * DHT11 class that extends the {@link TempSensor} class.
 * Represents an DHT11 tempsensor device that uses OneWire protocol.
 */
public class DHT11 extends TempSensor { 

    /**
     * Constructs an DHT11 device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link OneWire} for this DHT11 device.
     * @param protocol The protocol used by the DHT11 device (must be OneWire).
     * @param devId The unique identifier for the DHT11 device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link OneWire}.
     */
    public DHT11(Protocol protocol, int devId) { 
        super(protocol, devId);
        if (!(protocol instanceof OneWire)) { 
            throw new IllegalArgumentException("DHT11 requires OneWire protocol");
        }
    }

    /**
     * Turns on the DHT11 tempsensor.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the DHT11 is already on.
     */
    @Override
    public void turnOn() { 
        if (state == State.ON) {
            throw new IllegalStateException("DHT11 is already on");
        } else {        
            state = State.ON;
            protocol.write("turnON");
            System.out.println("DHT11: Turning ON.");
        }
    }

    /**
     * Turns off the DHT11 tempsensor.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the DHT11 is already off.
     */
    @Override
    public void turnOff() {
        if (state == State.OFF) {
            throw new IllegalStateException("DHT11 is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("DHT11: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the DHT11 device.
     * @return The device ID of the DHT11.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the DHT11 device.
     * @return The name of the DHT11, which is "DHT11" for this class.
     */   
    @Override
    public String getName() { return "DHT11"; }
}