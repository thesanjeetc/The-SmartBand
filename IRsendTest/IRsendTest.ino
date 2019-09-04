/* IRremoteESP8266: IRsendDemo - demonstrates sending IR codes with IRsend.
 *
 * Version 1.0 April, 2017
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

#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Example of data captured by IRrecvDumpV2.ino
uint16_t up[71] = {9026, 4456,  602, 556,  546, 584,  550, 1660,  578, 554,  582, 550,  552, 552,  582, 548,  576, 556,  548, 1662,  576, 1662,  598, 560,  554, 1658,  582, 1654,  584, 1654,  606, 1632,  608, 1630,  610, 550,  554, 1656,  604, 528,  574, 556,  578, 554,  550, 554,  580, 552,  572, 532,  580, 1656,  582, 522,  602, 1662,  576, 1660,  578, 1658,  580, 1656,  604, 1634,  604, 1634,  606, 39964,  9026, 2218,  578};  // NEC 20DF40BF
uint16_t down[67] = {9028, 4452,  604, 528,  596, 536,  578, 1658,  580, 524,  600, 532,  572, 532,  600, 532,  604, 528,  576, 1662,  578, 1660,  598, 532,  572, 1666,  574, 1664,  574, 1664,  596, 1642,  596, 1640,  598, 1640,  600, 1664,  576, 530,  604, 526,  578, 528,  606, 526,  598, 532,  570, 534,  600, 530,  572, 532,  602, 1662,  576, 1660,  578, 1658,  570, 1668,  602, 1636,  604, 1634,  604};  // NEC 20DFC03F
void setup() {
  irsend.begin();
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
}

void loop() {

  Serial.println("a rawData capture from IRrecvDumpV2");
  irsend.sendRaw(down, 67, 38);  // Send a raw data capture at 38kHz.
  delay(1000);
  irsend.sendRaw(up, 71, 38);
  delay(1000);
  
}
