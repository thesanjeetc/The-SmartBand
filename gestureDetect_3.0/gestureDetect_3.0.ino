#include <Arduino.h>
#include "GestureSensor.h"

unsigned long startMillis;
unsigned long currentMillis;
unsigned long diff;
enum States {getInput, checkInput};
States state;
GestureSensor sensor(A0);

void setup()
{
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  States state = getInput;
}

void loop()
{
  currentMillis = millis();
  diff = currentMillis - startMillis;
  
  if (diff >= 10) {
    switch(state){
      case(getInput):
        sensor.detectGesture();
        break;
    }
    startMillis = currentMillis;
  }
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
