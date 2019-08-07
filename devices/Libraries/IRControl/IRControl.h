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
        void sendNEC(uint64_t);
    private:
        const uint16_t kRecvPin;
        const uint16_t kCaptureBufferSize = 1024;
        const uint16_t kMinUnknownSize = 12;
        const uint8_t kTimeout = 15;

}

#endif