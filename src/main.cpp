#include <SoftwareSerial.h>
#include <LexusNavController.h>

void setup() {
    SoftwareSerial navUart(8, NULL);
    LexusNavController nav(navUart);
}

void loop() {
}
