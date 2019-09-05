#ifndef Datastructure_h
#define Datastructure_h

struct mqttConf {
      char addr[40];
      int port;
      char password[34];
      char username[34];
    };

struct wifiConf {
      char ssid[32];
      char password[32];
    };

#endif