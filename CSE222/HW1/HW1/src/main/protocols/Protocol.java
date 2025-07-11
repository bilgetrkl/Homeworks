package main.protocols;

public interface Protocol { // interface for the protocols

    String read(); // read method
    void write(String data); // write method
    String getProtocolName();   // gets the protocol name
    
}
