//app_logic.cpp

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "app_logic.h"
#include "motor_driver.h"
#include "OLED_driver.h"

static unsigned long currentTime = 0;
static unsigned long lastTime = 0;
static int systemMode = 0;
float vBattery = 0;
float vOut = 0;
float raw = 0;
long sum = 0;
float percent = 0;
SoftwareSerial espSerial(12, 11);

void App_Init(){
  OLED_Init();
  Motor_Init();
  Serial.begin(9600);
  espSerial.begin(9600);
}

static void App_ReadBatteryV(){
  sum = 0;
  for(int i = 0; i < 20; i++){
  sum += analogRead(A0);
  }
  raw = sum / 20.0;
  vOut = raw * (5.0 / 1023.0);
  vBattery = vOut * 2.0;
}

static void App_ReadBatteryPercent(){
  float lowV = 4.8;
  float highV = 6.85;
  percent = ((vBattery - lowV) / (highV - lowV)) * 100.0;
  if (percent > 100) {percent = 100;}
  if (percent < 0) {percent = 0;}
}

void App_Logic(){
  currentTime = millis();
  OLED_Clear();
  OLED_SetText(1);
  OLED_SetPosition(0, 0);
  OLED_PrintText("GEAR: ");
  if (espSerial.available()){
    char cmd = espSerial.read();
    Serial.print(cmd);
    if (cmd == 'F'){
      systemMode = 1;
    }
    else if (cmd == 'R'){
      systemMode = 2;
    }
    else if (cmd == 'S'){
      systemMode = 0;
    }
  }
  Serial.println(systemMode);

  switch (systemMode) {
    case 0: {
      OLED_PrintText("P");
      Motor_SetGear(STOPPED);
      break;
    }
    case 1: {
      OLED_PrintText("D");
      Motor_SetGear(FORWARD);
      Motor_SetPWMA(150);
      Motor_SetPWMB(150);
      break;
    }
    case 2: {
      OLED_PrintText("R");
      Motor_SetGear(REVERSE);
      Motor_SetPWMA(150);
      Motor_SetPWMB(150);
      break;
    }
  }
  Motor_Update();
  if (currentTime - lastTime >= 200){
    App_ReadBatteryV();
    App_ReadBatteryPercent();
    App_SendTelemetry();
    lastTime = currentTime;
  }

  OLED_PrintText("\nBattery (V): ");
  OLED_PrintNum(vBattery);
  OLED_PrintText("V");
  OLED_PrintText("\nBattery (%): ");
  OLED_PrintNum(percent);
  OLED_PrintText("%");
  OLED_Update();
}

void App_SendTelemetry(){
  char gearChar = 'P';

  if (systemMode == 1){gearChar = 'D';}
  else if (systemMode == 2){gearChar = 'R';}

  espSerial.print("T,");
  espSerial.print(vBattery, 2);
  espSerial.print(",");
  espSerial.print(percent, 1);
  espSerial.print(",");
  espSerial.println(gearChar);
}

//
