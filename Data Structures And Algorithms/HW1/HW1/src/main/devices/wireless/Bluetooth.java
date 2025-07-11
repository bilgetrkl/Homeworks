package main.devices.wireless;

import main.protocols.Protocol;
import main.protocols.UART;
import main.core.State;

public class Bluetooth extends WirelessIO { // Bluetooth class extends WirelessIO class
    public Bluetooth(Protocol protocol) { // constructor
        super(protocol);
        if (!(protocol instanceof UART)) {
            throw new IllegalArgumentException("Bluetooth requires UART protocol");
        }
    }

    @Override
    public void turnOn() { // turns on the Bluetooth
        if (state == State.ON) {
            throw new IllegalStateException("Bluetooth is already on");
        } else {
            state = State.ON;
            protocol.write("Bluetooth turned on");
        }
    }

    @Override
    public void turnOff() { // turns off the Bluetooth
        if (state == State.OFF) {
            throw new IllegalStateException("Bluetooth is already off");
        } else {
            state = State.OFF;
            protocol.write("Bluetooth turned off"); 
        }

    }

    @Override
    public String getName() { // returns the name of the Bluetooth
        return "Bluetooth";
    }
}
