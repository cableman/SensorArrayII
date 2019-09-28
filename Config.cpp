#include "Config.h"

/**
 * Constructor.
 */
Config::Config() {
}

/**
 * Load configuration from json file.
 */
bool Config::load() {
  if (SPIFFS.begin()) {
    Serial.println("SPIFFS started");
    if (SPIFFS.exists(CONFIG_FILE)) {
      Serial.println("SPIFFS config file loaded");

      // File exists, reading and loading
      File configFile = SPIFFS.open(CONFIG_FILE, "r");
    
      if (configFile) {
        size_t size = configFile.size();

        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);
        configFile.readBytes(buf.get(), size);

        Serial.print("Config file size: ");
        Serial.println(size);

        const size_t capacity = JSON_OBJECT_SIZE(6) + 250;
        DynamicJsonDocument doc(capacity);
        auto error = deserializeJson(doc, buf.get());

        if (!error) {
          strcpy(this->mqtt.name, doc["name"]);
          this->mqtt.interval = doc["interval"];
          
          strcpy(this->mqtt.addr, doc["mqtt_addr"]);
          this->mqtt.port = doc["mqtt_port"];
          strcpy(this->mqtt.username, doc["mqtt_username"]);
          strcpy(this->mqtt.password, doc["mqtt_passwd"]);

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

/**
 * Save configuration into json file.
 */
bool Config::save() {
    if (SPIFFS.begin()) {
    File configFile = SPIFFS.open(CONFIG_FILE, "w");
  
    if (configFile) {
      const size_t capacity = JSON_OBJECT_SIZE(6) + 500;
      DynamicJsonDocument doc(capacity);

      doc["name"] = this->mqtt.name;
      doc["interval"] = this->mqtt.interval;
      doc["mqtt_addr"] = this->mqtt.addr;
      doc["mqtt_port"] = this->mqtt.port;
      doc["mqtt_username"] = this->mqtt.username;
      doc["mqtt_passwd"] = this->mqtt.password;

      if (serializeJson(doc, configFile) == 0) {
        Serial.println(F("Failed to write to file"));
      }

      configFile.close();
    }
    else {
      Serial.println("Config file not loaded");
      return false;
    }
  } 
  else {
    Serial.println("SPIFFS not started");
    return false;
  }

  return true;
}
