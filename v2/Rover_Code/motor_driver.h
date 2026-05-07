//motor_driver.h

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

enum DriveMode {
  FORWARD,
  REVERSE,
  STOPPED
};

void Motor_Init();
void Motor_Set(int pwma, int pwmb);
void Motor_SetGear(DriveMode mode, int pin1, int pin2);
void Motor_Update();

#endif
//