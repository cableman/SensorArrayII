#include "Config.h"

/**
 * Constructor.
 */
Config::Config() {
}

bool Config::load() {
  if (SPIFFS.begin()) {
    if (SPIFFS.exists(CONFIG_FILE)) {
      // File exists, reading and loading
      File configFile = SPIFFS.open(CONFIG_FILE, "r");
    
      if (configFile) {
        size_t size = configFile.size();

        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);

        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        if (json.success()) {
          strcpy(this->name, json["name"]);
          this->interval = json["interval"];
          
          strcpy(this->mqtt.addr, json["mqtt_addr"]);
          this->mqtt.port = json["mqtt_port"];
          strcpy(this->mqtt.username, json["mqtt_username"]);
          strcpy(this->mqtt.password, json["mqtt_passwd"]);

          strcpy(this->wifi.ssid, json["wifi_ssid"]);
          strcpy(this->wifi.password, json["wifi_password"]);

          Serial.println("Config JSON parsed");
        } 
        else {
          Serial.println("Config JSON file not parsed");
          return false;
        }
        configFile.close();
      }
      else {
        Serial.println("Config file not loaded");
        return false;
      }
    }
    else {
      Serial.println("Config file not found");
      return false;
    }
  } 
  else {
    Serial.println("SPIFFS not started");
    return false;
  }

  return true;
}
