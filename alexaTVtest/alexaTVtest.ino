#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

#define WIFI_SSID "VM9304545"
#define WIFI_PASS "nv7nwTfjXksx"
#define SERIAL_BAUDRATE                 115200

const uint16_t kIrLed = 4;
IRsend irsend(kIrLed); 
uint16_t up[71] = {9026, 4456,  602, 556,  546, 584,  550, 1660,  578, 554,  582, 550,  552, 552,  582, 548,  576, 556,  548, 1662,  576, 1662,  598, 560,  554, 1658,  582, 1654,  584, 1654,  606, 1632,  608, 1630,  610, 550,  554, 1656,  604, 528,  574, 556,  578, 554,  550, 554,  580, 552,  572, 532,  580, 1656,  582, 522,  602, 1662,  576, 1660,  578, 1658,  580, 1656,  604, 1634,  604, 1634,  606, 39964,  9026, 2218,  578};  // NEC 20DF40BF
uint16_t down[67] = {9028, 4452,  604, 528,  596, 536,  578, 1658,  580, 524,  600, 532,  572, 532,  600, 532,  604, 528,  576, 1662,  578, 1660,  598, 532,  572, 1666,  574, 1664,  574, 1664,  596, 1642,  596, 1640,  598, 1640,  600, 1664,  576, 530,  604, 526,  578, 528,  606, 526,  598, 532,  570, 534,  600, 530,  572, 532,  602, 1662,  576, 1660,  578, 1658,  570, 1668,  602, 1636,  604, 1634,  604};  // NEC 20DFC03F

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("FauxMo demo sketch");
    irsend.begin();
    Serial.println("After connection, ask Alexa/Echo to 'turn <devicename> on' or 'off'");

    // Wifi
    wifiSetup();

    // Fauxmo
    fauxmo.addDevice("volume");

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        if (state) {
    Serial.println("ON");
    irsend.sendRaw(down, 67, 38);
  } else {
    Serial.println("OFF");
    irsend.sendRaw(up, 71, 38);
  }
  });
}

void loop() {
  fauxmo.handle();
}
