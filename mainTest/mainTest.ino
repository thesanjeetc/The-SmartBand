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
  waitResponse,
  deviceSelection,
  deviceOnSelect,
  resetProcess
};
int g;
States state;
GestureSensor sensor(A0);
IRsend irsend(4);
MQTTClient client;

uint16_t up[71] = {9026, 4456, 602, 556, 546, 584, 550, 1660, 578, 554, 582, 550, 552, 552, 582, 548, 576, 556, 548, 1662, 576, 1662, 598, 560, 554, 1658, 582, 1654, 584, 1654, 606, 1632, 608, 1630, 610, 550, 554, 1656, 604, 528, 574, 556, 578, 554, 550, 554, 580, 552, 572, 532, 580, 1656, 582, 522, 602, 1662, 576, 1660, 578, 1658, 580, 1656, 604, 1634, 604, 1634, 606, 39964, 9026, 2218, 578}; // NEC 20DF40BF
uint16_t down[67] = {9028, 4452, 604, 528, 596, 536, 578, 1658, 580, 524, 600, 532, 572, 532, 600, 532, 604, 528, 576, 1662, 578, 1660, 598, 532, 572, 1666, 574, 1664, 574, 1664, 596, 1642, 596, 1640, 598, 1640, 600, 1664, 576, 530, 604, 526, 578, 528, 606, 526, 598, 532, 570, 534, 600, 530, 572, 532, 602, 1662, 576, 1660, 578, 1658, 570, 1668, 602, 1636, 604, 1634, 604};                       // NEC 20DFC03F

unsigned long startTimeout;
unsigned long waitTimeout = 10000;

unsigned long deviceTimeout;
unsigned long deviceWait = 3000;

unsigned long selectionTimeout;
unsigned long selectionWait;

int selectedDevice;

dlist found;

char selectedDeviceName[5];

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
        Serial.println("----Gesture Detected----");
        client.clearDevices();
        startTimeout = millis();
        state = waitResponse;
      };
      break;
    case waitResponse:
      if (millis() - startTimeout > waitTimeout)
      {
        found = client.getDetected();
        Serial.println("----Processing Detected Devices----");
        switch (found.numDevices)
        {
        case 0:
          state = resetProcess;
        break;
        case 1:
          strncpy(selectedDeviceName, found.deviceList, 5);
          client.publish(selectedDeviceName, "Execute");
          state = resetProcess;
          Serial.println("----One Device Detected----");
          break;
        default:
          selectionTimeout = millis();
          selectionWait = found.numDevices*3000 + 12000;
          state = deviceSelection;
          Serial.println("----Multiple Devices----");
          break;
        }
      }
      break;
    case deviceSelection:
      if (millis() - selectionTimeout > selectionWait)
      {
        Serial.println("----No Device Selected----");
        state = resetProcess;
      }
      else
      {
        for (int i = 0; i < 5; i++)
        {
          selectedDeviceName[i] = *(found.deviceList + selectedDevice * 5 + i);
        };
        deviceTimeout = millis();
        client.publish(selectedDeviceName, "Select");
        Serial.println("----Device Selected: ");
        Serial.print(selectedDeviceName);
        state = deviceOnSelect;
      }
      break;
    case deviceOnSelect:
      if (millis() - deviceTimeout > deviceWait)
      {
        selectedDevice++;
        if (selectedDevice > found.numDevices)
        {
          selectedDevice = 0;
          Serial.println("----Device Cycle Finished----");
        }
        else
        {
          client.publish(selectedDeviceName, "Deselect");
          Serial.println("----Device Deselected: ");
          Serial.print(selectedDeviceName);
          state = deviceSelection;
        }
      }
      else
      {
        g = sensor.detectGesture();
        if (g == 0)
        {
          client.publish(selectedDeviceName, "Execute");
          Serial.println("----Selection Gesture Detected, Executing ");
          Serial.print(selectedDeviceName);
          state = resetProcess;
        };
      }
    case resetProcess:
      client.resetDevices();
      selectedDevice = 0;
      state = detectGesture;
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
