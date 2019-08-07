#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <string>
#include <stdint.h>
#include "MQTTClient.h"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

struct Timeout
{
  unsigned long timeoutStart;
  unsigned long timeout;
  Timeout(unsigned long t) { timeout = t; };
  void start() { timeoutStart = millis(); };
  bool checkTimeout() { return (millis() - timeoutStart) > timeout ? true : false; };
} mainT(30);

enum States
{
  detectIR,
  waitCommand,
  executeCommand
} state;

IRsend irsend(4);

IRrecv irrecv(13, 1024, 15, true);
uint64_t IRCodes[4] = {0x20DF906F, 0x20DF40BF, 0x20DFC03F, 0x20DF10EF};
decode_results results;

MQTTClient client;
char deviceID[] = "d0001";
int action;

void handleCommand(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  };

  char command = (char)payload[0];
  switch(command){
    case "e":
      state = executeCommand;
    break;
    case "s":
      Serial.println("---Selected - LED ON---");
    break;
    case "u":
      Serial.println("---Selected - LED OFF---");
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  States state = detectGesture;
  irsend.begin();

  client.startUp();
  client.subscribe(deviceID);
  client.setCallback(handleCommand);
}

void loop()
{
  if (mainT.checkTimeout())
  {
    // Serial.println(ESP.getFreeHeap());
    switch (state)
    {
    case detectIR:
      if (irrecv.decode(&results))
      {
        serialPrintUint64(results.value, HEX);
        for (int i = 0; i < 5; i++)
        {
          if (result.value == IRCodes[i])
          {
            command = i;
            state = waitCommand;
          }
        }
        irrecv.resume(); // Receive the next value
      }
      break;
    case waitCommand:
      break;
    case executeCommand:
      switch (action)
      {
      case 0:
        Serial.println("Tap");
        break;
      case 1:
        Serial.println("Swipe Up");
        break;
      case 2:
        Serial.println("Swipe Down");
        break;
      case 3:
        Serial.println("Long Tap");
        break;
      }
      break;
      state = detectIR;
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