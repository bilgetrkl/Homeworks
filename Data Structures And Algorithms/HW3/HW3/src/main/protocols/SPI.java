package main.protocols;

import java.io.FileWriter;
import java.util.Stack;

/**
 * Represents a SPI communication protocol.
 * Implements the Protocol interface.
 */
public class SPI implements Protocol { 
    Stack<String> SPILogStack = new Stack<>();
    String SPIFile;
    int portId; 

    /**
     * Constructs a SPI object with the specified port ID.
     * @param portId The ID of the port associated with this SPI instance.
     */
    public SPI(int portId) { 
        this.SPILogStack = new Stack<>();
        this.portId = portId;
    }

    /**
     * Returns the name of the protocol.
     * @return The protocol name, which is "SPI".
     */
    @Override
    public String getProtocolName() { return "SPI"; }

    /**
     * Simulates reading data from the SPI connection.
     * Logs the read operation.
     * @return null since no actual data is being read.
     */
    @Override
    public String read () {
        SPILogStack.push("Reading.");
        return null;
    }

    /**
     * Simulates writing data to the SPI connection.
     * Logs the written data.
     * @param data The data to be written.
     */
    @Override
    public void write (String data) { 
        if (data.equals("Port Opened.")) {
            SPILogStack.push("Port Opened.");
            return;
        }
        SPILogStack.push("Writing \"" + data + "\".");
    } 

    /**
     * Writes all logged SPI operations to a file.
     * If the file does not exist, it will be created.
     */
    @Override
    public void writeLogsToFile() { 
        if (SPILogStack.isEmpty()) { return; }
    
        try {
            java.io.File file = new java.io.File(SPIFile);
            if (!file.exists()) { 
                file.getParentFile().mkdirs(); 
                file.createNewFile();
            }
            
            FileWriter fileWriter = new FileWriter(file, false); 
            while (!SPILogStack.isEmpty()) { 
                String data = SPILogStack.pop();
                fileWriter.write(data);
                fileWriter.write(System.lineSeparator()); 
            }
            fileWriter.close(); 
        } catch (Exception e) {
            System.err.println("Error writing to log file: " + e.getMessage());
        }
    }

    /**
     * Sets the file path where SPI logs will be stored.
     * @param protocolFile The file path for storing logs.
     */
    @Override
    public void setProtocolFile(String protocolFile) { this.SPIFile = protocolFile; }

    /**
     * Returns the port ID associated with this SPI instance.
     * @return The port ID.
     */
    @Override
    public int getPortId() { return portId; }
}