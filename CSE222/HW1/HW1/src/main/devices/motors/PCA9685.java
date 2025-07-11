package main.devices.motors;

import main.protocols.I2C;
import main.protocols.Protocol;
import main.core.State;

public class PCA9685 extends MotorDriver { // PCA9685 class extends MotorDriver class
    public PCA9685(Protocol protocol) { // constructor
        super(protocol);
        if (!(protocol instanceof I2C)) {
            throw new IllegalArgumentException("PCA9685 requires I2C protocol");
        }
    }

    @Override
    public void turnOn() { // turns on the PCA9685
        if (state == State.ON) {
            throw new IllegalStateException("PCA9685 is already on");
        } else {
            state = State.ON;
            protocol.write("PCA9685 turned on");
        }
    }

    @Override
    public void turnOff() { // turns off the PCA9685
        if (state == State.OFF) {
            throw new IllegalStateException("PCA9685 is already off");
        } else {
            state = State.OFF;
            protocol.write("PCA9685 turned off");
        }

    }

    @Override
    public String getName() { // returns the name of the PCA9685
        return "PCA9685";
    }
}
