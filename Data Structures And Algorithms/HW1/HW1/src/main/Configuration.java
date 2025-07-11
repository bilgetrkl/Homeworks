package main;

import main.protocols.*;

import java.io.InputStream;
import java.util.Scanner;

public class Configuration { // reads the configuration file and initializes the system
    
    public static HWSystem loadConfiguration(String configFile) {
        try { // gets the config.txt file
            InputStream inputStream = Configuration.class.getResourceAsStream("/main/config.txt");
            
            if (inputStream == null) {
                System.out.println("Config file not found in resources");
                return null;
            }

            Scanner scanner = new Scanner(inputStream);

            String[] protocols = scanner.nextLine().split(","); // reads the line and split by comma to get the protocols

            // reads device limits for next 4 lines
            int maxSensors = parseConfigLine(scanner.nextLine());
            int maxDisplays = parseConfigLine(scanner.nextLine());
            int maxWireless = parseConfigLine(scanner.nextLine());
            int maxMotorDrivers = parseConfigLine(scanner.nextLine());

            HWSystem system = new HWSystem(maxSensors, maxDisplays, maxWireless, maxMotorDrivers); // creates the system with the lines read
            
            for (String protocolName : protocols) { // creates and adds the ports
                Protocol protocol = createProtocol(protocolName.trim());
                if (protocol != null) {
                    system.addPort(protocol);
                } else {
                    System.out.println("Invalid protocol type " + protocolName);
                }
            }
            
            scanner.close();
            inputStream.close();
            return system;
            
        } catch (Exception e) {
            System.out.println("Error loading configuration " + e.getMessage());
            return null;
        }
    }
    
    private static int parseConfigLine(String line) { // parses a config line
        try {
            String[] parts = line.split(":");
            if (parts.length != 2) {
                throw new IllegalArgumentException("Invalid line format " + line);
            }
            return Integer.parseInt(parts[1].trim());
        } catch (Exception e) {
            System.out.println("Error parsing line: " + line);
            return 0;
        }
    }
    
    private static Protocol createProtocol(String protocol) { // creates appropriate protocol
        switch (protocol.toUpperCase()) {
            case "UART":
                return new UART();
            case "SPI":
                return new SPI();
            case "I2C":
                return new I2C();
            case "ONEWIRE":
                return new OneWire();
            default:
                System.out.println("Unknown protocol " + protocol);
                return null;
        }
    }
}