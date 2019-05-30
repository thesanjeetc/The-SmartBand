#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include "IRControl.h"

IRControl::IRControl(uint16_t recvPin, uint16_t sendPin){
    IRrecv irrecv(recvPin);
}