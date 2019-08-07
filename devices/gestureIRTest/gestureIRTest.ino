#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
const int p = A0;
int curVal, startVal, lastVal = 0;

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t up[71] = {9026, 4456,  602, 556,  546, 584,  550, 1660,  578, 554,  582, 550,  552, 552,  582, 548,  576, 556,  548, 1662,  576, 1662,  598, 560,  554, 1658,  582, 1654,  584, 1654,  606, 1632,  608, 1630,  610, 550,  554, 1656,  604, 528,  574, 556,  578, 554,  550, 554,  580, 552,  572, 532,  580, 1656,  582, 522,  602, 1662,  576, 1660,  578, 1658,  580, 1656,  604, 1634,  604, 1634,  606, 39964,  9026, 2218,  578};  // NEC 20DF40BF
uint16_t down[67] = {9028, 4452,  604, 528,  596, 536,  578, 1658,  580, 524,  600, 532,  572, 532,  600, 532,  604, 528,  576, 1662,  578, 1660,  598, 532,  572, 1666,  574, 1664,  574, 1664,  596, 1642,  596, 1640,  598, 1640,  600, 1664,  576, 530,  604, 526,  578, 528,  606, 526,  598, 532,  570, 534,  600, 530,  572, 532,  602, 1662,  576, 1660,  578, 1658,  570, 1668,  602, 1636,  604, 1634,  604};  // NEC 20DFC03F
void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  pinMode(p, INPUT);
}

void loop() {

  curVal = analogRead(p);
  if (curVal > 30){
      startVal = curVal;
      lastVal = curVal;
      while (curVal > 30){
        curVal = analogRead(p);
        if (abs(lastVal - curVal) > 30){
          if(lastVal - startVal > 0){
            Serial.println(String("Swipe up"));
            irsend.sendRaw(up, 71, 38);
          }else{
            Serial.println(String("Swipe down"));
            irsend.sendRaw(down, 67, 38);
          }
        }
        lastVal = curVal;
        delay(2);
    }
  }
  delay(10);
  
}
