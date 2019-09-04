#include <Arduino.h>
/******************************************************************************
  SoftPot_Example.ino
  Example sketch for SparkFun's soft membrane potentiometer
  (https://www.sparkfun.com/products/8680)
  Jim Lindblom @ SparkFun Electronics
  April 28, 2016

  - Connect the softpot's outside pins to 5V and GND (the outer pin with an arrow
  indicator should be connected to GND).
  - Connect the middle pin to A0.

  As the voltage output of the softpot changes, a line graph printed to the
  serial monitor should match the wiper's position.

  Development environment specifics:
  Arduino 1.6.7
******************************************************************************/
const int p = A0;
int state = 0;
int lastVal = 0;
int num = 3;
int diffVal = 0;
int curVal = 0;
int startVal = 0;
unsigned long startMillis;
unsigned long currentMillis;
unsigned long diff;
enum States {getInput, checkInput};

void setup()
{
  Serial.begin(115200);
  pinMode(p, INPUT);
  pinMode(0, OUTPUT);
  States state = getInput;
}

void loop()
{
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  diff = currentMillis - startMillis;
  
  if (diff >= 10) {
    switch(state){
      case(getInput):
        curVal = analogRead(p)/10;
        if (curVal > 3){
          startVal = curVal;
          lastVal = curVal;
          while(curVal > 3){
            for(int i = 1; i < 5; ++i)
            {
              curVal += analogRead(p)/10;
              delay(2);
            }
            curVal /= 5;
            //curVal = analogRead(p)/10;
            if(abs(lastVal - curVal) > 6){
              break;
            }
            lastVal = curVal;
            yield();
          }
          state = checkInput;
        }
      break;
      case(checkInput):
        if(abs(startVal - lastVal)<6){
          Serial.println("Tap");
        }else if (startVal < lastVal){
          Serial.println("Swipe up");
        }else{
          Serial.println("Swipe down");
        }
        state = getInput;
      break;
    }
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
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
