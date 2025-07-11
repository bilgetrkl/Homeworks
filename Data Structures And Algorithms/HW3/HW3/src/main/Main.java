package main;

import java.util.Scanner;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Iterator;

/**
 * The Main class serves as the entry point of the system.
 * It reads commands from the user, processes them, and interacts with the HWSystem.
 */
public class Main {
    private static HWSystem system;
    private static Scanner scanner;
    private static LinkedList<String> cmdList;
    private static Queue<String> cmdQueue;

    /**
     * The main method initializes the system, reads user commands, and processes them.
     * @param args The command-line arguments, expecting a configuration file and a log directory.
     */
    public static void main(String[] args) {
        if (args.length < 2) {
            System.err.println("Please provide a config file and a log directory.");
            return;
        }
        
        String configFile = args[0]; 
        String logDir = args[1]; 
        system = Configuration.loadConfiguration(configFile, logDir); // loads the config file
        if (system == null) {
            System.err.println("Failed to start system");
            return;
        }

        scanner = new Scanner(System.in);
        cmdList = new LinkedList<String>();
        cmdQueue = new LinkedList<String>();
        boolean running = true;

        while (running) { 
                String input = scanner.nextLine().trim(); // reads the input from the user
                if (input.isEmpty()) continue;

                cmdList.addLast(input); // stores the command in the list

                String[] parts = input.split(" ");
                String command = parts[0];

                if (command.equals("exit")) {
                    running = false;
                    break;
                }
        }

        Iterator<String> iterator = cmdList.iterator();
        while (iterator.hasNext()) {
            cmdQueue.add(iterator.next());
        }
        processCommandsFromQueue(); // processes the commands from the queue
        system.logOperation(); // logs the operations
        scanner.close();
    }

    /**
     * Processes the commands from the queue and executes the corresponding system functions.
     */
    private static void processCommandsFromQueue() {
        while (!cmdQueue.isEmpty()) {
            String cmd = cmdQueue.poll();
            processCommand(cmd);
        }
    }
    /**
     * Processes a given command and executes the corresponding system function.
     * @param cmd The command to be processed.
     */
    private static void processCommand(String cmd) {
        String[] parts = cmd.split(" ");
        String command = parts[0];

        try {
            switch (command) {
                case "turnON": // turns the device on
                    if (parts.length == 2) {
                        system.turnON(Integer.parseInt(parts[1]));
                    } else {
                        System.err.println("Invalid number of arguments");
                        return;
                    }
                    break;

                case "turnOFF": // turns the device off
                    if (parts.length == 2) {
                        system.turnOFF(Integer.parseInt(parts[1]));
                    } else {
                        System.err.println("Invalid number of arguments");
                        return;
                    }
                    break;

                case "addDev": // adds a device to a port 
                    if (parts.length == 4) {
                        system.addDev(parts[1], Integer.parseInt(parts[2]), Integer.parseInt(parts[3]));
                    } else {
                        System.err.println("Invalid number of arguments");
                        return;
                    }
                    break;

                case "rmDev": // removes the device from the port
                    if (parts.length == 2) {
                        system.rmDev(Integer.parseInt(parts[1]));
                    } else {
                        System.err.println("Invalid number of arguments");
                        return;
                    }
                    break;

                case "list": // lists ports or certain types of devices
                    if (parts.length != 2) {
                        System.err.println("Invalid number of arguments for list");
                        return;
                    }
                    String listType = parts[1];
                    switch (listType) {
                        case "ports":
                            system.listPorts();
                            break;
                        case "Sensor":
                            system.listDevices("Sensor");
                            break;
                        case "Display":
                            system.listDevices("Display");
                            break;
                        case "WirelessIO":
                            system.listDevices("WirelessIO");
                            break;
                        case "MotorDriver":
                            system.listDevices("MotorDriver");
                            break;
                        default:
                        System.err.println("Invalid device type. Available: sensor, display, wirelessio, motordriver");
                        return;
                    }
                    break;

                case "readSensor": // reads the sensor device data
                    if (parts.length == 2) {
                        system.readSensor(Integer.parseInt(parts[1]));
                    } else {
                        System.err.println("Invalid number of arguments for readSensor");
                        return;
                    }
                    break;

                case "printDisplay": // prints the data on the display device
                    if (parts.length >= 3) {
                        String text = cmd.substring(cmd.indexOf(parts[2]));
                        system.printDisplay(Integer.parseInt(parts[1]), text);
                    } else {
                        System.err.println("Invalid number of arguments for printDisplay");
                        return;
                    }
                    break;

                case "readWireless": // reads the data from the wireless device
                    if (parts.length == 2) {
                        system.readWireless(Integer.parseInt(parts[1]));
                    } else {
                        System.err.println("Invalid number of arguments for readWireless");
                        return;
                    }
                    break;

                case "writeWireless": // writes the data to the wireless device
                    if (parts.length >= 3) {
                        String data = cmd.substring(cmd.indexOf(parts[2]));
                        system.writeWireless(Integer.parseInt(parts[1]), data);
                    } else {
                        System.err.println("Invalid number of arguments for writeWireless");
                        return;
                    }
                    break;

                case "setMotorSpeed": // sets the motor device speed
                    if (parts.length == 3) {
                        system.setMotorSpeed(Integer.parseInt(parts[1]), Integer.parseInt(parts[2]));
                    } else {
                        System.err.println("Invalid number of arguments for setMotorSpeed");
                        return;
                    }
                    break;

                case "exit": // exits the program
                    System.out.println("Exitting ...");
                    break;

                default:
                System.err.println("Unknown command");
            }
        } catch (NumberFormatException e) {
            System.err.println("Invalid number format");
        } catch (IllegalArgumentException e) {
            System.err.println(e.getMessage());
        } catch (Exception e) {
            System.err.println(e.getMessage());
        }
    }
}