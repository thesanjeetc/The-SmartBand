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

void MQTTClient::resetDevices(){
  deviceNum = 0;
}

void MQTTClient::clearDevices(){
  memset(devices, 0, sizeof(devices));
}

 void MQTTClient::callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    devices[deviceNum*5 + i] = (char)payload[i];
  };

  deviceNum++;
}

dlist MQTTClient::getDetected(){
  dlist foundDevices;
  foundDevices.deviceList = devices;
  foundDevices.numDevices = deviceNum;
  return foundDevices;
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
