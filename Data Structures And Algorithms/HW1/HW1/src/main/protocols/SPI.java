package main.protocols;

public class SPI implements Protocol { // SPI class implements Protocol interface

    @Override
    public String getProtocolName() { // gets the protocol name
        return "SPI";
    }

    @Override
    public String read () { // read method
        return getProtocolName() + ": Reading";
    }

    @Override
    public void write (String data) {  // write method
        System.out.printf("%s: Writing %s\n", getProtocolName(), data);
    }
    
}