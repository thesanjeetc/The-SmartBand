#include "GestureSensor.h"
#include <Arduino.h>

GestureSensor::GestureSensor(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}

int GestureSensor::detectGesture()
{
    curVal = analogRead(_pin)/10;
        startm = millis();
        if (curVal > 3){
          startVal = curVal;
          lastVal = curVal;
          while(curVal > 3){
            for(int i = 1; i < numAve; ++i)
            {
              curVal += analogRead(_pin)/10;
              delay(2);
            }
            curVal /= numAve;
            if(abs(lastVal - curVal) > 6){
              break;
            }
            lastVal = curVal;
            yield();
          }
          lastm = millis();
          int g =  getGesture();
          return g;
        }
      return -1;  
}

int GestureSensor::getGesture(){
    if(abs(startVal - lastVal) < 6){
        if(lastm-startm > 600){
          Serial.println("Long Tap");
          gesture = longTap;
        }else{
          Serial.println("Tap");
          gesture = tap;
        }
    }else if (startVal < lastVal){
      Serial.println("Swipe Up");
      gesture = swipeUp;
    }else{
      Serial.println("Swipe Down");
      gesture = swipeDown;
    }
    Serial.println(String(startVal)+' '+String(lastVal)+' '+String(startm)+' '+String(lastm)+' '+String(startm-lastm));
    return gesture;
}
