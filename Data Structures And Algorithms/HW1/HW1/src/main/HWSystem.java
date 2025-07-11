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

public class HWSystem {
    private ArrayList<Port> ports;
    private ArrayList<Display> displays;
    private ArrayList<MotorDriver> motordrivers; 
    private ArrayList<WirelessIO> wireless;
    private ArrayList<Sensor> sensors;

    private ArrayList<Integer> displayIds;
    private ArrayList<Integer> motorDriverIds;
    private ArrayList<Integer> wirelessIds;
    private ArrayList<Integer> sensorIds;

    private int maxSensors, maxDisplays, maxWireless, maxMotorDrivers;
    private int currentSensors, currentDisplays, currentWireless, currentMotorDrivers;

    public HWSystem(int maxSensors, int maxDisplays, int maxWireless, int maxMotorDrivers) { // constructor
        this.ports = new ArrayList<>();
        this.displays = new ArrayList<>();
        this.motordrivers = new ArrayList<>();
        this.wireless = new ArrayList<>();
        this.sensors = new ArrayList<>();

        this.sensorIds = new ArrayList<>();
        this.displayIds = new ArrayList<>();
        this.motorDriverIds = new ArrayList<>();
        this.wirelessIds = new ArrayList<>();
    
        this.maxSensors = maxSensors;
        this.maxDisplays = maxDisplays;
        this.maxWireless = maxWireless;
        this.maxMotorDrivers = maxMotorDrivers;

    }

    public void addPort(Protocol protocol) { // adds a port to the system
        ports.add(new Port(protocol, ports.size()));  // starting from 0
    }

    public boolean addDev(String deviceType, int portId, int devId) { // adds a device to a port that fits its compatible protocol
        
        if (!isValidDeviceName(deviceType)) {
            System.out.println("Invalid device name");
            return false;
        }
        if (portId < 0 || portId >= ports.size()) {
            System.out.println("Invalid port ID");
            return false;
        }

        if (ports.get(portId).isOccupied()) {
            System.out.println("Port is occupied");
            return false;
        }

        if (!isValidDeviceId(deviceType, devId)) {
            System.out.println("Invalid device ID for " + deviceType);
            return false;
        }

        if (deviceExists (devId, deviceType)) {
            System.out.println("Device ID already exists");
            return false;
        }

        Device device = Devices.createDevice(deviceType, ports.get(portId).getProtocol()); // creates a device based on the type
        if (device == null) {
            return false;
        }
        
        if (!checkDeviceLimit(device)) { // checks if the device limit is reached
            System.out.println("Device limit reached"); 
            return false;
        }
        ports.get(portId).setDevice(device);
        addDeviceToList(device, devId);
        System.out.println("Device added successfully");
        return true;
    }

    public boolean rmDev(int portId) { // removes a device from a port
        if (portId < 0 || portId >= ports.size()) {
            System.out.println("Invalid port ID: " + portId);
            return false;
        }
        
        Port port = ports.get(portId);
        if (!port.isOccupied()) {
            System.out.println("No device connected to port " + portId);
            return false;
        } 
        Device device = port.getDevice();
        if (device.getState() == State.ON) {
            System.out.println("Cannot remove device while it's ON");
            return false;
        }
        removeDeviceFromList(device);
        port.setDevice(null);
        System.out.println("Device removed successfully");
        return true;
    }

    public void turnON(int portId) { // turns the device on
        if (!isValidPort(portId)) {
            System.out.println("Invalid port ID");
            return;
        }
    
        Port port = ports.get(portId);
        if (!port.isOccupied()) {
            System.out.println("No device connected to port " + portId);
            return;
        }
        Device device = port.getDevice();
        device.turnOn();
    }

    public void turnOFF(int portId) { // turns the device off
        if (isValidPort(portId) && ports.get(portId).isOccupied()) {
            ports.get(portId).getDevice().turnOff();
        }
    }

    public void readSensor(int devId) { // reads the sensor data
        Sensor sensor = getSensorById(devId);
        if (sensor.getState() == State.ON) {
            System.out.printf("%s %s: %s%n",
                sensor.getName(),
                sensor.getDevType(),
                sensor.data2String());
        } else {
            System.out.println("Sensor is OFF");
        }
    }

    public void printDisplay(int devId, String data) { // prints the data on the display
        Display display = getDisplayById(devId);
        if (display == null) {
            System.out.println("There is no display with devId " + devId);
            return;
        }
        if (display.getState() == State.ON) {
            display.printData(data);
        } else {
            System.out.println("Display is OFF");
        }
    } 

    public void setMotorSpeed(int devId, int speed) { // sets the motor speed at a random value
        MotorDriver motordriver = getMotorDriverById(devId);
        if (motordriver.getState() == State.ON) {
            motordriver.setMotorSpeed(speed);
        } else {
            System.out.println("Motor is OFF");
        }
    }

    public void writeWireless(int devId, String data) { // writes the data to the wireless device
        WirelessIO wirelessIO = getWirelessById(devId);
        if (wirelessIO.getState() == State.ON) {
            wirelessIO.sendData(data);
        } else {
            System.out.println("Wireless device is OFF");
        }
    }

    public String readWireless(int devId) { // reads the data from the wireless device
        WirelessIO wirelessIO = getWirelessById(devId);
        if (wirelessIO.getState() == State.ON) {
            String data = wirelessIO.recvData();
            System.out.println(data);
            return data;
        } else {
            System.out.println("Wireless device is OFF");
            return null;
        }
    }

    private boolean deviceExists(int devId, String type) {
        type.toLowerCase();

        if (type.equals("dht11") || type.equals("bme280") || type.equals("mpu6050") || type.equals("gy951"))
            return sensorIds.contains(devId);
        else if (type.equals("lcd") || type.equals("oled"))
            return displayIds.contains(devId);
        else if (type.equals("bluetooth") || type.equals("wifi"))
            return wirelessIds.contains(devId);
        else if (type.equals("pca9685") || type.equals("sparkfunmd"))
            return motorDriverIds.contains(devId);
        else
            return false;
        
    }

    private void addDeviceToList(Device device, int devId) {
        if (device instanceof Sensor) {
            sensors.add((Sensor) device);
            sensorIds.add(devId);
        } else if (device instanceof Display) {
            displays.add((Display) device);
            displayIds.add(devId);
        } else if (device instanceof WirelessIO) {
            wireless.add((WirelessIO) device);
            wirelessIds.add(devId);
        } else if (device instanceof MotorDriver) {
            motordrivers.add((MotorDriver) device);
            motorDriverIds.add(devId);
        }
    }

    private void removeDeviceFromList(Device device) {
        if (device instanceof Sensor) {
            int index = sensors.indexOf(device);
            sensors.remove(index);
            sensorIds.remove(index);
        } else if (device instanceof Display) {
            int index = displays.indexOf(device);
            displays.remove(index);
            displayIds.remove(index);
        } else if (device instanceof WirelessIO) {
            int index = wireless.indexOf(device);
            wireless.remove(index);
            wirelessIds.remove(index);
        } else if (device instanceof MotorDriver) {
            int index = motordrivers.indexOf(device);
            motordrivers.remove(index);
            motorDriverIds.remove(index);
        }
    }

    public void listPorts() { // lists the ports with protocols and their status
        for (int i = 0; i < ports.size(); i++) {
            Port port = ports.get(i);
            if (port.isOccupied()) {
                Device dev = port.getDevice();
                System.out.printf("%d %s occupied %s %s %d %s%n",
                    i,
                    port.getProtocol().getProtocolName(),
                    dev.getName(),
                    dev.getDevType(),
                    getDeviceId(dev),
                    dev.getState());
            } else {
                System.out.printf("%d %s empty%n",
                    i,
                    port.getProtocol().getProtocolName());
            }
        }
    }

    public void listDevices(String deviceType) { // lists the devices of a certain type
        switch (deviceType.toLowerCase()) {
            case "sensor":
                for (int i = 0; i < sensors.size(); i++) {
                    printDeviceInfo(sensors.get(i), sensorIds.get(i));
                }
                break;
            case "display":
                for (int i = 0; i < displays.size(); i++) {
                    printDeviceInfo(displays.get(i), displayIds.get(i));
                }
                break;
            case "wireless":
                for (int i = 0; i < wireless.size(); i++) {
                    printDeviceInfo(wireless.get(i), wirelessIds.get(i));
                }
                break;
            case "motor":
                for (int i = 0; i < motordrivers.size(); i++) {
                    printDeviceInfo(motordrivers.get(i), motorDriverIds.get(i));
                }
                break;
            default:
                System.out.println("Invalid device type");
        }
    }
    
    private void printDeviceInfo(Device device, int devId) {
        Port port = findPortByDevice(device);
        if (port != null) {
            System.out.printf("%s %d %d %s%n", 
                device.getName(),
                devId,
                ports.indexOf(port),
                port.getProtocol().getProtocolName());
        }
    }
    private boolean checkDeviceLimit(Device device) { // checks if the device limit is reached
        if (device instanceof Sensor && currentSensors >= maxSensors) return false;
        if (device instanceof Display && currentDisplays >= maxDisplays) return false;
        if (device instanceof WirelessIO && currentWireless >= maxWireless) return false;
        if (device instanceof MotorDriver && currentMotorDrivers >= maxMotorDrivers) return false;
        return true;
    }
    private boolean isValidDeviceId(String deviceType, int devId) {
        switch (deviceType.toLowerCase()) {

                case "dht11":
                    return devId >= 0 && devId < maxSensors;
                case "bme280":
                    return devId >= 0 && devId < maxSensors;
                case "mpu6050":
                    return devId >= 0 && devId < maxSensors;
                case "gy951":
                    return devId >= 0 && devId < maxSensors;
                case "lcd":
                    return devId >= 0 && devId < maxDisplays;
                case "oled":
                    return devId >= 0 && devId < maxDisplays;
                case "bluetooth":
                    return devId >= 0 && devId < maxWireless;
                case "wifi":
                    return devId >= 0 && devId < maxWireless;
                case "pca9685":
                    return devId >= 0 && devId < maxMotorDrivers;
                case "sparkfunmd":
                    return devId >= 0 && devId < maxMotorDrivers;
                default:
                    return false;
        }
    }

    private boolean isValidDeviceName(String deviceType) {
        switch (deviceType.toLowerCase()) {

                case "dht11":
                case "bme280":
                case "mpu6050":
                case "gy951":
                case "lcd":
                case "oled":
                case "bluetooth":
                case "wifi":
                case "pca9685":
                case "sparkfunmd":
                    return true;
                default:
                    return false;
        }
    }
    private boolean isValidPort(int portId) { // checks if the port ID is valid
        return portId >= 0 && portId < ports.size();
    }

    private Display getDisplayById(int devId) {
        if (displayIds.contains(devId)) 
            return displays.get(displayIds.indexOf(devId));
        else
            return null;
    }
    private Sensor getSensorById(int devId) {
        if (sensorIds.contains(devId)) 
            return sensors.get(sensorIds.indexOf(devId));
        else
            return null;
    }
    private WirelessIO getWirelessById(int devId) {
        if (wirelessIds.contains(devId)) 
            return wireless.get(wirelessIds.indexOf(devId));
        else
            return null;
    }
    private MotorDriver getMotorDriverById(int devId) {
        if (motorDriverIds.contains(devId)) 
            return motordrivers.get(motorDriverIds.indexOf(devId));
        else
            return null;
    }
    private int getDeviceId(Device device) { // gets the device ID
        if (sensors.contains(device)) 
            return sensorIds.get(sensors.indexOf(device));
        if (displays.contains(device)) 
            return displayIds.get(displays.indexOf(device));
        if (wireless.contains(device)) 
            return wirelessIds.get(wireless.indexOf(device));
        if (motordrivers.contains(device)) 
            return motorDriverIds.get(motordrivers.indexOf(device));
        
        return -1;
    }
    private Port findPortByDevice(Device device) { // finds the port by the device
        for (Port port : ports) {
            if (port.getDevice() == device) return port;
        }
        return null;
    }
}