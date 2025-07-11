package main.devices.sensors.temp;

import main.devices.sensors.Sensor;
import main.protocols.Protocol;

public abstract class TempSensor extends Sensor { // TempSensor abstract class extends Sensor class

    public TempSensor(Protocol protocol) { // constructor
        super(protocol);
    }

    public Float getTemp () { // returns the temperature
        protocol.read();
        return (float) (Math.random() * 100);
    }

    @Override
    public String getSensType() { // returns the type of the sensor
        return "Temp";
    }

    @Override
    public String data2String() { // returns the temperature as a string
        return "Temperature: " + getTemp() + "°C";
    }
    
}
