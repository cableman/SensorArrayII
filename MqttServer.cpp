#include "MqttServer.h"

/**
 * Constructor.
 */
MqttServer::MqttServer() {
  WiFiClient espClient;
  this->espClient = espClient;
  this->client = new PubSubClient(this->espClient);
}

/**
 * Set server address for the MQTT broker. 
 */
void MqttServer::setServerAddr(const char* addr) {
  strcpy(this->addr, addr);
}

/**
 * Set MQTT port.
 */
void MqttServer::setServerPort(int port) {
  this->port = port;
}

/**
 * Set MQTT broker password.
 */
void MqttServer::setServerPassword(const char* password) {
  strcpy(this->password, password);
}

/**
 * Ser MQTT broker username.
 */
void MqttServer::setServerUsername(const char* username) {
  strcpy(this->username, username);
}

/**
 * Set Sensor name also used in the topic name in the MQTT messages.
 */
void MqttServer::setName(const char* name) {
  strcpy(this->name, name);
}

/**
 * Set configuration and begin the process.
 */
void MqttServer::begin() {
  this->client->setServer(this->addr, this->port);

  this->topic = "/esp/";
  this->topic += this->name;
  this->topic += "/";
}

/**
 * Disconnect from the MQTT broker.
 */
void MqttServer::disconnect() {
  this->client->disconnect();
}

/**
 * Send message.
 */
void MqttServer::sendMessage(const char* type, const char* message) {
  this->connect();

  String t = this->topic + type;
  this->client->publish(t.c_str(), message);

  // Listen for incomming messages. This assumes that the sendMessage is
  // used inside the main loop theard.
  this->client->loop();
}

/**
 * Reconnect to the MQTT server.
 */
void MqttServer::connect() {
  if (!this->client->connected()) {
    // Attempt to connect
    if (!this->client->connect(this->name, this->username, this->password)) {
      Serial.println("Unable to connect to MQTT server: ");
      Serial.println(this->addr);
    }
  }
}
