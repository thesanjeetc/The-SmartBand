#include <Arduino.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

const char* ssid     = "VM9304545";
const char* password = "nv7nwTfjXksx";

WiFiServer wifiServer(80);
 
void setup() {
 
  Serial.begin(115200);
 
  delay(1000);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
 
  wifiServer.begin();
}
 
void loop() {
 
  WiFiClient client = wifiServer.available();
 
  if (client) {
 
    while (client.connected()) {
 
      while (client.available()>0) {
        char c = client.read();
        Serial.write(c);
      }
 
      delay(10);
    }
 
    client.stop();
    Serial.println("Client disconnected");
 
  }
}
/*void loop() {
  WiFiClient client = server.available();
  if (!client) {    return;  }
  while(!client.available()) {
    delay(10);
  }
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
}*/
