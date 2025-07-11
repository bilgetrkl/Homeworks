package main.devices.sensors.imu;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.protocols.UART;
import main.core.State;

public class GY951 extends IMUSensor { // GY951 class extends IMUSensor class
    public GY951(Protocol protocol) {  // constructor
        super(protocol);
        if(!(protocol instanceof UART || protocol instanceof SPI)) {
            throw new IllegalArgumentException("GY951 requires UART or SPI protocols");
        }
    }

    @Override
    public void turnOn() {  // turns on the GY951
        if (state == State.ON) {
            throw new IllegalStateException("GY951 is already on");
        } else {
            state = State.ON;
            protocol.write("GY951 turned on");
        }
    }

    @Override
    public void turnOff() { // turns off the GY951
        if (state == State.OFF) {
            throw new IllegalStateException("GY951 is already off");
        } else {
            state = State.OFF;
            protocol.write("GY951 turned off");
        }
    }

    @Override
    public String getName() { // returns the name of the GY951
        return "GY951";
    }
    
}
