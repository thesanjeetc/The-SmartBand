#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <string>
#include <stdint.h>
#include "GestureSensor.h"
#include "MQTTClient.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

unsigned long startMillis;
unsigned long currentMillis;
unsigned long diff;
enum States
{
  detectGesture,
  waitResponse
};
int g;
States state;
GestureSensor sensor(A0);
IRsend irsend(4);
MQTTClient client;
int waitTimeout = 3000;

uint16_t up[71] = {9026, 4456, 602, 556, 546, 584, 550, 1660, 578, 554, 582, 550, 552, 552, 582, 548, 576, 556, 548, 1662, 576, 1662, 598, 560, 554, 1658, 582, 1654, 584, 1654, 606, 1632, 608, 1630, 610, 550, 554, 1656, 604, 528, 574, 556, 578, 554, 550, 554, 580, 552, 572, 532, 580, 1656, 582, 522, 602, 1662, 576, 1660, 578, 1658, 580, 1656, 604, 1634, 604, 1634, 606, 39964, 9026, 2218, 578}; // NEC 20DF40BF
uint16_t down[67] = {9028, 4452, 604, 528, 596, 536, 578, 1658, 580, 524, 600, 532, 572, 532, 600, 532, 604, 528, 576, 1662, 578, 1660, 598, 532, 572, 1666, 574, 1664, 574, 1664, 596, 1642, 596, 1640, 598, 1640, 600, 1664, 576, 530, 604, 526, 578, 528, 606, 526, 598, 532, 570, 534, 600, 530, 572, 532, 602, 1662, 576, 1660, 578, 1658, 570, 1668, 602, 1636, 604, 1634, 604};                       // NEC 20DFC03F

uint16_t IRCommands[4] = {

};

// dlist found;
int startTimeout;

void setup()
{
  Serial.begin(115200);
  States state = detectGesture;
  irsend.begin();

  client.startUp();
  client.subscribe("devices");
}

void loop()
{
  currentMillis = millis();
  diff = currentMillis - startMillis;

  if (diff >= 10)
  {
    // Serial.println(ESP.getFreeHeap());
    switch (state)
    {
    case detectGesture:
      g = sensor.detectGesture();
      if (g != -1)
      {
        client.publish("gestures", "Gesture Detected.");
        startTimeout = millis();
        state = waitResponse;
      };
    break;
    case waitResponse:
      if(millis() - startTimeout > waitTimeout){
        dlist found;
        found = client.getDetected();
        switch(found.numDevices){
          case 0:
          ;
          break;
          case 1:
            char d[5];
            strcpy(d, found.deviceList);
            client.publish(d, "Execute.");
          break;
          default:
          ;
          break;
        }
        // Serial.println(*(found.deviceList));
        // for (int i = 0; i < *(found.numDevices) - 1; i++) {
        //   Serial.print((char)payload[i]);
        //   devices[deviceNum*5 + i] = (char)payload[i];
        // };
        // while (*(found.deviceList)){
        //     Serial.print(*(found.deviceList));
        //     found.deviceList++;
        // }
        client.resetDevices();
        state = detectGesture;
      }
    break;
    }

    startMillis = currentMillis;
  }

  client.keepAlive();
  client.loop();
  
}

// Points - Lowered delay to ensure quick recognition
// Tried avaeraging but slowed down - not the problem
// Flow chart diagram
// 2-3 dyas
// changed to use millis() instead of delay - https://learn.adafruit.com/multi-tasking-the-arduino-part-1/using-millis-for-timing
// reduce accidental changes
// state machine !!!!!!!!!!
// testing using different length of pressures
// changed thinking from tap being small time delay to low variation in voltage
// soft wd reset - low delay - wifi stack - timing constraints - delay in wrong place

//enum class vs enum
//changed logic
//created new class - efficient, memory
//guides need to be referenced

//inheritance
//callbacks
