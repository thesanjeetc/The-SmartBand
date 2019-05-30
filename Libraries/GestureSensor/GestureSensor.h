#ifndef GestureSensor_h
#define GestureSensor_h
#include <Arduino.h>

class GestureSensor{
public:
    GestureSensor(int pin);
    int detectGesture();
    int getGesture();
private:
    int numAve = 5;
    int lastVal, curVal, startVal = 0;
    unsigned long startm, lastm;
    int _pin;
    enum Gestures {tap, swipeUp, swipeDown, longTap};
    Gestures gesture;
};

#endif