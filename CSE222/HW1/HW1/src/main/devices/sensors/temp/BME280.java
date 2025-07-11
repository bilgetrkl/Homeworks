package main.devices.sensors.temp;

import main.protocols.Protocol;
import main.core.State;
import main.protocols.I2C;
import main.protocols.SPI;

public class BME280 extends TempSensor { // BME280 class extends TempSensor class
    public BME280(Protocol protocol) { // constructor
        super(protocol);
        if (!(protocol instanceof I2C ||  protocol instanceof SPI)) { 
            throw new IllegalArgumentException("BME280 requires I2C or SPI protocols");
        }
    }

    @Override
    public void turnOn() { // turns on the BME280
        if (state == State.ON) {
            throw new IllegalStateException("BME280 is already on");
        } else {        
            state = State.ON;
            protocol.write("BME280 turned ON");
            }
    }

    @Override
    public void turnOff() { // turns off the BME280
        if (state == State.OFF) {
            throw new IllegalStateException("BME280 is already off");
        } else {
            state = State.OFF;
            protocol.write("BME280 turned OFF");
        }
    }

    @Override
    public String getName() { // returns the name of the BME280
        return "BME280";
    }
}

