#ifndef Sensors_h
#define Sensors_h

#include <Arduino.h>

// Sensor (i2c).
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

#define BME280_ADD 0x76

#include <BH1750.h>
#define BH1750_ADD 0x23

#include <Adafruit_SGP30.h>

#include <DHTesp.h>

class Sensors {
  public:
    // Constructor(s)
    Sensors(byte I2C_SDA, byte I2C_SCL, byte DHT_Pin);

    void begin();
    void readDHTvalues();
    float getTemperature();
    float getHumidity();
    float getHeatIndex();
    float getPressure();
    int getLUX();
    int getCO2();

  private:
    byte I2C_SDA;
    byte I2C_SCL;
    byte DHT_Pin;
    Adafruit_BME280* bme;
    Adafruit_SGP30* sgp;
    BH1750* lightMeter;
    DHTesp* dht;

    // Defines the sensors and their current status.
    struct SensorStatus {
        SensorStatus() : bme(true), sgp(true), lightMeter(true), dht(true) {}
        bool bme;
        bool sgp;
        bool lightMeter;
        bool dht;
    } status;

    float temperature;
    float humidity;
};

#endif
