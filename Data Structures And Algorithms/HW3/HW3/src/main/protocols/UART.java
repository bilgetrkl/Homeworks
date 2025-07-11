package main.protocols;

import java.util.Stack;
import java.io.FileWriter;

/**
 * Represents a UART communication protocol.
 * Implements the Protocol interface.
 */
public class UART implements Protocol { 
    Stack<String> UARTLogStack = new Stack<>();
    String UARTFile;
    int portId;

    /**
     * Constructs a UART object with the specified port ID.
     * @param portId The ID of the port associated with this UART instance.
     */
    public UART(int portId) {
        this.UARTLogStack = new Stack<>();
        this.portId = portId;
    }

    /**
     * Returns the name of the protocol.
     * @return The protocol name, which is "UART".
     */
    @Override
    public String getProtocolName() { return "UART"; }

    /**
     * Simulates reading data from the UART connection.
     * Logs the read operation.
     * @return null since no actual data is being read.
     */
    @Override
    public String read () { 
        UARTLogStack.push("Reading.");
        return null;
    }

    /**
     * Simulates writing data to the UART connection.
     * Logs the written data.
     * @param data The data to be written.
     */
    @Override
    public void write (String data) {
        if (data.equals("Port Opened.")) {
            UARTLogStack.push("Port Opened.");
            return;
        }
        UARTLogStack.push("Writing \"" + data + "\".");
    }

    /**
     * Writes all logged UART operations to a file.
     * If the file does not exist, it will be created.
     */
    @Override
    public void writeLogsToFile() { 
        if (UARTLogStack.isEmpty()) { return; }
    
        try {
            java.io.File file = new java.io.File(UARTFile);
            if (!file.exists()) { 
                file.getParentFile().mkdirs(); 
                file.createNewFile();
            }
            
            FileWriter fileWriter = new FileWriter(file, false);
            while (!UARTLogStack.isEmpty()) { 
                String data = UARTLogStack.pop();
                fileWriter.write(data); 
                fileWriter.write(System.lineSeparator());
            }
            fileWriter.close();
        } catch (Exception e) {
            System.err.println("Error writing to log file: " + e.getMessage());
        }
    }

    /**
     * Sets the file path where UART logs will be stored.
     * @param protocolFile The file path for storing logs.
     */
    @Override
    public void setProtocolFile(String protocolFile) { this.UARTFile = protocolFile; }

    /**
     * Returns the port ID associated with this UART instance.
     * @return The port ID.
     */
    @Override
    public int getPortId() { return portId; }
}