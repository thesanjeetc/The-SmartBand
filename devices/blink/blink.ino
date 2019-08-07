#include <Arduino.h>
#include <ESP8266Wifi.h>

void setup() {
  pinMode(0, OUTPUT);
}
 
void loop() {
  digitalWrite(0, HIGH);
  delay(200);
  digitalWrite(0, LOW);
  delay(200);
}
