package main.devices.displays;

import main.protocols.Protocol;
import main.protocols.I2C;
import main.core.State;

public class LCD extends Display { // LCD class extends Display class
    public LCD(Protocol protocol){ // constructor
        super(protocol);
        if(!(protocol instanceof I2C)){
            throw new IllegalArgumentException("LCD requires I2C protocol");
        }
    }

    @Override
    public void turnOn() { // turns on the LCD
        if (state == State.ON) {
            throw new IllegalStateException("LCD is already on");
        } else {
            state = State.ON;
            protocol.write("LCD turned on");
        }

    }

    @Override
    public void turnOff() { // turns off the LCD
        if (state == State.OFF) {
            throw new IllegalStateException("LCD is already off");
        } else {
            state = State.OFF;
            protocol.write("LCD turned off");
        }
    }

    @Override
    public String getName(){ // returns the name of the LCD
        return "LCD";
    }
}
