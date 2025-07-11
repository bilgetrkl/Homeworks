package main;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;


/**
 * PlanetSystem class manages a hierarchical tree structure
 * representing a planetary system with a star, planets, and satellites.
 * Provides operations to build the tree, search, traverse, and print reports.
 */
public class PlanetSystem {

    private Node root = null;
    private boolean systemFlag = false;
    private boolean starFlag = false;

    /**
     * Creates the planetary system with a star as the root node.
     * @param name      Name of the star
     * @param temp      Temperature in Kelvin
     * @param pressure  Pressure in Pascals
     * @param humidity  Humidity in percentage (must be 0 for a star)
     * @param radiation Radiation in Sieverts
     */
    public void createPlanetSystem(String name, Double temp, Double pressure, Double humidity, Double radiation) {
        if (systemFlag == true) {
            System.out.println("Planet system already created");
            return;
        }
        if (temp < 0 || pressure < 0 || humidity < 0 || radiation < 0) {
            System.out.println("Sensor data cannot be negative.");
            return;
        }
        if (humidity > 0) {
            System.out.println("Stars do not have humidity");
            return;
        } else {
            root = new Node(name, "Star", new SensorData(temp, pressure, humidity, radiation), false);
            System.out.println("Planet system created.");
            systemFlag = true;
        }
    }

    /**
     * Adds a new planet under an existing node in the planetary system.
     * @param name      Name of the planet
     * @param parentName Name of the parent node.
     * @param temp      Temperature in Kelvin
     * @param pressure  Pressure in Pascals
     * @param humidity  Humidity in percentage
     * @param radiation Radiation in Sieverts
     */
    public void addPlanet(String name, String parentName, Double temp, Double pressure, Double humidity, Double radiation) {
        if (root == null) {
            System.out.println("Planet system is not created yet.");
            return;
        }
        if (temp < 0 || pressure < 0 || humidity < 0 || radiation < 0) {
            System.out.println("Sensor data cannot be negative.");
            return;
        }
        Node child = new Node(name, "Planet", new SensorData(temp, pressure, humidity, radiation), false);
        Node planet = findNodeByName(name);
        Node parent = findNodeByName(parentName);
        if (parent != null) {
            if (parent.getType() == "Satellite") {
                System.out.println("Satellites do not have planets.");
                return;
            }
            if (planet == null) {
                if (parent.getType().equals("Star") && starFlag == true) {
                    System.out.println("Stars can only have one planet.");
                    return;
                }
                if (parent.getPlanetFlag() == true) {
                    System.out.println("Planets can only have one planet.");
                    return;
                }
                parent.addChildren(child);
                if (parent.getType().equals("Star") && starFlag == false) {
                    starFlag = true;
                } else if (parent.getType().equals("Planet") && parent.getPlanetFlag() == false) {
                    parent.setPlanetFlag(true);
                }
                System.out.println("Planet \"" + name + "\" added under \"" + parentName + "\".");
            } else {
                System.out.println("\"" + name + "\" already exists in the planet system.");
            }
        } else {
            System.out.println("\"" + parentName + "\" not found in the planet system.");
        }
    }

    /**
     * Adds a satellite under a specified parent node.
     * @param name      Name of the satellite
     * @param parentName Name of the parent node
     * @param temp      Temperature in Kelvin
     * @param pressure  Pressure in Pascals
     * @param humidity  Humidity in percentage
     * @param radiation Radiation in Sieverts
     */
    public void addSatellite(String name, String parentName, Double temp, Double pressure, Double humidity, Double radiation) {
        if (root == null) {
            System.out.println("Planet system is not created yet");
            return;
        }
        if (temp < 0 || pressure < 0 || humidity < 0 || radiation < 0) {
            System.out.println("Sensor data cannot be negative.");
            return;
        }
        Node child = new Node(name, "Satellite", new SensorData(temp, pressure, humidity, radiation), false);
        Node satellite = findNodeByName(name);
        Node parent = findNodeByName(parentName);

        if (parent != null) {
            if (parent.getType().equals("Satellite")) {
                System.out.println("Satellites do not have satellites.");
                return;
            }
            if (parent.getType().equals("Star")) {
                System.out.println("Stars do not have sattelites.");
                return;
            }
            if (satellite == null) {
                parent.addChildren(child);
                System.out.println("Satellite \"" + name + "\" added under \"" + parentName + "\".");
            } else {
                System.out.println("\"" + name + "\" already exists in the planet system.");
            }
        } else {
            System.out.println("\"" + parentName + "\" not found in the planet system.");
        }
    }

    /**
     * Recursively finds all nodes where radiation exceeds a given threshold.
     * @param threshold Radiation threshold
     * @return List of nodes with radiation values above the threshold
     */
    public List<Node> findRadiationAnomalies(Double threshold) {
        List<Node> nodes = new ArrayList<>();
        if (root != null) {
            recursiveTraverse(root, threshold, nodes);
            return nodes;
        } else { return null; }
    }

    /**
     * Returns the path from the root to the given node.
     * @param planetName Name of the node to find
     * @return A Stack representing the path from the root to the node
     */
    public Stack<String> getPathTo(String planetName) {
        Stack<Node> path = new Stack<>();
        Stack <String> pathString = new Stack<>();
        if (root != null) {
            findPath(root, planetName, path);
            for (Node node : path) {
                pathString.push(node.getType() + " " + node.getName());
            }
            return pathString;
        } else {
            return null;
        }   
    }

    /**
     * Prints the full tree from the root to all descendant nodes.
     */
    public void printMissionReport() {
        if (root != null) {
            traverseAndPrint(root);
        } else {
            System.out.println("Planet system is not created yet");
        }
    }

    /**
     * Prints the sensor and identity data of a specific node by name.
     * @param nodeName Name of the node to report
     */
    public void printMissionReportNode(String nodeName) {
        if (root != null) {
            findAndPrint(root, nodeName);
        } else {
            System.out.println("Planet system is not created yet");
        }
    }

    /**
     * Searches for a node with the specified name using DFS.
     * @param nodeName Name of the node to find
     * @return Node object if found, null otherwise
     */
    private Node findNodeByName(String nodeName) {
        if (root == null) return null;

        Stack<Node> stack = new Stack<>();
        stack.push(root);

        while (!stack.isEmpty()) {
            Node current = stack.pop();

            if (current.getName().equals(nodeName)) { return current; }

            for (Node child : current.getChildren()) { stack.push(child); }
        }
        return null;
    }

    /**
     * Recursive method to traverse the tree and collect radiation anomalies.
     * @param node      Current node
     * @param threshold Radiation threshold
     * @param nodes     List to collect matching nodes
     */
    private void recursiveTraverse(Node node, Double threshold, List <Node> nodes) {
        if (node == null) return;

        if (node.getSensorData().getRadiation() > threshold) { nodes.add(node); }
    
        for (Node child : node.getChildren()) { recursiveTraverse(child, threshold, nodes); }
    }

    /**
     * Recursive helper to find the path to a target node.
     * @param current    Current node
     * @param targetName Target node name
     * @param path       Stack to track the path
     * @return True if path is found, false otherwise
     */
    private boolean findPath(Node current, String targetName, Stack<Node> path) {
        if (current == null) return false;
    
        path.push(current); 
    
        if (current.getName().equals(targetName)) { return true; }
    
        for (Node child : current.getChildren()) {
            if (findPath(child, targetName, path)) { return true; }
        }
        path.pop();
        return false; 
    }

    /**
     * Recursively prints all nodes from the given node downward.
     * @param node Node to start from
     */
    private void traverseAndPrint(Node node) {
        if (node == null) return;

        System.out.println(node.getType() + " " + node.getName() + " Temperature: " + node.getSensorData().getTemperature() + " Kelvin" +
                ", Pressure: " + node.getSensorData().getPressure() + " Pascals" +
                ", Humidity: " + node.getSensorData().getHumidity() + "%" +
                ", Radiation: " + node.getSensorData().getRadiation() + " Sieverts");

        for (Node child : node.getChildren()) { traverseAndPrint(child); }
    }

    /**
     * Searches for and prints the node with the given name.
     * @param node     Current node to check
     * @param nodeName Name of the node to find and print
     */
    private void findAndPrint(Node node, String nodeName) {
        if (node == null) return;

        if (node.getName().equals(nodeName)) {
            System.out.println(node.getType() + " " + node.getName() + " Temperature: " + node.getSensorData().getTemperature() + " Kelvin" +
                    ", Pressure: " + node.getSensorData().getPressure() + " Pascals" +
                    ", Humidity: " + node.getSensorData().getHumidity() + "%" +
                    ", Radiation: " + node.getSensorData().getRadiation() + " Sieverts");
            return;
        }

        for (Node child : node.getChildren()) { findAndPrint(child, nodeName); }
    }
}