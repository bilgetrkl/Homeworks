package main.protocols;

/**
 * Represents a communication protocol interface.
 * Any class implementing this interface must provide
 * methods for reading, writing, logging, and handling protocol-specific settings.
 */
public interface Protocol { 

    /**
     * Reads data from the protocol. 
     * @return The data read as a string.
     */
    String read();

    /**
     * Writes data to the protocol.
     * @param data The data to be written.
     */
    void write(String data);

    /**
     * Returns the name of the protocol.
     * @return The protocol name.
     */
    String getProtocolName(); 

    /**
     * Writes all logged protocol operations to a file.
     * If the file does not exist, it will be created.
     */
    void writeLogsToFile();

    /**
     * Sets the protocol file.
     * @param protocolFile The file to write protocol logs to.
     */
    void setProtocolFile(String protocolFile);

    /**
     * Gets the protocol file.
     * @return The file to write protocol logs to.
     */
    int getPortId();
}