package main.core;

import main.protocols.Protocol;

/**
 * Abstract class representing a general device.
 * All specific device types (e.g., LCD, Sensor) should extend this class.
 */
public abstract class Device {

    /**
     * The protocol used by the device.
     */
    protected Protocol protocol;
    
    /**
     * The current state of the device (ON, OFF).
     */
    protected State state;

    /**
     * The device ID.
     */
    protected int devId;

    /**
     * Constructs a new device with the specified protocol and device ID.
     * Initializes the state of the device to OFF by default.
     * @param protocol The protocol used by the device.
     * @param devId The unique identifier for the device.
     */
    public Device(Protocol protocol, int devId) {
        this.protocol = protocol;
        this.state = State.OFF;
        this.devId = devId; 
    }

    /**
     * Abstract method to turn the device on.
     * Concrete classes must implement this method to define specific behavior.
     */
    public abstract void turnOn();

    /**
     * Abstract method to turn the device off.
     * Concrete classes must implement this method to define specific behavior.
     */
    public abstract void turnOff();

    /**
     * Abstract method to get the name of the device.
     * Concrete classes must implement this method to define specific behavior.
     * @return The name of the device.
     */
    public abstract String getName();

    /**
     * Abstract method to get the type of the device.
     * Concrete classes must implement this method to define specific behavior.
     * @return The type of the device.
     */
    public abstract String getDevType();

    /**
     * Abstract method to get the unique identifier (ID) of the device.
     * Concrete classes must implement this method to define specific behavior.
     * @return The device ID.
     */
    public abstract int getDevId();

    /**
     * Gets the protocol used by the device.
     * @return The protocol used by the device.
     */
    public Protocol getProtocol() { return protocol; }

    /**
     * Gets the current state of the device.
     * @return The state of the device.
     */
    public State getState() { return state; }
}