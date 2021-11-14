/* Copyright 2017, 2018 David Conran

  An IR LED circuit *MUST* be connected to the ESP8266 on a pin
  as specified by kIrLed below.

  TL;DR: The IR LED needs to be driven by a transistor for a good result.

  Suggested circuit:
      https://github.com/crankyoldgit/IRremoteESP8266/wiki#ir-sending

  Common mistakes & tips:
*   * Don't just connect the IR LED directly to the pin, it won't
      have enough current to drive the IR LED effectively.
*   * Make sure you have the IR LED polarity correct.
      See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
*   * Typical digital camera/phones can be used to see if the IR LED is flashed.
      Replace the IR LED with a normal LED if you don't have a digital camera
      when debugging.
*   * Avoid using the following pins unless you really know what you are doing:
*     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
*     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
*     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
*   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
      for your first time. e.g. ESP-12 etc.
*/
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Panasonic.h>

const uint16_t kIrLed = 26;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRPanasonicAc ac(kIrLed);  // Set the GPIO used for sending messages.

void printState() {
  // Display the settings.
  Serial.println("Panasonic A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kPanasonicAcStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
  Serial.println();
  Serial.println();
}

void setup() {
  ac.begin();
  Serial.begin(115200);
  delay(200);

  // Set up what we want to send. See ir_Panasonic.cpp for all the options.
  //  Serial.println("Default state of the remote.");
  //  printState();

  sendHeat();

}
void sendCool() {
  sendOrder(kPanasonicAcCool, 27);
  delay(1000);
  sendOffTimer(kPanasonicAcCool, 27);
}

void sendHeat() {
  sendOrder(kPanasonicAcHeat, 24);
  delay(1000);
  sendOffTimer(kPanasonicAcHeat, 24);
}

void sendOrder(uint8_t sendMode, uint8_t sendTemp) {

  //  ac.setModel(kPanasonicRkr);
  //  ac.on();
  //  ac.setFan(kPanasonicAcFanAuto);
  //  ac.setMode(kPanasonicAcCool);
  //  ac.setTemp(26);
  //  ac.setSwingVertical(kPanasonicAcSwingVAuto);
  //  ac.setSwingHorizontal(kPanasonicAcSwingHAuto);

  Serial.println("Setting desired state for A/C.");
  ac.setModel(kPanasonicJke);
  ac.on();
  ac.setMode(sendMode);
  ac.setTemp(sendTemp);
  ac.setFan(kPanasonicAcFanAuto);
  ac.setSwingVertical(kPanasonicAcSwingVAuto);
  ac.setSwingHorizontal(kPanasonicAcSwingHAuto);
  ac.send();
  printState();
}

void sendOffTimer(uint8_t sendMode, uint8_t sendTemp) {
  ac.setOffTimer(60);
  ac.send();
  printState();
}


void loop() {
  //  // Now send the IR signal.
  //#if SEND_PANASONIC_AC
  //  Serial.println("Sending IR command to A/C ...");
  //  ac.send();
  //#endif  // SEND_PANASONIC_AC
  //  printState();
  //  delay(5000);
}
