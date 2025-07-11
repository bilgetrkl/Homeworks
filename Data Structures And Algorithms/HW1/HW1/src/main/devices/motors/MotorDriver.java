package main.devices.motors;

import main.core.Device;
import main.core.State;
import main.protocols.Protocol;

public abstract class MotorDriver extends Device { // MotorDriver abstract class extends Device class
    public MotorDriver(Protocol protocol) { // constructor
        super(protocol);
    }

    public void setMotorSpeed(int speed) { // sets the motor speed
        if (state == State.ON) {
            protocol.write("Motor speed set to: " + speed);
        }
    }

    @Override
    public String getDevType() { // returns the type of the device
        return "MotorDriver";
    }
}
