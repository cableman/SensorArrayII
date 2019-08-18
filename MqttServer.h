#ifndef MqttServer_h
#define MqttServer_h

#include <Arduino.h>

#include <FS.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MqttServer {
  public:
    // Constructor(s)
    MqttServer();
 
    void setServerAddr(const char* addr);
    void setServerPort(int port);
    void setServerPassword(const char* password);
    void setServerUsername(const char* username);
    void setName(const char* name);

    bool begin();
    bool disconnect();
    bool sendMessage(const char* type, const char* message);

  private:
    void connect();
    char addr[40];
    int port;
    char password[34];
    char username[34];
    char name[34];
    String topic;

    PubSubClient *client;
    WiFiClient espClient;
};

#endif
