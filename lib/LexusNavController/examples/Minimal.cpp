/*
  LexusNavController - Arduino library to interface with the Lexus IS200/IS300/Toyota Altezza Navigation Controller (1998–2005)

  Author: Anyzun (https://github.com/anyzunstudios)
  License: MIT
  Version: 1.0.0
  Year: 2025

  Description:
  This library allows decoding of button presses from the Lexus/Toyota Navigation Remote Controller,
  originally used with the OEM GPS navigation system. The controller communicates via UART (2400 baud, 8N1, 5V TTL).
  Compatible with Arduino (Uno, Mega, Nano) and 3.3V MCUs (ESP32, etc.) using a voltage divider.

  Credits:
  If you use this library in your project, please consider giving credit by linking back to:
  https://github.com/anyzunstudios/Arduino-LexusNavController
*/

#include <SoftwareSerial.h>
#include <LexusNavController.h>

// RX=D8 (remote line), TX=NULL (unused)
SoftwareSerial navUart(8, NULL);
LexusNavController nav(navUart);

void setup() {
  Serial.begin(115200);
  navUart.begin(2400);

  // Same defaults as your working sketch: showHex=true, autoPrint=true
  nav.begin(/*showHex=*/true, /*autoPrint=*/true);

  // If you ever tweak: nav.setFrameTimeoutMs(40); nav.setDebounceMs(180);
  Serial.println(F("Minimal ready: 2400bps RX=D8, strict match, HEX ON"));
}

void loop() {
  nav.update();
  // Printing is handled by the library (exactly like your sketch).
  // If you prefer to decide when to print, call nav.setAutoPrint(false) and use example 02.
}

