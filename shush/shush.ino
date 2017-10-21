/*
Simple momentary key control from a single momentary switch mounted to my desk.
Used to control Shush: http://mizage.com/shush/

It's a Push-to-talk/toggle/cough button for microphones.
Very handy for anybody who does a lot of group audio/videoconferencing.

This isn't updated any more, but I'm leaving it as a reference for a simple one-button app.

*/

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
