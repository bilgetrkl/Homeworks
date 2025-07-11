package main.protocols;

public class I2C implements Protocol { // I2C class implements Protocol interface

    @Override
    public String getProtocolName() { // gets the protocol name
        return "I2C";
    }

    @Override
    public String read () { // read method
        return getProtocolName() + ": Reading";
    }

    @Override
    public void write (String data) {   // write method
        System.out.printf("%s: Writing %s\n", getProtocolName(), data);
    }
    
}