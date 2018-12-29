#ifndef Config_h
#define Config_h

#include <Arduino.h>

#include <FS.h>
#include <ArduinoJson.h>

#define CONFIG_FILE "/config.json"

class Config {
  public:
    // Constructor(s)
    Config();

    bool load();

    // Sensor name.
    char name[34];
    int interval;

    struct mqtt {
      char addr[40];
      int port;
      char password[34];
      char username[34];
    } mqtt;

    struct wifi {
      char ssid[32];
      char password[32];
    } wifi;
};


#endif
