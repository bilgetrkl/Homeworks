package main.devices.wireless;

import main.protocols.Protocol;
import main.protocols.SPI;
import main.protocols.UART;
import main.core.State;

public class Wifi extends WirelessIO { // Wifi class extends WirelessIO class
    public Wifi(Protocol protocol) { // constructor
        super(protocol);
        if (!(protocol instanceof SPI || protocol instanceof UART)) {
            throw new IllegalArgumentException("Wifi requires SPI or UART protocols");
        }
    }

    @Override
    public void turnOn() { // turns on the Wifi
        if (state == State.ON) {
            throw new IllegalStateException("Wifi is already on");
        } else {
            state = State.ON;
            protocol.write("Wifi turned on");
        }
    }

    @Override
    public void turnOff() { // turns off the Wifi
        if (state == State.OFF) {
            throw new IllegalStateException("Wifi is already off");
        } else {
            state = State.OFF;
            protocol.write("Wifi turned off");
        }
    }

    @Override
    public String getName() { // returns the name of the Wifi
        return "Wifi";
    }
}
