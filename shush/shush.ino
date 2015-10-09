#define OSX 0

#include "Keyboard.h"
#include "HID.h"

int platform = OSX;

void setup() {
  // set pin 2 to input with pullup resistor, high unless grounded
  pinMode(2, INPUT_PULLUP);
  Keyboard.begin();
}

void loop() {
  while (digitalRead(2) == LOW) {
  }
  
  while (digitalRead(2) == HIGH) {
    Keyboard.releaseAll();
  }
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(KEY_LEFT_CTRL);
}
