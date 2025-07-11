package main.devices.sensors.temp;

import main.protocols.Protocol;
import main.core.State;
import main.protocols.OneWire;

public class DHT11 extends TempSensor { // DHT11 class extends TempSensor class
    public DHT11(Protocol protocol) { // constructor
        super(protocol);
        if (!(protocol instanceof OneWire)) { 
            throw new IllegalArgumentException("DHT11 requires OneWire protocol");
        }
    }

    @Override
    public void turnOn() { // turns on the DHT11
        if (state == State.ON) {
            throw new IllegalStateException("DHT11 is already on");
        } else {        
            state = State.ON;
            protocol.write("DHT11 turned ON");
            }
    }

    @Override
    public void turnOff() { // turns off the DHT11
        if (state == State.OFF) {
            throw new IllegalStateException("DHT11 is already off");
        } else {
            state = State.OFF;
            protocol.write("DHT11 turned OFF");
        }
    }

    @Override
    public String getName() { // returns the name of the DHT11
        return "DHT11";
    }
}
