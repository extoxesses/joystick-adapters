/**
  Copyright (c) 2023 Alex Badan
  
  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation
  files (the "Software"), to deal in the Software without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the
  Software is furnished to do so, subject to the following
  conditions:
  
  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
*/
#include <Joystick.h>

#define UP    4
#define DOWN  5
#define LEFT  6
#define RIGHT 7

#define START 0
#define RESET 1
#define FIRE  3

Joystick_ Joystick;
short pinIn[4] = {UP, DOWN, LEFT, RIGHT};

void setup() {
  // Joystick initialization
  Joystick.begin();
  delay(50);

  // Controller configuration
  for (short i = 0; i < 4; ++i) {
    short pin = pinIn[i];
    pinMode(pin, INPUT);
    Joystick.setButton(pin, 0);
  }

  // Interrupt definition
  attachInterrupt(digitalPinToInterrupt(FIRE), fireIsr, CHANGE);
  Joystick.setButton(FIRE, 0);
  attachInterrupt(digitalPinToInterrupt(START), startIsr, CHANGE);  
  Joystick.setButton(START, 0);
  attachInterrupt(digitalPinToInterrupt(RESET), resetIsr, CHANGE);
  Joystick.setButton(RESET, 0);

  delay(500);
}

void loop() {
  for (short i = 0; i < 4; ++i) {
    setState(pinIn[i]);
  }
  delay(50);
}

// --- ISRs ---

void fireIsr()  {
  setState(FIRE); 
}

void startIsr() {
  setState(START);
}

void resetIsr() {
  setState(RESET);
}

// --- Utility functions ---

void setState(short pin) {
  short state = !digitalRead(pin);
  Joystick.setButton(pin, state);
}
