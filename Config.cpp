#include "Config.h"

/**
 * Constructor.
 */
Config::Config() {
}

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

        DynamicJsonDocument doc(size);
        auto error = deserializeJson(doc, buf.get());
        JsonObject obj = doc.as<JsonObject>();

        Serial.print("Data: ");
        Serial.println(obj.containsKey("name"));
        const char* name1 = obj["name"];
        Serial.print("Content: ");
        Serial.println(name1);


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

bool Config::save() {
    if (SPIFFS.begin()) {
    File configFile = SPIFFS.open(CONFIG_FILE, "w");
  
    if (configFile) {
      const size_t capacity = JSON_OBJECT_SIZE(6);
      DynamicJsonDocument doc(capacity);

      doc["name"] = String(this->mqtt.name);
      doc["interval"] = this->mqtt.interval;
      doc["mqtt_addr"] = String(this->mqtt.addr);
      doc["mqtt_port"] = this->mqtt.port;
      doc["mqtt_username"] = String(this->mqtt.username);
      doc["mqtt_passwd"] = String(this->mqtt.password);

      serializeJson(doc, configFile);
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
