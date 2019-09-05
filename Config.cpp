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

        Serial.print("Config file size: ");
        Serial.println(size);

        DynamicJsonDocument doc(size);
        auto error = deserializeJson(doc, buf.get());

        //JsonObject& json = jsonBuffer.parseObject(buf.get());
        if (!error) {
          strcpy(this->name, doc["name"]);
          this->interval = doc["interval"];
          
          strcpy(this->mqtt.addr, doc["mqtt_addr"]);
          this->mqtt.port = doc["mqtt_port"];
          strcpy(this->mqtt.username, doc["mqtt_username"]);
          strcpy(this->mqtt.password, doc["mqtt_passwd"]);

          strcpy(this->wifi.ssid, doc["wifi_ssid"]);
          strcpy(this->wifi.password, doc["wifi_password"]);

          Serial.println("Config JSON parsed");
        } 
        else {
          Serial.println("Config JSON file not parsed");
          Serial.println(error.c_str());
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
