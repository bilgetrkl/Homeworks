package main;

import main.core.Device;
import main.protocols.Protocol;

import main.devices.sensors.temp.DHT11;
import main.devices.sensors.temp.BME280;
import main.devices.sensors.imu.MPU6050;
import main.devices.sensors.imu.GY951;
import main.devices.displays.LCD;
import main.devices.displays.OLED;
import main.devices.wireless.Bluetooth;
import main.devices.wireless.Wifi;
import main.devices.motors.PCA9685;
import main.devices.motors.SparkFunMD;

public class Devices { // class for all devices
    
    public static Device createDevice(String deviceType, Protocol protocol) { // creates a devices based on the type and protocol it uses
        try {
            switch (deviceType.toLowerCase()) {
                case "dht11":
                    return new DHT11(protocol);
                case "bme280":
                    return new BME280(protocol);
                case "mpu6050":
                    return new MPU6050(protocol);
                case "gy951":
                    return new GY951(protocol);
                case "lcd":
                    return new LCD(protocol);
                case "oled":
                    return new OLED(protocol);
                case "bluetooth":
                    return new Bluetooth(protocol);
                case "wifi":
                    return new Wifi(protocol);
                case "pca9685":
                    return new PCA9685(protocol);
                case "sparkfunmd":
                    return new SparkFunMD(protocol);
                default:
                    System.out.println("Unknown device type: " + deviceType);
                    return null;
            }
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage());
            return null;
        }
    }
}