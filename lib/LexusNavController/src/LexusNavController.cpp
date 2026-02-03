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

#include <LexusNavController.h>

void LexusNavController::_flushFrameIfAny() {
  if (_frameLen == 0) return;

  const int idx = _matchIndex();
  const uint32_t now = millis();

  if (idx >= 0) {
    // per-button debounce
    if (now - _lastSeen[idx] >= _debounceMs) {
      _lastSeen[idx] = now;
      // emit event
      press = (LNR_Button)_patterns[idx].id;
      _evtReady = true;
      _frameLenAtEmit = _frameLen;

      if (_autoPrint) {
        Serial.print(_patterns[idx].name);
        if (_showHex) {
          Serial.print(F("  "));
          _printHex(_frame, _frameLen);
        } else {
          Serial.println();
        }
      }
    }
  } else {
    // unknown with global debounce (shared timer index: use slot 0 safely)
    static uint32_t lastUnknown = 0;
    if (now - lastUnknown >= _debounceMs) {
      lastUnknown = now;
      // keep press as last known or UNKNOWN (we won't emit a named event)
      if (_autoPrint) {
        Serial.print(F("UNKNOWN"));
        if (_showHex) {
          Serial.print(F("  "));
          _printHex(_frame, _frameLen);
        } else {
          Serial.println();
        }
      }
    }
  }

  _frameLen = 0; // reset buffer
}

int LexusNavController::_matchIndex() const {
  for (uint8_t i = 0; i < _count; ++i) {
    if (_patterns[i].len == _frameLen && _eq(_patterns[i].bytes, _frame, _frameLen)) {
      return (int)i;
    }
  }
  return -1;
}

bool LexusNavController::_eq(const uint8_t* a, const uint8_t* b, uint8_t n) {
  for (uint8_t i = 0; i < n; ++i) if (a[i] != b[i]) return false;
  return true;
}

void LexusNavController::_printHex(const uint8_t* f, uint8_t n) const {
  for (uint8_t i = 0; i < n; ++i) {
    if (f[i] < 0x10) Serial.print('0');
    Serial.print(f[i], HEX);
    if (i + 1 < n) Serial.print(' ');
  }
  Serial.println();
}
