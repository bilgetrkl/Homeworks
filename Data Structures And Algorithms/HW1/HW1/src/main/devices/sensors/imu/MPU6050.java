package main.devices.sensors.imu;

import main.protocols.Protocol;
import main.protocols.I2C;
import main.core.State;

public class MPU6050 extends IMUSensor { // MPU6050 class extends IMUSensor class
    public MPU6050(Protocol protocol) { // constructor
        super(protocol);
        if(!(protocol instanceof I2C)) {
            throw new IllegalArgumentException("MPU6050 requires I2C protocol");
        }
    }

    @Override
    public void turnOn() { // turns on the MPU6050
        if (state == State.ON) {
            throw new IllegalStateException("MPU6050 is already on");
        } else {
            state = State.ON;
            protocol.write("MPU6050 turned on");  
        }
    }

    @Override
    public void turnOff() { // turns off the MPU6050
        if (state == State.OFF) {
            throw new IllegalStateException("MPU6050 is already off");
        } else {
            state = State.OFF;
            protocol.write("MPU6050 turned off");
        }
    }

    @Override
    public String getName() { // returns the name of the MPU6050
        return "MPU6050";
    }
    
}


