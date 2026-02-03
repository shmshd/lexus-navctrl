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

// RX = D8 (remote line), TX = NULL (unused)
SoftwareSerial navUart(8, NULL);
LexusNavController nav(navUart);

static void printHexFrame(const uint8_t* f, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) {
    if (f[i] < 0x10) Serial.print('0');
    Serial.print(f[i], HEX);
    if (i + 1 < n) Serial.print(' ');
  }
}

void setup() {
  Serial.begin(115200);
  navUart.begin(2400);
  // We handle printing ourselves (no auto-print) and disable global HEX debug
  nav.begin(/*showHex=*/false, /*autoPrint=*/false);
  nav.setFrameTimeoutMs(40);
  nav.setDebounceMs(180);
  Serial.println(F("SimpleSwitch example ready."));
}

void loop() {
  nav.update();

  if (nav.available()) {
    switch (nav.press) {
      // Main buttons
      case LNR_BTN_OFF:    Serial.println(F("OFF"));    break;
      case LNR_BTN_DISP:   Serial.println(F("DISP"));   break;
      case LNR_BTN_INFO:   Serial.println(F("INFO"));   break;
      case LNR_BTN_BACK:   Serial.println(F("BACK"));   break;
      case LNR_BTN_MENU:   Serial.println(F("MENU"));   break;
      case LNR_BTN_DEST:   Serial.println(F("DEST"));   break;
      case LNR_BTN_MAP:    Serial.println(F("MAP"));    break;
      case LNR_BTN_MINUS:  Serial.println(F("MINUS"));  break;
      case LNR_BTN_PLUS:   Serial.println(F("PLUS"));   break;
      case LNR_BTN_UP:     Serial.println(F("UP"));     break;
      case LNR_BTN_DOWN:   Serial.println(F("DOWN"));   break;
      case LNR_BTN_RIGHT:  Serial.println(F("RIGHT"));  break;
      case LNR_BTN_LEFT:   Serial.println(F("LEFT"));   break;
      case LNR_BTN_ENTER:  Serial.println(F("ENTER"));  break;

      // Joystick diagonals
      case LNR_BTN_UP_RIGHT:   Serial.println(F("UP-RIGHT"));   break;
      case LNR_BTN_UP_LEFT:    Serial.println(F("UP-LEFT"));    break;
      case LNR_BTN_DOWN_RIGHT: Serial.println(F("DOWN-RIGHT")); break;
      case LNR_BTN_DOWN_LEFT:  Serial.println(F("DOWN-LEFT"));  break;

      default: {
        // Unknown code: show raw HEX so you can capture new patterns
        Serial.print(F("UNKNOWN  "));
        printHexFrame(nav.lastFrame(), nav.lastFrameLen());
        Serial.println();
      } break;
    }
  }
}


