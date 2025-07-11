package main;

import main.core.Device;
import main.core.Port;
import main.core.State;
import main.devices.displays.Display;
import main.devices.motors.MotorDriver;
import main.devices.sensors.Sensor;
import main.devices.wireless.WirelessIO;
import main.protocols.Protocol;

import java.util.ArrayList;
import java.util.Iterator;

/**
 * Represents the hardware system that manages ports and connected devices.
 * This class provides functionalities to add, remove, and control various devices
 * such as sensors, displays, motor drivers, and wireless communication modules.
 */
public class HWSystem {
    private ArrayList<Port> ports;
    private ArrayList<Display> displays;
    private ArrayList<MotorDriver> motordrivers; 
    private ArrayList<WirelessIO> wireless;
    private ArrayList<Sensor> sensors;

    private int maxSensors, maxDisplays, maxWireless, maxMotorDrivers;

    /**
     * Constructs an HWSystem with specified maximum device limits.
     * @param maxSensors     Maximum number of sensors allowed.
     * @param maxDisplays    Maximum number of displays allowed.
     * @param maxWireless    Maximum number of wireless devices allowed.
     * @param maxMotorDrivers Maximum number of motor drivers allowed.
     */
    public HWSystem(int maxSensors, int maxDisplays, int maxWireless, int maxMotorDrivers) { 
        this.maxSensors = maxSensors;
        this.maxDisplays = maxDisplays;
        this.maxWireless = maxWireless;
        this.maxMotorDrivers = maxMotorDrivers;

        this.ports = new ArrayList<>();
        this.displays = new ArrayList<>();
        this.motordrivers = new ArrayList<>();
        this.wireless = new ArrayList<>();
        this.sensors = new ArrayList<>();

        while (sensors.size() < maxSensors) { // initializes the sensors list
            sensors.add(null);
        }
        while (displays.size() < maxDisplays) { // initializes the displays list
            displays.add(null);
        }
        while (wireless.size() < maxWireless) { // initializes the wireless list
            wireless.add(null);
        }
        while (motordrivers.size() < maxMotorDrivers) { // initializes the motor drivers list
            motordrivers.add(null);
        }
    }

    /**
     * Adds a new port to the system.
     * @param protocol The protocol associated with the port.
     * @param logDir   Directory to store log files for the port.
     */
    public void addPort(Protocol protocol, String logDir) { 
        ports.add(new Port(protocol, ports.size(), logDir)); 
    }

    /**
     * Adds a device to a specified port if it is available.
     * @param devType The type of device to be added.
     * @param portId  The port ID where the device should be connected.
     * @param devId   The unique device ID.
     * @return true if the device is successfully added, false otherwise.
     */
    public boolean addDev(String devType, int portId, int devId) {
        if (!isValidDeviceName(devType)) { // checks if the device name is valid
            System.err.println("Invalid device name");
            return false;
        }
        if (portId < 0 || portId >= ports.size()) { // checks if the port ID is valid
            System.err.println("Invalid port ID");
            return false;
        }
        if (ports.get(portId).isOccupied()) { // checks if the port is occupied
            System.err.println("Port is occupied");
            return false;
        }
        if (!isValidDeviceId(devType, devId)) { // checks if the device ID is valid
            System.err.println("Invalid device ID for " + devType);
            return false;
        }
        if (deviceExists (devId, devType)) { // checks if the device already exists
            System.err.println("Device ID already exists");
            return false;
        }
        if (devType.equals("DHT11") || devType.equals("BME280") || devType.equals("MPU6050") || devType.equals("GY951")) {
            Sensor sensor = Sensors.createSensor(devType, ports.get(portId).getProtocol(), devId); // creates a sensor
            if (sensor == null) {
                return false;
            }
            ports.get(portId).setDevice(sensor);
            sensors.set(devId, sensor);
            System.out.println("Device added.");
            return true;
        } else if (devType.equals("LCD") || devType.equals("OLED")) {
            Display display = Displays.createDisplay(devType, ports.get(portId).getProtocol(), devId); // creates a display 
            if (display == null) {
                return false;
            }
            
            ports.get(portId).setDevice(display);
            displays.set(devId, display);

            System.out.println("Device added.");
            return true;
        } else if (devType.equals("Bluetooth") || devType.equals("Wifi")) {
            WirelessIO wirelessIO = Wireless.createWireless(devType, ports.get(portId).getProtocol(), devId); // creates a wireless
            if (wirelessIO == null) {
                return false;
            }
            ports.get(portId).setDevice(wirelessIO);
            wireless.set(devId, wirelessIO);
            System.out.println("Device added.");
            return true;
        } else if (devType.equals("PCA9685") || devType.equals("SparkFunMD")) {
            MotorDriver motordriver = MotorDrivers.createMotor(devType, ports.get(portId).getProtocol(), devId); // creates a motor driver 
            if (motordriver == null) {
                return false;
            }
            ports.get(portId).setDevice(motordriver);
            motordrivers.set(devId, motordriver);
            System.out.println("Device added.");
            return true;
        }
        else {
            System.err.println("Invalid device type");
            return false;
        }
    }

    /**
     * Removes a device from a port.
     * @param portId The ID of the port where the device is connected.
     * @return true if the device is successfully removed, false otherwise.
     */
    public boolean rmDev(int portId) {
        if (portId < 0 || portId >= ports.size()) { // checks if the port ID is valid
            System.err.println("Invalid port ID: " + portId);
            return false;
        }
        
        Port port = ports.get(portId);
        if (!port.isOccupied()) { // checks if the port is occupied
            System.err.println("No device connected to port " + portId);
            return false;
        } 
        Device device = port.getDevice();
        if (device.getState() == State.ON) { // checks if the device is ON
            System.err.println("Cannot remove device while it's ON");
            return false;
        }
        removeDeviceFromList(device);
        port.setDevice(null);
        System.out.println("Device removed.");
        return true;
    }

    /**
     * Turns ON a device connected to a specific port.
     * @param portId The port ID where the device is connected.
     */
    public void turnON(int portId) {
        if (!isValidPort(portId)) { // checks if the port ID is valid
            System.err.println("Invalid port ID");
            return;
        }
    
        Port port = ports.get(portId);
        if (!port.isOccupied()) { // checks if the port is occupied
            System.err.println("No device connected to port " + portId);
            return;
        }
        Device device = port.getDevice();
        device.turnOn();
    }

    /**
     * Turns OFF a device connected to a specific port.
     * @param portId The port ID where the device is connected.
     */
    public void turnOFF(int portId) {
        if (isValidPort(portId) && ports.get(portId).isOccupied()) { // checks if the port ID is valid and the port is occupied 
            ports.get(portId).getDevice().turnOff();
        }
    }

    /**
     * Reads data from a sensor with the specified device ID.
     * @param devId The ID of the sensor to read.
     */
    public void readSensor(int devId) {
        Sensor sensor = getSensorById(devId);
        if (sensor == null) {
            System.err.println("There is no sensor with devId " + devId);
            return;
        }
        if (sensor.getState() == State.ON) {
            System.out.printf("%s %s: %s%n",
                sensor.getName(),
                sensor.getDevType(),
                sensor.data2String());
        } else {
            System.err.println("Sensor is OFF");
            return;
        }
    }

    /**
     * Prints data on a display device with the specified ID.
     * @param devId The ID of the display device.
     * @param data The data to be printed on the display.
     */
    public void printDisplay(int devId, String data) {
        Display display = getDisplayById(devId);
        if (display == null) {
            System.err.println("There is no display with devId " + devId);
            return;
        }
        String devName = display.getName();
        display.printData(devName, data);
    } 

    /**
     * Sets the motor speed of a motor driver device.
     * @param devId The ID of the motor driver.
     * @param speed The speed value to set.
     */
    public void setMotorSpeed(int devId, int speed) { 
        MotorDriver motordriver = getMotorDriverById(devId);
        if (motordriver == null) {
            System.err.println("There is no motor driver with devId " + devId);
            return;
        }
        String devName = motordriver.getName();
        motordriver.setMotorSpeed(devName, speed);

    }
    /**
     * Writes data to a wireless device.
     * @param devId The ID of the wireless device.
     * @param data The data to be sent.
     */
    public void writeWireless(int devId, String data) { 
        WirelessIO wirelessIO = getWirelessById(devId);
        if (wirelessIO == null) {
            System.err.println("There is no wireless device with devId " + devId);
            return;
        }
        String devName = wirelessIO.getName();
        wirelessIO.sendData(devName, data);
    }

    /**
     * Reads data from a wireless device.
     * @param devId The ID of the wireless device.
     */
    public void readWireless(int devId) {
        WirelessIO wirelessIO = getWirelessById(devId);
        if (wirelessIO == null) {
            System.err.println("There is no wireless device with devId " + devId);
            return;
        }
        String devName = wirelessIO.getName();
        wirelessIO.recvData(devName);
    }

    /**
     * Checks if a device with the specified ID already exists in the system.
     * @param devId The ID of the device.
     * @param type The type of the device.
     * @return true if a device with the given ID and type exists, false otherwise.
     */
    private boolean deviceExists(int devId, String type) { 
        if (type.equals("DHT11") || type.equals("BME280") || type.equals("MPU6050") || type.equals("GY951")) {
            if (sensors.get(devId) != null) { return true; }
            return false;
        }
        else if (type.equals("LCD") || type.equals("OLED")) {
            if (displays.get(devId) != null) { return true; }
            return false;
        }
        else if (type.equals("Bluetooth") || type.equals("Wifi")) {
            if (wireless.get(devId) != null) { return true; }
            return false;
        }
        else if (type.equals("PCA9685") || type.equals("SparkFunMD")) {
            if (motordrivers.get(devId) != null) { return true; }
            return false;
        }
        return false;
    }

    /**
     * Removes a device from its respective list.
     * @param device The device to be removed.
     */
    private void removeDeviceFromList(Device device) {
        int index = device.getDevId();
        if (device instanceof Sensor) {
            sensors.set(index, null);
        } else if (device instanceof Display) {
            displays.set(index, null);
        } else if (device instanceof WirelessIO) {
            wireless.set(index, null);
        } else if (device instanceof MotorDriver) {
            motordrivers.set(index, null);
        }
    }

    /**
     * Lists all ports with their associated devices and statuses.
     */
    public void listPorts() {
        System.out.println("list of ports:");
        Iterator<Port> iterator = ports.iterator();
        int i = 0;
        while (iterator.hasNext()) {
            Port port = iterator.next();
            if (port.isOccupied()) {
                Device dev = port.getDevice();
                System.out.printf("%d %s occupied %s %s %d %s%n",
                    i,
                    port.getProtocol().getProtocolName(),
                    dev.getName(),
                    dev.getDevType(),
                    dev.getDevId(),
                    dev.getState());
            } else {
                System.out.printf("%d %s empty%n",
                    i,
                    port.getProtocol().getProtocolName());
            }
            i++;
        }
    }

    /**
     * Lists all devices of a specific type.
     * @param deviceType The type of devices to list.
     */
    public void listDevices(String deviceType) {
        System.out.println("list of " + deviceType + "s:");
        switch (deviceType) {
            case "Sensor": 
                Iterator<Sensor> sensorIterator = sensors.iterator();
                while (sensorIterator.hasNext()) { // iterates over the sensors
                    Sensor device = sensorIterator.next();
                    if (device != null) {
                        printDeviceInfo(device);
                    }
                }
                break;
            case "Display":
                Iterator<Display> displayIterator = displays.iterator();
                while (displayIterator.hasNext()) { // iterates over the displays
                    Display device = displayIterator.next();
                    if (device != null) {
                        printDeviceInfo(device);
                    }
                }
                break;
            case "WirelessIO":
                Iterator<WirelessIO> wirelessIterator = wireless.iterator();
                while (wirelessIterator.hasNext()) { // iterates over the wireless devices
                    WirelessIO device = wirelessIterator.next();
                    if (device != null) {
                        printDeviceInfo(device);
                    }
                }
                break;
            case "MotorDriver":
                Iterator<MotorDriver> motorDriverIterator = motordrivers.iterator();
                while (motorDriverIterator.hasNext()) { // iterates over the motor drivers
                    MotorDriver device = motorDriverIterator.next();
                    if (device != null) {
                        printDeviceInfo(device);
                    }
                }
                break;
            default:
                System.out.println("Invalid device type");
        }
    }

    /**
     * Prints the device information.
     * @param device The device whose information is to be printed.
     */
    private void printDeviceInfo(Device device) {
        System.out.printf("%s %d %d %s%n", 
            device.getName(),
            device.getDevId(),
            device.getProtocol().getPortId(),
            device.getProtocol().getProtocolName());
    }

    /**
     * Checks if the given device ID is valid for the specified device type.
     * @param deviceType The type of the device.
     * @param devId The ID of the device.
     * @return true if the device ID is valid, false otherwise.
     */
    private boolean isValidDeviceId(String deviceType, int devId) {
        switch (deviceType) {

                case "DHT11":
                    return devId >= 0 && devId < maxSensors;
                case "BME280":
                    return devId >= 0 && devId < maxSensors;
                case "MPU6050":
                    return devId >= 0 && devId < maxSensors;
                case "GY951":
                    return devId >= 0 && devId < maxSensors;
                case "LCD":
                    return devId >= 0 && devId < maxDisplays;
                case "OLED":
                    return devId >= 0 && devId < maxDisplays;
                case "Bluetooth":
                    return devId >= 0 && devId < maxWireless;
                case "Wifi":
                    return devId >= 0 && devId < maxWireless;
                case "PCA9685":
                    return devId >= 0 && devId < maxMotorDrivers;
                case "SparkFunMD":
                    return devId >= 0 && devId < maxMotorDrivers;
                default:
                    return false;
        }
    }

    /**
     * Checks if the given device type is valid.
     * @param deviceType The type of the device.
     * @return true if the device type is valid, false otherwise.
     */
    private boolean isValidDeviceName(String deviceType) {
        switch (deviceType) {

                case "DHT11":
                case "BME280":
                case "MPU6050":
                case "GY951":
                case "LCD":
                case "OLED":
                case "Bluetooth":
                case "Wifi":
                case "PCA9685":
                case "SparkFunMD":
                    return true;
                default:
                    return false;
        }
    }
    
    /**
     * Checks if the given port ID is valid.
     * @param portId The ID of the port.
     * @return true if the port ID is valid, false otherwise.
     */
    private boolean isValidPort(int portId) { return portId >= 0 && portId < ports.size(); }

    /**
     * Retrieves a display device by its ID.
     * @param devId The ID of the display device.
     * @return The display device if found, or null if the ID is invalid.
     */
    private Display getDisplayById(int devId) {
        if (devId < 0 || devId >= maxDisplays) { return null; }
        return displays.get(devId);
    }

    /**
     * Retrieves a sensor device by its ID.
     * @param devId The ID of the sensor device.
     * @return The sensor device if found, or null if the ID is invalid.
     */
    private Sensor getSensorById(int devId) { 
        if (devId < 0 || devId >= maxSensors) { return null; }
        return sensors.get(devId);
    }

    /**
     * Retrieves a wireless device by its ID.
     * @param devId The ID of the wireless device.
     * @return The wireless device if found, or null if the ID is invalid.
     */    
    private WirelessIO getWirelessById(int devId) { 
        if (devId < 0 || devId >= maxWireless) { return null; }
        return wireless.get(devId);
    }

    /**
     * Retrieves a motor driver by its ID.
     * @param devId The ID of the motor driver.
     * @return The motor driver if found, or null if the ID is invalid.
     */ 
    private MotorDriver getMotorDriverById(int devId) {
        if (devId < 0 || devId >= maxMotorDrivers) { return null; }
        return motordrivers.get(devId);
    }

    /**
     * Writes all operations to log files.
     */
    public void logOperation(){
        Iterator<Port> iterator = ports.iterator();
        while (iterator.hasNext()) {
            Port port = iterator.next();
            port.getProtocol().writeLogsToFile();
        }
    }
}