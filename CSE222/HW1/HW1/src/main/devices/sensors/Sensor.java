package main.devices.sensors;

import main.core.Device;
import main.protocols.Protocol;;

public abstract class Sensor extends Device { // Sensor abstract class extends Device class

    public Sensor (Protocol protocol) { // constructor
        super(protocol);
    }

    public abstract String getSensType(); // abstract method to get the sensor type
    public abstract String data2String(); // abstract method to convert data to string

    @Override
    public String getDevType() { // returns the type of the device
        return getSensType() + "Sensor";
    }
    
}
