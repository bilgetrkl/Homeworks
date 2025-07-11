package main.protocols;

public class UART implements Protocol { // UART class implements Protocol interface

    @Override
    public String getProtocolName() { // gets the protocol name
        return "UART";
    }

    @Override
    public String read () { // read method
        return getProtocolName() + ": Reading";
    }

    @Override
    public void write (String data) { // write method
        System.out.printf("%s: Writing %s\n", getProtocolName(), data);
    }
    
}
