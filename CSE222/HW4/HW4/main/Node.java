package main;

import java.util.ArrayList;
import java.util.List;

/**
 * Node class represents a node in the planet system.
 * It can be a star, planet or a satellite and contains sensor data.
 */

public class Node {
    private String name;
    private String type;
    private SensorData sensorData;
    private List<Node> children;
    private boolean planetFlag;

    /**
     * Constructor to initialize the node.
     * @param name Name of the node
     * @param type Type of the node (star, planet, satellite)
     * @param sensorData Sensor data associated with the node
     * @param planetFlag If a planet has one planet or not
     */

    public Node(String name, String type, SensorData sensorData, boolean planetFlag) {
        this.planetFlag = planetFlag;
        this.name = name;
        this.type = type;
        this.sensorData = sensorData;
        this.children = new ArrayList<>();
    }

    /**
     * Getter for name.
     * @return name of the node.
     */
    public String getName() { return name; }

    /**
     * Getter for type.
     * @return type of the node.
     */
    public String getType() { return type; }

    /**
     * Getter for sensor data.
     * @return sensor data of the node.
     */
    public SensorData getSensorData() { return sensorData; }

    /**
     * Adds a child node to the tree.
     * @param child Child node to be added.
     */
    public void addChildren(Node child) { children.add(child); }

    /**
     * Getter for children.
     * @return list of child nodes.
     */
    public List<Node> getChildren() { return children; }

    /**
     * Getter for planet flag.
     * @return the planet flag.
     */
    public boolean getPlanetFlag() { return planetFlag; }

    /**
     * Setter for planet flag.
     * @param planetFlag the planet flag to set.
     */
    public void setPlanetFlag(boolean planetFlag) { this.planetFlag = planetFlag; }
}