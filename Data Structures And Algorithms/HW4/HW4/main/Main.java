package main;

import java.util.Scanner;
import java.util.List;
import java.util.Stack;


/**
 * Entry point for the Deep Space Planetary System Analysis tool.
 * Provides a terminal UI for interacting with the PlanetSystem tree.
 */
public class Main {
    private static Scanner scanner;

    /**
     * Main method that initializes and runs the command-line interface
     * for building and querying a planetary system.
     * @param args Command-line arguments (not used)
     */
    public static void main(String[] args) {

        PlanetSystem system = new PlanetSystem(); 
        scanner = new Scanner(System.in);
        boolean running = true;
        
        System.out.println("System has started. Enter command: ");

        while(running) {
            try {
                
                String input = scanner.nextLine().trim(); // reads the input from the user
                if (input.isEmpty()) continue;

                String[] parts = input.split(" ");
                String command = parts[0];

                switch(command) {
                    case "createPlanetSystem": // creates a new planet system
                        if(parts.length == 6) {
                            system.createPlanetSystem(parts[1], // name of the star
                                                      Double.parseDouble(parts[2]), // temperature
                                                      Double.parseDouble(parts[3]), // pressure
                                                      Double.parseDouble(parts[4]), // humidity
                                                      Double.parseDouble(parts[5])); // radiation
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                    case "addPlanet": // adds a planet to the system
                        if (parts.length == 7) {
                            system.addPlanet(parts[1], // name of the planet
                                             parts[2], // parent name
                                             Double.parseDouble(parts[3]), // temperature
                                             Double.parseDouble(parts[4]), // pressure
                                             Double.parseDouble(parts[5]), // humidity
                                             Double.parseDouble(parts[6])); // radiation
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                    case "addSatellite": // adds a sattelite to the system
                        if (parts.length == 7) {
                            system.addSatellite(parts[1], // name of the satellite
                                                parts[2], // parent name
                                                Double.parseDouble(parts[3]), // temperature
                                                Double.parseDouble(parts[4]), // pressure
                                                Double.parseDouble(parts[5]), // humidity
                                                Double.parseDouble(parts[6])); // radiation
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                    case "findRadiationAnomalies": // finds the radiation anomalies
                        if (parts.length == 2) { 
                            List <Node> anomalies = system.findRadiationAnomalies(Double.parseDouble(parts[1]));
                            if (anomalies == null) {
                                System.out.println("Planet system is not created yet");
                            }
                            if (anomalies.isEmpty()) {
                                System.out.println("No radiation anomalies found");
                            } else {
                                System.out.println("Radiation anomalies: ");
                                for (Node node : anomalies) {
                                    System.out.println(node.getType() + " " + node.getName() + " Radiation: " + node.getSensorData().getRadiation() + " Sieverts");
                                }
                            }

                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        break;

                    case "getPathTo": // represents a path from the root to the given target
                        if (parts.length == 2) {
                            Stack <String> path = system.getPathTo(parts[1]);
                            if (path == null) {
                                System.out.println("Planet system is not created yet");
                            } else if (path.isEmpty()) {
                                System.out.println("No path found to the given target");
                            } else {
                                System.out.println("Path to " + parts[1] + ": ");
                                for (String node : path) {
                                    System.out.println(node);
                                }
                            }
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments");
                        }
                        
                        break;

                    case "printMissionReport": 
                        if (parts.length == 1) {
                            System.out.println("Mission report: ");
                            system.printMissionReport(); // prints all tree data
                        } else if (parts.length == 2) {
                            System.out.println("Mission report for " + parts[1] + ": ");
                            system.printMissionReportNode(parts[1]); // prints the data of the given node
                        } else {
                            throw new IllegalArgumentException("Invalid number of arguments for readSensor");
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