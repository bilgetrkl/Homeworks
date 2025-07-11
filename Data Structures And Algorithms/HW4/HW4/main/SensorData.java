package main;

/** 
 * SensorData class represents the sensor data of a planet or satellite.
 * It contains the temperature, pressure, humidity, and radiation data.
 */
public class SensorData {
    private Double temperature;
    private Double pressure;
    private Double humidity;
    private Double radiation;

    /**
     * Constructor to initialize the sensor data.
     * @param temperature Temperature in Kelvin
     * @param pressure Pressure in Pascals
     * @param humidity Humidity in percentage (0-100)
     * @param radiation Radiation in Sieverts
     */

    public SensorData(Double temperature, Double pressure, Double humidity, Double radiation) {
        this.temperature = temperature; // in Kelvin
        this.pressure = pressure; // in Pascals
        this.humidity = humidity; // percentage (0-100)
        this.radiation = radiation; // in Sieverts
    }

    /**
     * Getter for temperature.
     * @return temperature value
     */
    public Double getTemperature() { return temperature; }

    /**
     * Getter for pressure.
     * @return pressure value
     */

    public Double getPressure() { return pressure; }

    /**
     * Getter for humidity.
     * @return humidity value
     */
    public Double getHumidity() { return humidity; }

    /**
     * Getter for radiation.
     * @return radiation value
     */
    public Double getRadiation() { return radiation; }
}