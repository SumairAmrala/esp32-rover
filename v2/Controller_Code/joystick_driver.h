//joystick_driver.h

#ifndef JOYSTICK_DRIVER_H
#define JOYSTICK_DRIVER_H

void Joystick_Init(int xPin, int yPin);
int Joystick_GetXReading();
int Joystick_GetYReading();

#endif