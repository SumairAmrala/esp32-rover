//motor_driver.cpp
#include "motor_driver.h"
#include <Arduino.h>

static int motorSpeedA = 0;
static int motorSpeedB = 0;

static const int pwma = 9;
static const int ain1 = 7;
static const int ain2 = 6;
static const int bin1 = 4;
static const int bin2 = 3;
static const int pwmb = 10;
static const int stby = 8;

void Motor_Init(){
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);
  pinMode(stby, OUTPUT);

  digitalWrite(stby, HIGH);
}

void Motor_SetPWMA(int value){
  motorSpeedA = value;
}

void Motor_SetPWMB(int value){
  motorSpeedB = value;
}

void Motor_SetGear(DriveMode mode){
  switch(mode){
    case FORWARD: {
      digitalWrite(ain1, HIGH);
      digitalWrite(ain2, LOW);
      digitalWrite(bin1, HIGH);
      digitalWrite(bin2, LOW);
      break;
    }
    case REVERSE: {
      digitalWrite(ain1, LOW);
      digitalWrite(ain2, HIGH);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, HIGH);
      break;
    }
    case STOPPED: {
      digitalWrite(ain1, LOW);
      digitalWrite(ain2, LOW);
      digitalWrite(bin1, LOW);
      digitalWrite(bin2, LOW);
      Motor_SetPWMB(0);
      Motor_SetPWMA(0);
      break;
    }
  }
}

void Motor_Update(){
  analogWrite(pwma, motorSpeedA);
  analogWrite(pwmb, motorSpeedB);
}
//