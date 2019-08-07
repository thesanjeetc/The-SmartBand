#ifndef MQTTSetup_h
#define MQTTSetup_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void setupMQTT(PubSubClient& client);
void callback();