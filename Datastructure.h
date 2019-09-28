#ifndef Datastructure_h
#define Datastructure_h

/**
 * MQTT configuration.
 */
struct mqttConf {
      char name[40];
      char addr[40];
      int port;
      char password[34];
      char username[34];
      int interval;
    };

#endif