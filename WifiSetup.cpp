#include "WifiSetup.h"

WifiSetup::WifiSetup(wifiConf wifiConfig, mqttConf mqttConfig) {
    WifiSetup::shouldSaveConfig = false;

    this->wifiConfig = wifiConfig;
    this->mqttConfig = mqttConfig;
}

bool WifiSetup::hasConfigChanged() {
  return WifiSetup::shouldSaveConfig;
}

void WifiSetup::saveConfigCallback () {
  WifiSetup::shouldSaveConfig = true;
}

void WifiSetup::begin(char name[40], char addr[40], int port, char username[34], char password[34]) {

  WiFiManagerParameter custom_name("name", "Name", name, 40);
  WiFiManagerParameter custom_mqtt_addr("addr", "mqtt adress", addr, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", String(port).c_str(), 34);
  WiFiManagerParameter custom_mqtt_username("username", "mqtt username", username, 34);
  WiFiManagerParameter custom_mqtt_password("password", "mqtt password", password, 34);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(WifiSetup::saveConfigCallback);

  // set static ip
  wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  // Add all your parameters here.
  wifiManager.addParameter(&custom_name);
  wifiManager.addParameter(&custom_mqtt_addr);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_username);
  wifiManager.addParameter(&custom_mqtt_password);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("SensorArray", "SensorArray")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  if (shouldSaveConfig) {
    //strcpy(mqtt_server, custom_mqtt_server.getValue());
    //strcpy(mqtt_port, custom_mqtt_port.getValue());
    //strcpy(blynk_token, custom_blynk_token.getValue());
  }
}
