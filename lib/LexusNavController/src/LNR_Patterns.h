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

/* Known Lexus Nav Remote button codes (exact length+content match)
   NOTE: some diagonal frames do NOT end with 0xFF (kept as-is) */

struct LNR_PatternDef {
  const char*   name;
  const uint8_t*bytes;
  uint8_t       len;
  int16_t       id;   // enum LNR_Button value
};

// Main button codes
static const uint8_t OFF_[]   = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0x7B,0xCB,0x7B,0x7B,0x4F};
static const uint8_t DISP_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0xCB,0xCB,0x7B,0x4F,0x4F,0xCB};
static const uint8_t INFO_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0x4B,0x7B,0x7B,0x4F,0x7B,0xFB};
static const uint8_t BACK_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0x4B,0x4F,0xCF,0xCF,0xCB,0x4F,0xFF};
static const uint8_t MENU_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0xCB,0xCF,0xCB,0xCF,0x4B,0x4F,0xFF};
static const uint8_t DEST_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0x7B,0x7B,0xCB,0x7B,0xCB};
static const uint8_t MAP_[]   = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0xCF,0xCB,0x4F,0xCB,0x4F,0xFF};
static const uint8_t MINUS_[] = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0xCB,0x7B,0x7B,0x4F,0xCB};
static const uint8_t PLUS_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0x4B,0x7B,0xCF,0xCF,0xCF,0x7B,0xFB};
static const uint8_t UP_[]    = {0x00,0x4B,0x4B,0xCB,0x4B,0x4B,0x4B,0xCF,0xCF,0xCF,0x4F,0xFF};
static const uint8_t DOWN_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0x4B,0xCF,0x7B,0xCF,0x4F,0xFF};
static const uint8_t RIGHT_[] = {0x00,0x4B,0x4B,0xCB,0x4B,0xCB,0x4B,0xCF,0x4F,0xCF,0x4F,0xFF};
static const uint8_t LEFT_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0x4B,0xCF,0xCB,0xCF,0x4F,0xFF};
static const uint8_t ENTER_[] = {0x00,0x4B,0x4B,0xCB,0x4B,0x4B,0x4F,0xCF,0xCF,0x4F,0x4F,0xFF};

// Joystick diagonal codes
static const uint8_t UPRIGHT_[]    = {0x00,0x4B,0x4B,0xCB,0x4B,0x4B,0x4B,0xCF,0x4F,0xCF,0xCB}; // no 0xFF
static const uint8_t UPLEFT_[]     = {0x00,0x4B,0x4B,0xCB,0x4B,0xCB,0x4F,0x7B,0x4F,0xCF,0x4F,0xFF};
static const uint8_t DOWNRIGHT_[]  = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0x4F,0x7B,0x7B,0xCF,0x4F,0xFF};
static const uint8_t DOWNLEFT_[]   = {0x00,0x4B,0x4B,0xCB,0x4B,0x7B,0x4F,0x7B,0x4B,0xCF,0xCB}; // no 0xFF

// Public enum in the same order as the table below:
enum LNR_Button : int16_t {
  LNR_BTN_OFF = 0, LNR_BTN_DISP, LNR_BTN_INFO, LNR_BTN_BACK, LNR_BTN_MENU,
  LNR_BTN_DEST, LNR_BTN_MAP, LNR_BTN_MINUS, LNR_BTN_PLUS,
  LNR_BTN_UP, LNR_BTN_DOWN, LNR_BTN_RIGHT, LNR_BTN_LEFT, LNR_BTN_ENTER,
  LNR_BTN_UP_RIGHT, LNR_BTN_UP_LEFT, LNR_BTN_DOWN_RIGHT, LNR_BTN_DOWN_LEFT,
  LNR_BTN_UNKNOWN = -1
};

static const LNR_PatternDef LNR_DEFAULT_PATTERNS[] = {
  {"OFF",   OFF_,   (uint8_t)sizeof(OFF_),   LNR_BTN_OFF},
  {"DISP",  DISP_,  (uint8_t)sizeof(DISP_),  LNR_BTN_DISP},
  {"INFO",  INFO_,  (uint8_t)sizeof(INFO_),  LNR_BTN_INFO},
  {"BACK",  BACK_,  (uint8_t)sizeof(BACK_),  LNR_BTN_BACK},
  {"MENU",  MENU_,  (uint8_t)sizeof(MENU_),  LNR_BTN_MENU},
  {"DEST",  DEST_,  (uint8_t)sizeof(DEST_),  LNR_BTN_DEST},
  {"MAP",   MAP_,   (uint8_t)sizeof(MAP_),   LNR_BTN_MAP},
  {"MINUS", MINUS_, (uint8_t)sizeof(MINUS_), LNR_BTN_MINUS},
  {"PLUS",  PLUS_,  (uint8_t)sizeof(PLUS_),  LNR_BTN_PLUS},
  {"UP",    UP_,    (uint8_t)sizeof(UP_),    LNR_BTN_UP},
  {"DOWN",  DOWN_,  (uint8_t)sizeof(DOWN_),  LNR_BTN_DOWN},
  {"RIGHT", RIGHT_, (uint8_t)sizeof(RIGHT_), LNR_BTN_RIGHT},
  {"LEFT",  LEFT_,  (uint8_t)sizeof(LEFT_),  LNR_BTN_LEFT},
  {"ENTER", ENTER_, (uint8_t)sizeof(ENTER_), LNR_BTN_ENTER},
  {"UP-RIGHT",    UPRIGHT_,   (uint8_t)sizeof(UPRIGHT_),   LNR_BTN_UP_RIGHT},
  {"UP-LEFT",     UPLEFT_,    (uint8_t)sizeof(UPLEFT_),    LNR_BTN_UP_LEFT},
  {"DOWN-RIGHT",  DOWNRIGHT_, (uint8_t)sizeof(DOWNRIGHT_), LNR_BTN_DOWN_RIGHT},
  {"DOWN-LEFT",   DOWNLEFT_,  (uint8_t)sizeof(DOWNLEFT_),  LNR_BTN_DOWN_LEFT}
};
static const uint8_t LNR_DEFAULT_COUNT = sizeof(LNR_DEFAULT_PATTERNS)/sizeof(LNR_DEFAULT_PATTERNS[0]);
