package main.devices.displays;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.core.State;

public class OLED extends Display { // OLED class extends Display class
    public OLED(Protocol protocol) { // constructor
        super(protocol);
        if(!(protocol instanceof SPI)) {
            throw new IllegalArgumentException("OLED requires SPI protocol");
        }
    }

    @Override
    public void turnOn() { // turns on the OLED
        if (state == State.ON) {
            throw new IllegalStateException("OLED is already on");
        } else {
            state = State.ON;
            protocol.write("OLED turned on");
        }
    }

    @Override
    public void turnOff() { // turns off the OLED
        if (state == State.OFF) {
            throw new IllegalStateException("OLED is already off");
        } else {
            state = State.OFF;
            protocol.write("OLED turned off");
        }
    }

    @Override
    public String getName() { // returns the name of the OLED
        return "OLED";
    }
}
