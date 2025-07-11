package main.devices.sensors.temp;

import main.protocols.Protocol;
import main.core.State;
import main.protocols.I2C;
import main.protocols.SPI;

/**
 * BME280 class that extends the {@link TempSensor} class.
 * Represents an BME280 imusensor device that uses I2C or SPI protocol.
 */
public class BME280 extends TempSensor { 

    /**
     * Constructs an BME280 device with the specified protocol and device ID.
     * Ensures the protocol is an instance of {@link I2C} or {@link SPI} for this BME280 device.
     * @param protocol The protocol used by the BME280 device (must be I2C or SPI).
     * @param devId The unique identifier for the BME280 device.
     * @throws IllegalArgumentException if the protocol is not an instance of {@link I2C} or {@link SPI}.
     */
    public BME280(Protocol protocol, int devId) { 
        super(protocol, devId);
        if (!(protocol instanceof I2C ||  protocol instanceof SPI)) { 
            throw new IllegalArgumentException("BME280 requires I2C or SPI protocols");
        }
    }

    /**
     * Turns on the BME280 tempsensor.
     * The device state is changed to {@link State#ON}, and a "turnON" command is sent via the protocol.
     * @throws IllegalStateException if the BME280 is already on.
     */
    @Override
    public void turnOn() { 
        if (state == State.ON) {
            throw new IllegalStateException("BME280 is already on");
        } else {        
            state = State.ON;
            protocol.write("turnON");
            System.out.println("BME280: Turning ON.");
        }
    }

    /**
     * Turns off the BME280 tempsensor.
     * The device state is changed to {@link State#OFF}, and a "turnOFF" command is sent via the protocol.
     * @throws IllegalStateException if the BME280 is already off.
     */
    @Override
    public void turnOff() {
        if (state == State.OFF) {
            throw new IllegalStateException("BME280 is already off");
        } else {
            state = State.OFF;
            protocol.write("turnOFF");
            System.out.println("BME280: Turning OFF.");
        }
    }

    /**
     * Returns the unique identifier of the BME280 device.
     * @return The device ID of the BME280.
     */
    @Override
    public int getDevId() { return devId; }

    /**
     * Returns the name of the BME280 device.
     * @return The name of the BME280, which is "BME280" for this class.
     */ 
    @Override
    public String getName() { return "BME280"; }
}