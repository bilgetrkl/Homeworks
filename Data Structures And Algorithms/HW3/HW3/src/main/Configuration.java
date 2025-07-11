package main;

import main.protocols.*;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.Scanner;
import java.util.Iterator;
import java.util.Arrays;

/**
 * Configuration class to read the configuration file and initialize the system.
 */
public class Configuration { 

    /**
     * Loads the configuration from the specified file and initializes the HWSystem.
     * @param configFile the name of the configuration file
     * @param logDir the directory where logs will be stored
     * @return the initialized HWSystem or null if there was an error
     */
    public static HWSystem loadConfiguration(String configFile, String logDir) {
        try {
            String path = System.getProperty("user.dir") + "/" + configFile;
            int portsCount = 0;
            File configFileObj = new File(path);

            if (!configFileObj.exists()) {
                System.err.println("Configuration file not found at: " + path);
                System.err.println("Current path: " + System.getProperty("user.dir"));
                return null;
            }

            InputStream inputStream = new FileInputStream(configFileObj);
            Scanner scanner = new Scanner(inputStream);

            String firstLine = scanner.nextLine().replace("Port Configuration:", "").trim();
            String[] protocols = firstLine.split(","); // reads the line and split by comma to get the protocols

            // reads device limits for next 4 lines
            int maxSensors = parseConfigLine(scanner.nextLine());
            int maxDisplays = parseConfigLine(scanner.nextLine());
            int maxWireless = parseConfigLine(scanner.nextLine());
            int maxMotorDrivers = parseConfigLine(scanner.nextLine());

            HWSystem system = new HWSystem(maxSensors, maxDisplays, maxWireless, maxMotorDrivers); // creates the system with the lines read
            
            Iterator<String> protocolIterator = Arrays.asList(protocols).iterator(); // creates an iterator for protocols
            while (protocolIterator.hasNext()) {
                String protocolName = protocolIterator.next();
                Protocol protocol = createProtocol(protocolName.trim(), portsCount++); // creates the protocol
                if (protocol != null) {
                    system.addPort(protocol, logDir);
                } else {
                    System.err.println("Invalid protocol type " + protocolName);
                }
            }
            
            scanner.close();
            inputStream.close();
            return system;
            
        } catch (Exception e) {
            System.err.println("Error loading configuration " + e.getMessage());
            return null;
        }
    }

    /**
    * Parses a line from the configuration file to extract the value.
    * @param line the line to parse
    * @return the parsed integer value or 0 if there was an error
    */
    private static int parseConfigLine(String line) {
        try {
            String[] parts = line.split(":");
            if (parts.length != 2) {
                System.err.println("Invalid line format " + line);
            }
            return Integer.parseInt(parts[1].trim());
        } catch (Exception e) {
            System.err.println("Error parsing line: " + line);
            return 0;
        }
    }

    /**
    * Creates a protocol object based on the protocol name.
    * @param protocol the name of the protocol
    * @param portId the port ID
    * @return the created Protocol object or null if the protocol is unknown
    */
    private static Protocol createProtocol(String protocol, int portId) {
        switch (protocol) {
            case "UART":
                return new UART(portId);
            case "SPI":
                return new SPI(portId);
            case "I2C":
                return new I2C(portId);
            case "OneWire":
                return new OneWire(portId);
            default:
                System.err.println("Unknown protocol " + protocol);
                return null;
        }
    }
}