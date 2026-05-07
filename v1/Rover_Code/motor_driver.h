//motor_driver.h

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

enum DriveMode {
  FORWARD,
  REVERSE,
  STOPPED
};

void Motor_Init();
void Motor_SetPWMA(int value);
void Motor_SetPWMB(int value);
void Motor_SetGear(DriveMode mode);
void Motor_Update();

#endif
//