package main.devices.sensors.imu;

import main.devices.sensors.Sensor;
import main.protocols.Protocol;

public abstract class IMUSensor extends Sensor { // IMUSensor abstract class extends Sensor class

    public IMUSensor (Protocol protocol) { // constructor
        super(protocol);
    }

    public Float getAcceI () { // returns the acceleration 
        protocol.read();
        return (float) (Math.random() * 15);
    }

    public Float getRot () { // returns the rotation speed
        protocol.read();
        return (float) (Math.random() * 360);
    }

    @Override
    public String getSensType() { // returns the type of the sensor
        return "IMU";
    }

    @Override
    public String data2String() { // returns the data as a string
        return "AcceI: " + getAcceI() + " Rot: " + getRot();
    }
    
}
