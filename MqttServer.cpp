#include "MqttServer.h"

MqttServer::MqttServer() {
  WiFiClient espClient;
  this->client = new PubSubClient(espClient);
}

void MqttServer::setServerAddr(const char* addr) {
  strcpy(this->addr, addr);
}

void MqttServer::setServerPort(int port) {
  this->port = port;
}

void MqttServer::setServerPassword(const char* password) {
  strcpy(this->password, password);
}

void MqttServer::setServerUsername(const char* username) {
  strcpy(this->username, username);
}

void MqttServer::setName(const char* name) {
  strcpy(this->name, name);
}

/**
 * Set configuration and begin the process.
 */
bool MqttServer::begin() {
  this->client->setServer(this->addr, this->port);

  this->topic = "/esp/";
  this->topic += this->name;
  this->topic += "/";
}

bool MqttServer::disconnect() {
  this->client->disconnect();
}

/**
 * Send message.
 */
bool MqttServer::sendMessage(const char* type, const char* message) {
  if (!this->client->connected()) {
    this->reconnect();
  }

  String t = this->topic + type;
  this->client->publish(t.c_str(), message);

  yield();

  // Listen for incomming messages. This assumes that the sendMessage is
  // used inside the main loop theard.
  this->client->loop();
}

/**
 * Reconnect to the MQTT server.
 */
void MqttServer::reconnect() {
  // Loop until we're reconnected
  if (!this->client->connected()) {
    // Attempt to connect
    if (!this->client->connect(this->name, this->username, this->password)) {
      Serial.println("Unable to connect to MQTT server: ");
      Serial.println(this->addr);
      yield();
    }
  }
}
