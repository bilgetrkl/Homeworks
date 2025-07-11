package main;

import java.util.Scanner;

public class Main {
    private static HWSystem system;
    private static Scanner scanner;

    public static void main(String[] args) {
        system = Configuration.loadConfiguration("config.txt"); // loads the config.txt file
        if (system == null) {
            System.out.println("Failed to start system");
            return;
        }

        scanner = new Scanner(System.in);
        boolean running = true;

        System.out.println("System started. Please write a command.");

        while (running) { 
            try {

                String input = scanner.nextLine().trim(); // reads the input from the user
                if (input.isEmpty()) continue;

                String[] parts = input.split(" ");
                String command = parts[0];

                switch (command) {
                    case "turnON": // turns on the device
                        if (parts.length == 2) {
                            system.turnON(Integer.parseInt(parts[1]));
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                    case "turnOFF": // turns off the device
                        if (parts.length == 2) {
                            system.turnOFF(Integer.parseInt(parts[1]));
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                    case "addDev": // adds a device to a port that fits its compatible protocol
                        if (parts.length == 4) {
                            system.addDev(parts[1], 
                                          Integer.parseInt(parts[2]), 
                                          Integer.parseInt(parts[3]));
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                    case "rmDev":
                        if (parts.length == 2) { // removes a device from a port
                            system.rmDev(Integer.parseInt(parts[1]));
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                        case "list": // lists ports or certain type of devices
                        if (parts.length != 2) {
                            throw new IllegalArgumentException("Invalid number of arguments for list");
                        }
                        
                        String listType = parts[1].toLowerCase();
                        switch (listType) {
                            case "ports":
                                System.out.println("list of ports");
                                system.listPorts();
                                break;
                            case "sensor":
                                System.out.println("list of sensors");
                                system.listDevices("sensor");
                                break;
                            case "display":
                                System.out.println("list of displays");
                                system.listDevices("display");
                                break;
                            case "wireless":
                                System.out.println("list of wireless");
                                system.listDevices("wireless");
                                break;
                            case "motordriver":
                                System.out.println("list of motordrivers");
                                system.listDevices("motor");
                                break;
                            default:
                                throw new IllegalArgumentException("Invalid device type. Avaliable: sensor, display, wireless, motordriver");
                        }
                        break;

                    case "readSensor": // read the sensor data
                        if (parts.length == 2) {
                            system.readSensor(Integer.parseInt(parts[1]));
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments for readSensor");
                        }
                        break;

                    case "printDisplay": // prints the data on the display
                        if (parts.length >= 3) {
                            String text = input.substring(input.indexOf(parts[2]));
                            system.printDisplay(Integer.parseInt(parts[1]), text);
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments for printDisplay");
                        }
                        break;

                    case "readWireless": // reads the data from the wireless device
                        if (parts.length == 2) {
                            system.readWireless(Integer.parseInt(parts[1]));
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments for readWireless");
                        }
                        break;

                    case "writeWireless": // writes the data to the wireless device
                        if (parts.length >= 3) {
                            String data = input.substring(input.indexOf(parts[2]));
                            system.writeWireless(Integer.parseInt(parts[1]), data);
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments for writeWireless");
                        }
                        break;

                    case "setMotorSpeed": // sets the motor speed
                        if (parts.length == 3) {
                            system.setMotorSpeed(Integer.parseInt(parts[1]), 
                                              Integer.parseInt(parts[2]));
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments for setMotorSpeed");
                        }
                        break;

                    case "exit": // exits the program
                        running = false;
                        break;

                    default:
                        throw new IllegalArgumentException("Unknown command");
                }
            } catch (NumberFormatException e) {
                System.out.println("Invalid number format");
            } catch (IllegalArgumentException e) {
                System.out.println(e.getMessage());
            } catch (Exception e) {
                System.out.println(e.getMessage());
            }
        }

        scanner.close();
    }
}