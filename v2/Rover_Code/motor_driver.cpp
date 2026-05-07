//motor_driver.cpp
#include "motor_driver.h"
#include <Arduino.h>

static int motorSpeedA = 0;
static int motorSpeedB = 0;

static const int Pwma = 4;
static const int ain1 = 6;
static const int ain2 = 5;
static const int bin1 = 15;
static const int bin2 = 17;
static const int Pwmb = 16;
static const int stby = 7;

void Motor_Init(){
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(Pwma, OUTPUT);
  pinMode(Pwmb, OUTPUT);
  pinMode(stby, OUTPUT);

  digitalWrite(stby, HIGH);
}

void Motor_Set(int pwma, int pwmb){
  if (pwma > 0){
    Motor_SetGear(FORWARD, ain1, ain2);
    motorSpeedA = pwma;
  }
  else if (pwma < 0){
    Motor_SetGear(REVERSE, ain1, ain2);
    motorSpeedA = abs(pwma);
  }
  else {motorSpeedA = 0; Motor_SetGear(STOPPED, ain1, ain2);}

  if (pwmb > 0){
    Motor_SetGear(FORWARD, bin1, bin2);
    motorSpeedB = pwmb;
  }
  else if (pwmb < 0){
    Motor_SetGear(REVERSE, bin1, bin2);
    motorSpeedB = abs(pwmb);
  }
  else {motorSpeedB = 0; Motor_SetGear(STOPPED, bin1, bin2);}
}

void Motor_SetGear(DriveMode mode, int pin1, int pin2){
  switch(mode){
    case FORWARD: {
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      break;
    }
    case REVERSE: {
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      break;
    }
    case STOPPED: {
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      break;
    }
  }
}

void Motor_Update(){
  analogWrite(Pwma, motorSpeedA);
  analogWrite(Pwmb, motorSpeedB);
}
//