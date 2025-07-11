package main.protocols;

import java.io.FileWriter;
import java.util.Stack;

/**
 * Represents a OneWire communication protocol.
 * Implements the Protocol interface.
 */
public class OneWire implements Protocol { 
    Stack<String> OneWireLogStack = new Stack<>(); 
    String OneWireFile;
    int portId;

    /**
     * Constructs a OneWire object with the specified port ID.
     * @param portId The ID of the port associated with this OneWire instance.
     */
    public OneWire(int portId) {
        this.OneWireLogStack = new Stack<>();
        this.portId = portId;
    }

    /**
     * Returns the name of the protocol.
     * @return The protocol name, which is "OneWire".
     */
    @Override
    public String getProtocolName() { return "OneWire"; }

    /**
     * Simulates reading data from the OneWire connection.
     * Logs the read operation.
     * @return null since no actual data is being read.
     */
    @Override
    public String read () { 
        OneWireLogStack.push("Reading.");
        return null;
    }

    /**
     * Simulates writing data to the OneWire connection.
     * Logs the written data.
     * @param data The data to be written.
     */
    @Override
    public void write (String data) {  // write method
        if (data.equals("Port Opened.")) {
            OneWireLogStack.push("Port Opened.");
            return;
        }
        OneWireLogStack.push("Writing \"" + data +"\".");
    }

    /**
     * Writes all logged OneWire operations to a file.
     * If the file does not exist, it will be created.
     */
    @Override
    public void writeLogsToFile() { 
        if (OneWireLogStack.isEmpty()) { return; }
    
        try {
            java.io.File file = new java.io.File(OneWireFile);
            if (!file.exists()) { 
                file.getParentFile().mkdirs(); 
                file.createNewFile();
            }
            
            FileWriter fileWriter = new FileWriter(file, false); 
            while (!OneWireLogStack.isEmpty()) { 
                String data = OneWireLogStack.pop(); 
                fileWriter.write(data); 
                fileWriter.write(System.lineSeparator());
            }
            fileWriter.close(); 
        } catch (Exception e) {
            System.err.println("Error writing to log file: " + e.getMessage());
        }
    }

    /**
     * Sets the file to which the OneWire logs will be written.
     * @param protocolFile The file to write the logs to.
     */
    @Override
    public void setProtocolFile(String protocolFile) { this.OneWireFile = protocolFile; }

    /**
     * Returns the ID of the port associated with this OneWire instance.
     * @return The port ID.
     */
    @Override
    public int getPortId() { 
        return portId;
    }
}