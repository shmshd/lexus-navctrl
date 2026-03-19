#include <SoftwareSerial.h>
#include <LexusNavController.h>
#include <HID-Project.h>

#define DATA_PIN 8 // D8

// RX = D8 (remote line), TX = NULL (unused)
SoftwareSerial navUart(DATA_PIN, NULL);
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
  Consumer.begin();
  Keyboard.begin();
  System.begin();
}

void loop() {
  nav.update();

  if (nav.available()) {
    switch (nav.press) {
      // Main buttons
      case LNR_BTN_OFF:
          // Keyboard.press(KEY_LEFT_GUI);
          // Keyboard.press(KEY_L);
          // Keyboard.releaseAll();
        System.write(HID_SYSTEM_SETUP);
        Serial.println(F("OFF")); 
        break;
      case LNR_BTN_DISP:
        System.write(HID_SYSTEM_SETUP);
        Serial.println(F("DISP"));   
        break;
      case LNR_BTN_INFO:
        System.write(HID_SYSTEM_SPEAKER_MUTE);
        Serial.println(F("INFO"));   
        break;
      case LNR_BTN_BACK:
          Keyboard.write(HID_KEYBOARD_ESCAPE);
          Serial.println(F("BACK"));   
        break;
      case LNR_BTN_MENU:
          Serial.println(F("MENU"));   
        break;
      case LNR_BTN_DEST:
          Serial.println(F("DEST"));   
        break;
      case LNR_BTN_MAP:
          Consumer.write(MEDIA_PLAY_PAUSE);
          Serial.println(F("MAP"));    
        break;
      case LNR_BTN_MINUS:
          Consumer.write(MEDIA_VOLUME_DOWN);  
          Serial.println(F("MINUS"));  
        break;
      case LNR_BTN_PLUS: 
          Consumer.write(MEDIA_VOLUME_UP);  
          Serial.println(F("PLUS"));   
        break;
      case LNR_BTN_ENTER:
          Keyboard.write(KEY_ENTER);
          Serial.println(F("ENTER"));  
        break;

      // Joystick
      case LNR_BTN_UP:
        Keyboard.write(KEY_UP_ARROW);
        Serial.println(F("UP"));
        break;
      case LNR_BTN_DOWN:
        Keyboard.write(KEY_DOWN_ARROW);
        Serial.println(F("DOWN"));   
        break;
      case LNR_BTN_RIGHT:
        Keyboard.write(KEY_RIGHT_ARROW);
        Serial.println(F("RIGHT"));  
        break;
      case LNR_BTN_LEFT:
        Keyboard.write(KEY_LEFT_ARROW);
          Serial.println(F("LEFT"));   
        break;
      case LNR_BTN_UP_RIGHT:
          Keyboard.write(KEY_UP_ARROW);
          Keyboard.write(KEY_RIGHT_ARROW);
          Serial.println(F("UP-RIGHT"));   
        break;
      case LNR_BTN_UP_LEFT:
          Keyboard.write(KEY_UP_ARROW);
          Keyboard.write(KEY_LEFT_ARROW);
          Serial.println(F("UP-LEFT"));    
        break;
      case LNR_BTN_DOWN_RIGHT:
          Keyboard.write(KEY_DOWN_ARROW);
          Keyboard.write(KEY_RIGHT_ARROW);
          Serial.println(F("DOWN-RIGHT")); 
        break;
      case LNR_BTN_DOWN_LEFT:
          Keyboard.write(KEY_DOWN_ARROW);
          Keyboard.write(KEY_LEFT_ARROW);
          Serial.println(F("DOWN-LEFT"));  
       break;

      default: {
        // Unknown code: show raw HEX so you can capture new patterns
        Serial.print(F("UNKNOWN  "));
        printHexFrame(nav.lastFrame(), nav.lastFrameLen());
        Serial.println();
      } break;
    }
  }
}

