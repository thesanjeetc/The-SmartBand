#include "MQTTClient.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;

MQTTClient::MQTTClient(): PubSubClient(espClient){
  Serial.println("Created.");
}

void MQTTClient::startUp(){
    WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("Connected to the WiFi network");
 
  setServer(mqttServer, mqttPort);
  setCallback([this] (char* topic, byte* payload, unsigned int length) { this->callback(topic, payload, length); });
 
  while (!connected()) {
    if (connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("Connected to MQTT broker.");  
    } else {
      Serial.print("Error State:");
      Serial.print(state());
      delay(2000);
    }
  }
}

void MQTTClient::keepAlive(){
  while (!connected()) {
    if (connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("Connected to MQTT broker.");  
    } else {
      Serial.print("Error State:");
      Serial.print(state());
      delay(2000);
    }
  }
}
