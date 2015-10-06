/*

 Toggles a motorized sit/stand desk between sitting position and standing position.
 Hardware/Circuit Requirements:
   (2) 5v coil, 20V+/0.5A+ relay (or some electrically activated switch/transistor that matches the values of your desk controls)
   (1) motor-controlled desk with a simple 2-direction, 3-conductor control interface
   (1) normally-open momentary switch
   (2) normally-open reed switches
   (2) arrays of (14) 6.4mm x 1.8mm disc neodymium magnets taped to max/min (or some magnet strong enough to close the reed switch)
   wires and cables and stuff
   (I used:
     - A leviton-type RJ45 jack to connect the circuit to the controller
     - 3.5mm stereo jacks to connect the button to the circuit

 This would be more efficient/expensive with:
   - reed switches at the max and min points
   - some way to directly measure height/distance.

 created some October Sunday in 2015
 by Justin Farris
 */


// first section of globals can be configured to meet your desk configuration
const int B = 2; // toggle button
const int Ru = 3; // up relay
const int Rd = 4;//9; // down relay
const int Sd = 5; // sitting reed switch
const int Su = 6; // standing reed switch
int Tm = 18500; // max travel time from max to min
int Td = 4500; // time from bottom reed switch to min height
int Tu = 12900; // time from top reed switch to min height
int Offset = 250; // how long after reed switch is closed to stop moving 
int bDelay = 350; // how long to wait between button press detection (to prevent accidental double-click)

// this is probably an inefficient use of globals, but idgaf
int Bs = 0; // button state
int Sds = 0; // down reed switch state
int Sus = 0; // up reed switch state
int RP = Rd; // default relay for positive movement within move() (down)
int RN = Ru; // default relay for negative movement within move(), (up)
int S = Sd; // default reed switch to test within move() (sitting)
int Ss = Sds; // default reed switch state within move() (down/Sds)
int T1 = Tm; // default time for first direction of seek within move()
int T2 = Td; // default time for second direction of seek within move()

char lastDir = 'n'; // used to toggle direction after a forced stop with the button

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(Ru, OUTPUT);
  pinMode(Rd, OUTPUT);
  pinMode(B, INPUT_PULLUP);
  pinMode(Su, INPUT_PULLUP);
  pinMode(Sd, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {

  Bs = digitalRead(B);
  Sds = digitalRead(Sd);
  Sus = digitalRead(Su);
  if (Bs == LOW) { // if the button is pressed...
//    Serial.println("Button is pressed, so I'll do something.");
//    Serial.println("Sd & Su are: ");
//    Serial.println(Sds);
//    Serial.println(Sus);

// If Su and Sd are closed, something is wrong so don't do anything:
    if ((Sus == LOW) && (Sds == LOW)) {
//      Serial.println("Both reed switches are closed.\nI don't know what to do, so I'll do nothing.");
    }

// Else If Su is closed, move to sitting position
    else if (Sus == LOW) {
//      Serial.println("Standing switch is closed. Moving to sitting position.");
        move('d');
    }

// Else If Sd is closed, move to standing position
    else if (Sds == LOW) {
//      Serial.println("Sitting switch is closed. Moving to standing position.");
      move('u');
    }
    
// Else If Su and Sd are open, move to bottom then move up to sitting position, or stop at sit position if we pass it.
    else if ((Sus == HIGH) && (Sds == HIGH)) {
//      Serial.println("No reed switches closed.\nNo idea where I am, so I'll try to find a switch one way or the other.");
      if (lastDir == 'u') {
        move('d');
      }
      else {
        move('u');
      }
    }



}
}

void stop() {
  delay(50);
  digitalWrite(Rd, LOW);
  delay(50);
  digitalWrite(Ru, LOW);
  delay(50);
}

int move(char dir) {
    lastDir = 'd';
    RP = Rd;
    RN = Ru;
    S = Sd;
    Ss = Sds;
    T1 = Tm;
    T2 = Td;  
  if (dir == 'u') {
    lastDir = 'u';
    RP = Ru;
    RN = Rd;
    S = Su;
    Ss = Sus;
    T1 = Tm;
    T2 = Tm - Tu;
  }
//  Serial.print("DEBUG: direction is ");
//  Serial.println(dir);
//  Serial.print("DEBUG: RP is ");
//  Serial.println(RP);

// Start moving. If I find the reed switch, stop. If not, stop after max travel time
  unsigned long previousMillis = 0;
  unsigned long currentMillis = 0;
  int trip = 0;
  digitalWrite(RP, HIGH);
  previousMillis = millis();
  while (true) {
     currentMillis = millis();

     delay(bDelay);  //delay so that a lingering finger doesn't immediately negate a legit button-press
     
     Bs = digitalRead(B);
     Ss = digitalRead(S);
     
     if (Bs == LOW) {
       stop();
       delay(bDelay);
       return 1;
     }
    
     if (Ss == LOW) {
       delay(Offset); //replace with specific offsets and check trip count if switches aren't identical
       //or replace offset with overshoot-and-refind
       stop();
       return 0;
     }
     else if ((trip == 0) && (currentMillis - previousMillis > T1)) {
       previousMillis = currentMillis;
       stop();
       digitalWrite(RN, HIGH);
       trip++;
     }
     else if ((trip == 1) && (currentMillis - previousMillis > T2)) {
         stop();
         return 1;
         
       }
  }
  
  return 0;
}

