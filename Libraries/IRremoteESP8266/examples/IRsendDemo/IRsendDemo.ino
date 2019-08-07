/* IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.
 *
 * Version 1.1 January, 2019
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009,
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 *
 * An IR LED circuit *MUST* be connected to the ESP8266 on a pin
 * as specified by kIrLed below.
 *
 * TL;DR: The IR LED needs to be driven by a transistor for a good result.
 *
 * Suggested circuit:
 *     https://github.com/markszabo/IRremoteESP8266/wiki#ir-sending
 *
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[119] = {1320, 374,  1324, 368,  470, 1224,  1316, 378,  1322, 372,  478, 1216,  474, 1220,  470, 1224,  476, 1218,  472, 1222,  478, 1216,  1324, 7202,  1322, 372,  1318, 376,  474, 1220,  1320, 372,  1318, 376,  472, 1222,  478, 1216,  474, 1220,  470, 1224,  476, 1218,  472, 1222,  1318, 7206,  1318, 376,  1324, 370,  470, 1222,  1328, 368,  1322, 370,  478, 1216,  474, 1220,  470, 1224,  476, 1220,  470, 1224,  478, 1216,  1324, 7202,  1322, 372,  1318, 376,  472, 1220,  1320, 374,  1326, 368,  470, 1222,  478, 1216,  472, 1222,  478, 1216,  474, 1218,  470, 1224,  1316, 7210,  1324, 370,  1320, 374,  476, 1218,  1322, 372,  1316, 378,  472, 1222,  478, 1216,  474, 1220,  480, 1214,  476, 1220,  470, 1224,  1326};
// Example Samsung A/C state captured from IRrecvDumpV2.ino
uint8_t samsungState[kSamsungAcStateLength] = {
    0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
    0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};

void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {
  irsend.sendRaw(rawData, 119, 38);  // Send a raw data capture at 38kHz.
  delay(2000);
}
