package main.devices.motors;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.core.State;

public class SparkFunMD extends MotorDriver { // SparkFunMD class extends MotorDriver class
    public SparkFunMD(Protocol protocol) { // constructor
        super(protocol);
        if (!(protocol instanceof SPI)) {
            throw new IllegalArgumentException("SparkFunMD requires SPI protocol");
        }
    }

    @Override
    public void turnOn() { // turns on the SparkFunMD
        if (state == State.ON) {
            throw new IllegalStateException("SparkFunMD is already on");
        } else {
            state = State.ON;
            protocol.write("SparkFunMD turned on");
        }
    }
    @Override
    public void turnOff() { // turns off the SparkFunMD
        
        if (state == State.OFF) {
            throw new IllegalStateException("SparkFunMD is already off");
        } else {
            state = State.OFF;
            protocol.write("SparkFunMD turned off");
        }

    }

    @Override
    public String getName() { // returns the name of the SparkFunMD
        return "SparkFunMD";
    }
}
