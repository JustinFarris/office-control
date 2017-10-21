/*
Simple momentary key controls from momentary switches mounted to my desk.
Used (among other things) to control Shush: http://mizage.com/shush/

It's a Push-to-talk/toggle/cough button for microphones.
Very handy for anybody who does a lot of group audio/videoconferencing.

Optional: extra pins shift the function of the single button (using a multi-throw switch)

Todo:
* Add rocker switch to toggle shush instead of double-press
* Combine with desk_control_toggle to do both with one chip (raise/lower desk)
* Fade/larson LEDs
* Get input from shush and other apps to activate LEDs appropriately

*/

#define OSX 0
#include "Keyboard.h"
#include "HID.h"

typedef struct t  {
    unsigned long tStart;
    unsigned long tTimeout;
};

//Tasks and their Schedules.
t t_func100 = {0, 100}; //Run every 100ms
t t_func1000 = {0, 1000}; //Run every second

bool tCheck (struct t *t ) {
  if (millis() > t->tStart + t->tTimeout) return true;    
}

void tRun (struct t *t) {
    t->tStart = millis();
}

int platform = OSX;

//*const int green = 2; // make this another input shift?
const int mode_shift = 3; //shift switch down (shift), white
const int mode_shush = 4; //shift switch up (shush/PTT), red
const int ptt = 5; //ptt, right button, yellow
const int up = 6; // vona/desk-up? top button, brown
const int down = 7; // harley/desk-down? bottom button, blue
const int left = 8; // ?, left button, orange

int pttState = HIGH;
long lastPttDebounceTime = 0;
int upState = HIGH;
long lastUpDebounceTime = 0;
int downState = HIGH;
long lastDownDebounceTime = 0;
int leftState = HIGH;
long lastLeftDebounceTime = 0;

long debounceDelay = 500;


const int ledr = A0; // right LED
const int ledt = A1; // top LED
const int ledb = A2; // bottm LED
const int ledl = A3; // left LED

int bright = 0;
int fade = 5;

void setup() {
  Serial.begin(9600);
  // set pin 2 to input with pullup resistor, high unless grounded
  pinMode(ptt, INPUT_PULLUP);
  pinMode(mode_shift, INPUT_PULLUP);
  pinMode(mode_shush, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(ledr, OUTPUT);
  pinMode(ledt, OUTPUT);
  pinMode(ledb, OUTPUT);
  pinMode(ledl, OUTPUT);
  Keyboard.begin();
}

void out(String debugInfo){
  Serial.println(debugInfo);
}

void halt(){
  Keyboard.releaseAll();
}

void hold_shush(){
  while ((digitalRead(ptt) == LOW) && (digitalRead(mode_shush) == LOW)){
    digitalWrite(ledr,HIGH);
  }
}

void shush_action(){
    out("Starting shush action.");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_LEFT_CTRL);
}

void shift_action(){
  out("Starting shift action.");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('Y');
}

void vona_action(){
  out("Starting vona action.");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('I');
}

void fart_action(){
  out("Starting fart action.");
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press('F');
}

void ptt_pressed(){
  if ((digitalRead(mode_shift) == HIGH) && (digitalRead(mode_shush) == HIGH)){
    vona_action();
  }
  if ((digitalRead(mode_shift) == HIGH) && (digitalRead(mode_shush) == LOW)){
    shush_action();
    hold_shush();
    digitalWrite(ledr,LOW);
  }
  if ((digitalRead(mode_shift) == LOW) && (digitalRead(mode_shush) == HIGH)){
    shift_action();
  }
}

void down_pressed(){
    if ((digitalRead(mode_shift) == HIGH) && (digitalRead(mode_shush) == LOW)){
    vona_action();
  }
  if ((digitalRead(mode_shift) == LOW) && (digitalRead(mode_shush) == HIGH)){
    fart_action();
  }
}

void up_pressed(){
    if ((digitalRead(mode_shift) == HIGH) && (digitalRead(mode_shush) == LOW)){
   // harley_action();
  }
  if ((digitalRead(mode_shift) == LOW) && (digitalRead(mode_shush) == HIGH)){
    shift_action();
  }
}

void left_pressed(){
    if ((digitalRead(mode_shift) == HIGH) && (digitalRead(mode_shush) == LOW)){
   // harley_action();
  }
  if ((digitalRead(mode_shift) == LOW) && (digitalRead(mode_shush) == HIGH)){
    shift_action();
  }
}

void loop() {

//digitalWrite(ledr,HIGH);
//digitalWrite(ledt,HIGH);
//digitalWrite(ledb,HIGH);
//digitalWrite(ledl,HIGH);

// below stuff should cycle the lights
/*
if (tCheck(&t_func100)) {
//      func100();
  analogWrite(ledt,bright);
  bright = bright + fade;
  if (bright <= 0 || bright >= 255) {
    fade = -fade;
  }

      tRun(&t_func100);
    }
*/

//  out("Starting loop().");
  halt();

   pttState = digitalRead(ptt);
  if ( (millis() - lastPttDebounceTime) > debounceDelay || (digitalRead(mode_shush) == LOW))  {
  if (digitalRead(ptt) == LOW){
    ptt_pressed();
    lastPttDebounceTime = millis();
  }
  }

  downState = digitalRead(down);
  if ( (millis() - lastDownDebounceTime) > debounceDelay) {
  if (digitalRead(down) == LOW){
    down_pressed();
    lastDownDebounceTime = millis();
  }
  }

  upState = digitalRead(up);
  if ( (millis() - lastUpDebounceTime) > debounceDelay) {
  if (digitalRead(up) == LOW){
    up_pressed();
    lastUpDebounceTime = millis();
  }
  }

  leftState = digitalRead(left);
  if ( (millis() - lastLeftDebounceTime) > debounceDelay) {
  if (digitalRead(left) == LOW){
    left_pressed();
    lastLeftDebounceTime = millis();
  }
  }
  
}
