package main.core;

import main.protocols.Protocol;

public abstract class Device { // abstract class for all type of devices
    protected Protocol protocol; // protocol of the device
    protected State state; // state of the device

    public Device(Protocol protocol) {  // constructor
        this.protocol = protocol;
        this.state = State.OFF;
    }

    public abstract void turnOn();  // abstract method to turn on the device

    public abstract void turnOff(); // abstract method to turn off the device

    public abstract String getName(); // abstract method to get the name of the device

    public abstract String getDevType(); // abstract method to get the type of the device

    public State getState() { // method to get the state of the device
        return state;
    }
    
}
