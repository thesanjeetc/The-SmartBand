#include <Arduino.h>
#include <string>
#include <stdint.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtimer.h>
#include <IRutils.h>
#include "MQTTClient.h"

struct Timeout
{
  unsigned long timeoutStart;
  unsigned long timeout;
  Timeout(unsigned long t) { timeout = t; };
  void start() { timeoutStart = millis(); };
  bool checkTimeout() { return (millis() - timeoutStart) > timeout ? true : false; };
} mainT(100), selectionT(5000);

enum States
{
  detectIR,
  waitCommand,
  executeCommand
} state;

IRsend irsend(4);

IRrecv irrecv(13);
uint64_t IRCodes[4] = {0x20DF906F, 0x20DF40BF, 0x20DFC03F, 0x20DF10EF};
decode_results results;

MQTTClient client("d");
char deviceID[] = "d0001";
int action;

int rPin = 15;
int gPin = 5;
int bPin = 16;

void handleCommand(char *topic, byte *payload, unsigned int length)
{
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  };

  char c = (char)payload[0];
  switch (c)
  {
  case 'e':
    ledState(3);
    state = executeCommand;
    break;
  case 's':
    selectionT.start();
    ledState(1);
    break;
  case 'u':
    selectionT.start();
    ledState(0);
    break;
  }
}

void ledState(int i)
{
  switch (i)
  {
  case 0: //red
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, LOW);
    break;
  case 1: //green
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, LOW);
    break;
  case 2: //orange
    digitalWrite(rPin, HIGH);
    digitalWrite(gPin, HIGH);
    digitalWrite(bPin, LOW);
    break;
  case 3: //off
    digitalWrite(rPin, LOW);
    digitalWrite(gPin, LOW);
    digitalWrite(bPin, LOW);
    break;
  }
}

void setup()
{
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  Serial.begin(115200);
  States state = detectIR;
  irsend.begin();
  irrecv.enableIRIn();
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
        for (int i = 0; i < 5; i++)
        {
          if (results.value == IRCodes[i])
          {
            Serial.println("Command detected");
            ledState(2);
            action = i;
            client.publish("devices", deviceID);
            selectionT.start();
            state = waitCommand;
          }
        }
        irrecv.resume(); // Receive the next value
      }
      break;
    case waitCommand:
      if (selectionT.checkTimeout())
      {
        ledState(3);
        state = detectIR;
      }
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