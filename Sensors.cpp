#include "Sensors.h"

/**
 * Constructor.
 */
Sensors::Sensors(byte I2C_SDA, byte I2C_SCL, byte DHT_Pin) {
  this->I2C_SDA = I2C_SDA;
  this->I2C_SCL = I2C_SCL;
  this->DHT_Pin = DHT_Pin;
}

/**
 * Init the sensors.
 */
void Sensors::begin() {
  Wire.begin(this->I2C_SDA, this->I2C_SCL);

  // Give the wire a change to init.
  delay(500);

  // BH1750 Sensor.
  this->lightMeter = new BH1750(BH1750_ADD);
  if (!this->lightMeter->begin()) {
    Serial.println("Could not find a valid BH1750 sensor, check wiring!");
    this->status.lightMeter = false;
  }

  // BME280 Sensor.
  this->bme = new Adafruit_BME280();
  if (!this->bme->begin(BME280_ADD)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    this->status.bme = false;
  }

  this->sgp = new Adafruit_SGP30();
  if (!this->sgp->begin()) {
    Serial.println("Could not find a valid SGP30 sensor, check wiring!");
    this->status.sgp = false;
  }

  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //this->sgp.setIAQBaseline(0x8E68, 0x8F41);

  // DHT Sensor.
  this->dht = new DHTesp();
  this->dht->setup(this->DHT_Pin, DHTesp::DHT22);

  // Init the temp/hum values.
  delay(this->dht->getMinimumSamplingPeriod());
  this->readDHTvalues();
}

/**
 * Read DHT values.
 */
void Sensors::readDHTvalues() {
  if (this->status.dht) {
    this->temperature = this->dht->getTemperature();
    this->humidity = this->dht->getHumidity();
  }
  else {
    this->temperature = sqrt(-1);
    this->humidity = sqrt(-1);
  }
}

/**
 * Get DHT temperature.
 */
float Sensors::getTemperature() {
  return this->temperature;
}

/**
 * Get DHT humidity.
 */
float Sensors::getHumidity() {
  return this->humidity;
}

/**
 * Get heat-index (how hot it really feels when relative humidity is factored in with the actual air temperature).
 */
float Sensors::getHeatIndex() {
  const float h = this->getHumidity();
  const float t = this->getTemperature();

  if (isnan(h) || isnan(t)) {
    return h;
  }

  // Compute heat index in Celsius.
  return this->dht->computeHeatIndex(t, h, false);
}

/**
 * Get pressure in hPa.
 */
float Sensors::getPressure() {
  if (this->status.bme) {
    const float pressure = this->bme->readPressure();
    if (!isnan(pressure)) {
      return pressure / 100.0F;
    }
  }

  // Return NAN.
  return sqrt(-1);
}

/**
 * Get light reading in LUX.
 */
int Sensors::getLUX() {
  if (this->status.lightMeter) {
    return this->lightMeter->readLightLevel();
  }

  // Return NAN.
  return sqrt(-1);
}

/**
 * Get IAQ-calculated equivalent CO2 in ppm.
 */
int Sensors::getCO2() {
  if (this->status.sgp) {
    const float h = this->getHumidity();
    const float t = this->getTemperature();

    if (!isnan(h) || !isnan(t)) {
      // Approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
      const float absoluteHumidity = 216.7f * ((h / 100.0f) * 6.112f * exp((17.62f * t) / (243.12f + t)) / (273.15f + t)); // [g/m^3]
      const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
      this->sgp->setHumidity(absoluteHumidityScaled);
    }

    if (!this->sgp->IAQmeasure()) {
      return sqrt(-1);
    }

    return this->sgp->eCO2;
  }

  // Return NAN.
  return sqrt(-1);
}