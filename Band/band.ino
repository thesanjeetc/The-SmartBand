#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <string>
#include <stdint.h>
#include "GestureSensor.h"
#include "MQTTClient.h"
#include <IRremoteESP8266.h>
#include <IRsend.h>

struct Timeout
{
  unsigned long timeoutStart;
  unsigned long timeout;
  Timeout(unsigned long t) { timeout = t; };
  void start() { timeoutStart = millis(); };
  bool checkTimeout() { return (millis() - timeoutStart) > timeout ? true : false; };
} mainT(30), dProcessT(3500), dSelectionT(0), dHoverT(3000);

enum States
{
  detectGesture,
  waitResponse,
  deviceSelection,
  deviceOnSelect,
  resetProcess
} state;

IRsend irsend(4);
int gesture;
uint64_t IRCodes[4] = {0x20DF906F, 0x20DF40BF, 0x20DFC03F, 0x20DF10EF};

GestureSensor sensor(A0);
MQTTClient client("b");
dlist found;
int selectedDevice;
char selectedDeviceName[5];

void setup()
{
  Serial.begin(115200);
  client.startUp();

  States state = detectGesture;

  irsend.begin();
  client.subscribe("devices");  
}

void loop()
{
  if (mainT.checkTimeout())
  {
    // Serial.println(ESP.getFreeHeap());
    switch (state)
    {
    case detectGesture:
      gesture = sensor.detectGesture();
      irsend.sendNEC(IRCodes[gesture]);
      if (gesture != -1)
      {
        client.publish("gestures", "Gesture Detected.");
        Serial.println("----Gesture Detected----");
        client.clearDevices();
        dProcessT.start();
        state = waitResponse;
      };
      break;
    case waitResponse:
      if (dProcessT.checkTimeout())
      {
        found = client.getDetected();
        Serial.println("----Processing Detected Devices----");
        switch (found.numDevices)
        {
        case 0:
          state = resetProcess;
          Serial.println("----No Devices Detected----");
          break;
        case 1:
          strncpy(selectedDeviceName, found.deviceList, 5);
          client.publish(selectedDeviceName, "Execute");
          state = resetProcess;
          Serial.println("----One Device Detected----");
          break;
        default:
          dSelectionT.start();
          dSelectionT.timeout = 20000;
          state = deviceSelection;
          Serial.println("----Multiple Devices----");
          break;
        }
      }
      break;
    case deviceSelection:
      if (dSelectionT.checkTimeout())
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
        dHoverT.start();
        client.publish(selectedDeviceName, "Select");
        Serial.println("----Device Selected: ");
        Serial.print(selectedDeviceName);
        state = deviceOnSelect;
      }
      break;
    case deviceOnSelect:
      if (dHoverT.checkTimeout())
      {
        selectedDevice++;
        if (selectedDevice > found.numDevices)
        {
          selectedDevice = 0;
          Serial.println("----Device Cycle Finished----");
          state = resetProcess;
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
        gesture = sensor.detectGesture();
        if (gesture == 0)
        {
          client.publish(selectedDeviceName, "Execute");
          Serial.println("----Selection Gesture Detected, Executing ");
          Serial.print(selectedDeviceName);
          state = resetProcess;
        };
      }
      break;
    case resetProcess:
      client.resetDevices();
      selectedDevice = 0;
      state = detectGesture;
      break;
    }

    mainT.start();
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

//structures cleanup  - timeout - code readability