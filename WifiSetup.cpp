#include "WifiSetup.h"

// Static variable used to set config state.
bool WifiSetup::shouldSaveConfig = false;

/**
 * Constructor.
 */
WifiSetup::WifiSetup(mqttConf mqttConfig) {
    WifiSetup::shouldSaveConfig = false;

    this->mqttConfig = mqttConfig;
}

/**
 * Get state for configuration. 
 */
bool WifiSetup::hasConfigChanged() {
  return WifiSetup::shouldSaveConfig;
}

/**
 * Get local ip.
 */
IPAddress WifiSetup::getIp() {
  return WiFi.localIP();
}

/**
 * Static method to use to change config state.
 */
void WifiSetup::saveConfigCallback () {
  Serial.println("Should save config");
  WifiSetup::shouldSaveConfig = true;
}

/**
 * Get current configuration.
 */
mqttConf WifiSetup::getConfig() {
  return this->mqttConfig;
}

/**
 * Start WifiMangner and connect to the network.
 * 
 * Network connection errors table.
 * 
 * WL_IDLE_STATUS      = 0
 * WL_NO_SSID_AVAIL    = 1
 * WL_SCAN_COMPLETED   = 2
 * WL_CONNECTED        = 3
 * WL_CONNECT_FAILED   = 4
 * WL_CONNECTION_LOST  = 5
 * WL_DISCONNECTED     = 6 
 */
void WifiSetup::begin() {
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  WiFiManagerParameter custom_name("name", "Name", this->mqttConfig.name, 40);
  WiFiManagerParameter custom_interval("interval", "Publish interval", String(this->mqttConfig.interval).c_str(), 10);
  WiFiManagerParameter custom_mqtt_addr("addr", "mqtt adress", this->mqttConfig.addr, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", String(this->mqttConfig.port).c_str(), 34);
  WiFiManagerParameter custom_mqtt_username("username", "mqtt username", this->mqttConfig.username, 34);
  WiFiManagerParameter custom_mqtt_password("password", "mqtt password", this->mqttConfig.password, 34);

  // Reset settings - for testing
  //wifiManager.resetSettings();

  // set config save notify callback
  wifiManager.setSaveConfigCallback(WifiSetup::saveConfigCallback);
 
  // Add all your parameters here.
  wifiManager.addParameter(&custom_name);
  wifiManager.addParameter(&custom_mqtt_addr);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_username);
  wifiManager.addParameter(&custom_mqtt_password);
  wifiManager.addParameter(&custom_interval);

  if (!wifiManager.autoConnect("SensorArray", "password")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  if (WifiSetup::shouldSaveConfig) {
    strncpy(this->mqttConfig.name, custom_name.getValue(), sizeof(this->mqttConfig.name)/sizeof(char));
    strncpy(this->mqttConfig.addr, custom_mqtt_addr.getValue(), sizeof(this->mqttConfig.addr)/sizeof(char));
    this->mqttConfig.port = atoi(custom_mqtt_port.getValue());
    strncpy(this->mqttConfig.password, custom_mqtt_password.getValue(), sizeof(this->mqttConfig.password)/sizeof(char));
    strncpy(this->mqttConfig.username, custom_mqtt_username.getValue(), sizeof(this->mqttConfig.username)/sizeof(char));
    this->mqttConfig.interval = atoi(custom_interval.getValue());

    Serial.println("---------------------===---------------------");
    Serial.println(this->mqttConfig.name);
    Serial.println(this->mqttConfig.addr);
    Serial.println(this->mqttConfig.password);
    Serial.println(this->mqttConfig.username);
    Serial.println(this->mqttConfig.interval);
    Serial.println("---------------------===---------------------");
  }
}
