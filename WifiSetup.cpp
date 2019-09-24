#include "WifiSetup.h"

bool WifiSetup::shouldSaveConfig = false;

WifiSetup::WifiSetup(mqttConf mqttConfig) {
    WifiSetup::shouldSaveConfig = false;

    this->mqttConfig = mqttConfig;
}

bool WifiSetup::hasConfigChanged() {
  return WifiSetup::shouldSaveConfig;
}

IPAddress WifiSetup::getIp() {
  return WiFi.localIP();
}

void WifiSetup::saveConfigCallback () {
  Serial.println("Should save config");
  WifiSetup::shouldSaveConfig = true;
}

mqttConf WifiSetup::getConfig() {
  return this->mqttConfig;
}

void WifiSetup::begin() {
  // WiFiManager
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

  if (!wifiManager.autoConnect("SensorArray", "Sensor")) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  /**
  "WL_IDLE_STATUS      = 0\n"
  "WL_NO_SSID_AVAIL    = 1\n"
  "WL_SCAN_COMPLETED   = 2\n"
  "WL_CONNECTED        = 3\n"
  "WL_CONNECT_FAILED   = 4\n"
  "WL_CONNECTION_LOST  = 5\n"
  "WL_DISCONNECTED     = 6\n"
  */

  // If you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  Serial.println();
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
