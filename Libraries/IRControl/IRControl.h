#ifndef IRControl_h
#define IRControl_h
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRutils.h>

class IRControl{
    public:
        IRControl(uint16_t recvPin, uint16_t sendPin);
        IRControl(uint16_t recvPin);
        uint64_t detectIR();
        void send(uint64_t);
    private:
        const uint16_t kRecvPin;

}

#endif