// WifiConfiguraion.
#include "WifiSetup.h"

// Create sensor object.
#include "Sensors.h"
Sensors sensors(SDA, SCL, D3);

#include "MqttServer.h"
MqttServer mqtt;

#include "Display.h"
Display display;

// LED status.
#include <Ticker.h>
Ticker ticker;

// Configuration.
#include "Config.h"
Config config;

/**
 * Ticker callback that change led (flip on/off).
 */
void tick() {
  int state = digitalRead(LED_BUILTIN);
  digitalWrite(LED_BUILTIN, !state);
}

/**
 * Error function to flash led with interval.
 */
void error(float interval) {
  Serial.println("ERROR!!!");
  ticker.detach();
  ticker.attach(interval, tick);
  delay(10000);
  //ESP.reset();
}

/**
 * Setup the system.
 */
void setup() {
  Serial.begin(115200);
  delay(2000);

  // Init display
  display.begin();
  display.write("Booting", "");

  // Set led pin as output
  pinMode(LED_BUILTIN, OUTPUT);

  // Flash the led.
  ticker.attach(1, tick);

  // Load configuration.
  if (!config.load()) {
    // Set defaults.
    display.write("No config", "");
    config.mqtt.port = 1883;
    config.mqtt.interval = 5000;
  }
  else {
    display.write("Config loaded", "");
  }

  // Start wifi manager.
  WifiSetup* wifiSetup = new WifiSetup(config.mqtt);
  wifiSetup->begin();

  if (wifiSetup->hasConfigChanged()) {
    // Save configuration.
    config.mqtt = wifiSetup->getConfig();
    config.save();
  }
  delay(5000);

  randomSeed(micros());
  
  Serial.print("local ip: ");
  String ip =  wifiSetup->getIp().toString();
  Serial.println(ip);

  display.write("IP", ip.c_str());
  delay(5000);
  display.clear();

  // Set MQTT configuration.
  mqtt.setName(config.mqtt.name);
  mqtt.setServerAddr(config.mqtt.addr);
  mqtt.setServerPort(config.mqtt.port);
  mqtt.setServerUsername(config.mqtt.username);
  mqtt.setServerPassword(config.mqtt.password);

  // Start the MQTT process.
  mqtt.begin();

  // Init the sensors.
  sensors.begin();

  // Stop led flashing.
  ticker.detach();
}

/**
 * Main loop.
 */
void loop() {
  digitalWrite(LED_BUILTIN, LOW);

  // Update DHT values.
  sensors.readDHTvalues();

  const float humidity = sensors.getHumidity();
  if (!isnan(humidity)) {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" % ");

    mqtt.sendMessage("humidity", String(humidity).c_str());
  }

  const float temperature = sensors.getTemperature();
  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C ");

    mqtt.sendMessage("temperature", String(temperature).c_str());

    display.write("Temperature", String(temperature).c_str());
  }

  const float heatindex = sensors.getHeatIndex();
  if (!isnan(heatindex)) {
    Serial.print("Heat index: ");
    Serial.print(heatindex);
    Serial.println(" C ");

    mqtt.sendMessage("heatindex", String(heatindex).c_str());
  }

  const float pressure = sensors.getPressure();
  if (!isnan(pressure)) {
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" hPa ");

    mqtt.sendMessage("pressure", String(pressure).c_str());
  }

  const float lux = sensors.getLUX();
  if (!isnan(lux)) {
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx ");

    mqtt.sendMessage("lux", String(lux).c_str());
  } 

  const float co2 = sensors.getCO2();
  if (!isnan(co2)) {
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.println(" ppm ");

    mqtt.sendMessage("co2", String(co2).c_str());
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("................");

  delay(config.mqtt.interval);
}
