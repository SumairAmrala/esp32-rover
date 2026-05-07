//joystick_driver.cpp
#include <Arduino.h>
#include "joystick_driver.h"

static int pinX = 0;
static int pinY = 0;

void Joystick_Init(int xPin, int yPin){
  pinX = xPin;
  pinY = yPin;
}


int Joystick_GetXReading(){
  int xReading = analogRead(pinX);
  return xReading;
}
int Joystick_GetYReading(){
  int yReading = analogRead(pinY);
  return yReading;
}