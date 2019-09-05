#ifndef WifiSetup_h
#define WifiSetup_h

#include "Datastructure.h"

#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>

class WifiSetup {
  public:
    // Constructor(s)
    WifiSetup(wifiConf wifiConfig, mqttConf mqttConfig);

    void begin(char name[40], char addr[40], int port, char username[34], char password[34]);
    bool hasConfigChanged();
    static void saveConfigCallback();
    wifiConf getWifiConfiguration();
    mqttConf getattrlist();

  private:
    wifiConf wifiConfig;
    mqttConf mqttConfig;
    static bool shouldSaveConfig;
};

#endif