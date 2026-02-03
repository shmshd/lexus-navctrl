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

#pragma once
#include <Arduino.h>
#include <LNR_Patterns.h>  // brings LNR_Button and default table

// Defaults matching your working sketch
#ifndef LNR_MAX_FRAME
#define LNR_MAX_FRAME 24
#endif

#ifndef LNR_FRAME_TIMEOUT_MS
#define LNR_FRAME_TIMEOUT_MS 40
#endif

#ifndef LNR_DEBOUNCE_MS
#define LNR_DEBOUNCE_MS 180
#endif

class LexusNavController {
public:
  explicit LexusNavController(Stream& in) : _in(in) {}

  // Initialize internal state; does NOT set baud on the Stream
  void begin(bool showHex = true, bool autoPrint = true) {
    _frameLen = 0;
    _lastByteMs = millis();
    _frameTimeoutMs = LNR_FRAME_TIMEOUT_MS;
    _debounceMs = LNR_DEBOUNCE_MS;
    _showHex = showHex;
    _autoPrint = autoPrint;

    // load built-in patterns
    _count = 0;
    for (uint8_t i = 0; i < LNR_DEFAULT_COUNT && _count < MAX_PATTERNS; ++i) {
      _patterns[_count].name  = LNR_DEFAULT_PATTERNS[i].name;
      _patterns[_count].bytes = LNR_DEFAULT_PATTERNS[i].bytes;
      _patterns[_count].len   = LNR_DEFAULT_PATTERNS[i].len;
      _patterns[_count].id    = LNR_DEFAULT_PATTERNS[i].id;
      _lastSeen[_count] = 0;
      _count++;
    }
    press = LNR_BTN_UNKNOWN;
    _evtReady = false;
  }

  // Setters to match the original behavior if you ever need to tweak
  void setFrameTimeoutMs(uint16_t ms) { _frameTimeoutMs = ms; }
  void setDebounceMs(uint16_t ms)     { _debounceMs     = ms; }
  void setShowHex(bool enable)        { _showHex = enable; }
  void setAutoPrint(bool enable)      { _autoPrint = enable; }

  // Add a custom pattern at runtime (id can be >=100 for user space)
  bool addPattern(const char* name, const uint8_t* bytes, uint8_t len, int16_t id) {
    if (_count >= MAX_PATTERNS || len == 0 || len > LNR_MAX_FRAME) return false;
    _patterns[_count].name = name;
    _patterns[_count].bytes = bytes;
    _patterns[_count].len = len;
    _patterns[_count].id = id;
    _lastSeen[_count] = 0;
    _count++;
    return true;
  }

  // Call this often in loop()
  void update() {
    _evtReady = false; // one-shot

    while (_in.available() > 0) {
      int v = _in.read();
      if (v < 0) break;

      if (_frameLen < LNR_MAX_FRAME) {
        _frame[_frameLen++] = (uint8_t)v;
      } else {
        // overflow safeguard: flush current frame and restart
        _flushFrameIfAny();
        _frame[0] = (uint8_t)v;
        _frameLen = 1;
      }
      _lastByteMs = millis();
    }

    if (_frameLen > 0 && (millis() - _lastByteMs > _frameTimeoutMs)) {
      _flushFrameIfAny();
    }
  }

  // Returns true exactly once per new event; also outputs the button id
  bool poll(LNR_Button& out) {
    if (!_evtReady) return false;
    out = press;
    _evtReady = false;
    return true;
  }

  // “Level-style” check (true only for the update() that created it)
  bool available() const { return _evtReady; }

  // Last detected button
  LNR_Button press = LNR_BTN_UNKNOWN;

  // Raw frame (for debugging/printing)
  const uint8_t* lastFrame() const { return _frame; }
  uint8_t lastFrameLen() const { return _frameLenAtEmit; }

private:
  struct PatRef {
    const char* name;
    const uint8_t* bytes;
    uint8_t len;
    int16_t id;
  };

  static const uint8_t MAX_PATTERNS = 48; // default + some custom

  Stream& _in;

  // Assembler state
  uint8_t  _frame[LNR_MAX_FRAME];
  uint8_t  _frameLen = 0;
  uint8_t  _frameLenAtEmit = 0;
  uint32_t _lastByteMs = 0;

  // Params
  uint16_t _frameTimeoutMs = LNR_FRAME_TIMEOUT_MS;
  uint16_t _debounceMs     = LNR_DEBOUNCE_MS;

  // Patterns
  PatRef   _patterns[MAX_PATTERNS];
  uint8_t  _count = 0;
  uint32_t _lastSeen[MAX_PATTERNS] = {0};

  // Event state
  bool _evtReady = false;

  // Output flags (to mimic original sketch)
  bool _showHex   = true;
  bool _autoPrint = true;

  // Helpers
  void   _flushFrameIfAny();
  int    _matchIndex() const;
  static bool _eq(const uint8_t* a, const uint8_t* b, uint8_t n);
  void   _printHex(const uint8_t* f, uint8_t n) const;
};
