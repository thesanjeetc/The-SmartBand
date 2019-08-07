#ifndef MQTTClient_h
#define MQTTClient_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

struct dlist{
    int numDevices = 0;
    char *deviceList;
};

class MQTTClient: public PubSubClient{
public:
    MQTTClient();
    void callback(char* topic, byte* payload, unsigned int length);
    void startUp();
    void keepAlive();
private:
    const char* ssid = "VM9304545";
    const char* password =  "nv7nwTfjXksx";
    const char* mqttServer = "m24.cloudmqtt.com";
    const int mqttPort = 19552;
    const char* mqttUser = "ubjhoqpg";
    const char* mqttPassword = "VFLCkzwLxvuD";
};

#endif