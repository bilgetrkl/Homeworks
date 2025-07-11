package main.protocols;

import java.io.FileWriter;
import java.util.Stack;

/**
 * Represents an I2C communication protocol.
 * Implements the Protocol interface.
 */
public class I2C implements Protocol { 
    Stack<String> I2CLogStack = new Stack<>();
    String I2CFile;
    int portId;

    /**
     * Constructs an I2C object with the specified port ID.
     * @param portId The ID of the port associated with this I2C instance.
     */
    public I2C(int portId) {
        this.I2CLogStack = new Stack<>();
        this.portId = portId;
    }

    /**
     * Returns the name of the protocol.
     * @return The protocol name, which is "I2C".
     */
    @Override
    public String getProtocolName() { return "I2C"; }

    /**
     * Simulates reading data from the I2C connection.
     * Logs the read operation.
     * @return null since no actual data is being read.
     */
    @Override
    public String read () { 
        I2CLogStack.push("Reading.");
        return null;
    }

    /**
     * Simulates writing data to the I2C connection.
     * Logs the written data.
     * @param data The data to be written.
     */
    @Override
    public void write (String data) {  
        if (data.equals("Port Opened.")) {
            I2CLogStack.push("Port Opened.");
            return;
        }
        I2CLogStack.push("Writing \"" + data + "\".");  
    }


    /**
     * Writes all logged I2C operations to a file.
     * If the file does not exist, it will be created.
     */
    @Override
    public void writeLogsToFile() {
        if (I2CLogStack.isEmpty()) { return; }
    
        try {
            java.io.File file = new java.io.File(I2CFile);
            if (!file.exists()) { 
                file.getParentFile().mkdirs();
                file.createNewFile();
            }
            
            FileWriter fileWriter = new FileWriter(file, false); 
            while (!I2CLogStack.isEmpty()) {
                String data = I2CLogStack.pop(); 
                fileWriter.write(data);
                fileWriter.write(System.lineSeparator());
            }
            fileWriter.close();
        } catch (Exception e) {
            System.err.println("Error writing to log file: " + e.getMessage());
        }
    }

    /**
     * Sets the file to which the I2C logs will be written.
     * @param protocolFile The file to which the I2C logs will be written.
     */
    @Override
    public void setProtocolFile(String protocolFile) { this.I2CFile = protocolFile; }

    /**
     * Returns the port ID associated with this I2C instance.
     * @return The port ID.
     */
    @Override
    public int getPortId() { return portId; }
}