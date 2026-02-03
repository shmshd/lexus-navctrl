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

// Example custom code (replace with a real one)
const uint8_t MY_NEW_CODE[] = {0x01,0x02,0x03,0x04};

void setup() {
  Serial.begin(115200);
  navUart.begin(2400);
  nav.begin(true, true);

  // Add a new pattern with a custom ID (>=100 recommended for user space)
  nav.addPattern("MYBTN", MY_NEW_CODE, sizeof(MY_NEW_CODE), 100);

  Serial.println(F("AddCustomButtonCode ready."));
}

void loop() {
  nav.update();
  // Library will auto-print on match; to handle yourself:
  /*
  LNR_Button btn;
  if (nav.poll(btn) && btn == (LNR_Button)100) {
    Serial.println(F("MYBTN detected"));
  }
  */
}

