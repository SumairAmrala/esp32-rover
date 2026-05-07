#include "motor_driver.h"
#include "app_logic.h"
#include "OLED_Driver.h"
#include "espnow_driver.h"
#include <Arduino.h>

static unsigned long currentTime = 0;
static unsigned long lastBatteryUpdate = 0;
static unsigned long lastSpeedUpdate = 0;

static const unsigned long speedDelay = 20;
static const unsigned long connectionTimeout = 2000;

static float vBattery = 0;
static float vOut = 0;
static float raw = 0;
static long sum = 0;
static float percent = 0;

static int currentLeftSpeed = 0;
static int currentRightSpeed = 0;

static int targetLeftSpeed = 0;
static int targetRightSpeed = 0;

static int leftTrim = -6;
static int rightTrim = 0;

static const float BATTERY_LOW_WARNING = 6.2;
static const float BATTERY_HARD_CUTOFF = 6.0;

static bool batteryLow = false;
static bool batteryCritical = false;

void App_Init() {
  Motor_Init();
  OLED_Init();
  Serial.begin(115200);
  ESPNOW_Init();
}

static void App_CheckBatteryStatus() {
  batteryLow = (vBattery <= BATTERY_LOW_WARNING);
  batteryCritical = (vBattery <= BATTERY_HARD_CUTOFF);
}

static void App_UpdateSpeed() {
  int steps = 15;

  if (currentRightSpeed < targetRightSpeed) {
    currentRightSpeed += steps;
    if (currentRightSpeed > targetRightSpeed) {
      currentRightSpeed = targetRightSpeed;
    }
  }
  else if (currentRightSpeed > targetRightSpeed) {
    currentRightSpeed -= steps;
    if (currentRightSpeed < targetRightSpeed) {
      currentRightSpeed = targetRightSpeed;
    }
  }

  if (currentLeftSpeed < targetLeftSpeed) {
    currentLeftSpeed += steps;
    if (currentLeftSpeed > targetLeftSpeed) {
      currentLeftSpeed = targetLeftSpeed;
    }
  }
  else if (currentLeftSpeed > targetLeftSpeed) {
    currentLeftSpeed -= steps;
    if (currentLeftSpeed < targetLeftSpeed) {
      currentLeftSpeed = targetLeftSpeed;
    }
  }
}

static int App_ClampSpeed(int value) {
  if (value > 255) {
    return 255;
  }

  if (value < -255) {
    return -255;
  }

  return value;
}

static int App_MapRemoteSpeed(int value) {
  int mapped = map(value, -2047, 2047, -150, 150);
  return App_ClampSpeed(mapped);
}

static void App_ReadBatteryV() {
  sum = 0;

  for (int i = 0; i < 20; i++) {
    sum += analogRead(10);
  }

  raw = sum / 20.0;
  vOut = raw * (3.3 / 4095.0);
  vBattery = vOut * 3.2;
}

static void App_ReadBatteryPercent() {
  float lowV = 6.0;
  float highV = 8.4;

  percent = ((vBattery - lowV) / (highV - lowV)) * 100.0;

  if (percent > 100) {
    percent = 100;
  }

  if (percent < 0) {
    percent = 0;
  }
}

static void App_ProcessNewDrivePacket() {
  int mappedLeft = App_MapRemoteSpeed(ESPNOW_GetSpeedL());
  int mappedRight = App_MapRemoteSpeed(ESPNOW_GetSpeedR());

  if (mappedLeft != 0) {
    mappedLeft += leftTrim;
  }

  if (mappedRight != 0) {
    mappedRight += rightTrim;
  }

  targetLeftSpeed = App_ClampSpeed(mappedLeft);
  targetRightSpeed = App_ClampSpeed(mappedRight);

  ESPNOW_ClearNewPacket();
}

static void App_ApplySafetyOverrides() {
  if (currentTime - ESPNOW_LastReceiveTime() >= connectionTimeout) {
    targetLeftSpeed = 0;
    targetRightSpeed = 0;
  }

  if (batteryCritical) {
    targetLeftSpeed = 0;
    targetRightSpeed = 0;
  }
}

static void App_UpdateBattery() {
  if (currentTime - lastBatteryUpdate >= 200) {
    App_ReadBatteryV();
    App_ReadBatteryPercent();
    App_CheckBatteryStatus();

    lastBatteryUpdate = currentTime;
  }
}

static void App_UpdateMotionRamp() {
  if (currentTime - lastSpeedUpdate >= speedDelay) {
    App_UpdateSpeed();
    lastSpeedUpdate = currentTime;
  }
}

static void App_UpdateOLED() {
  OLED_Clear();
  OLED_SetText(1);
  OLED_SetPosition(0, 0);

  OLED_PrintText("L: ");
  OLED_PrintNum(currentLeftSpeed);

  OLED_PrintText("\nR: ");
  OLED_PrintNum(currentRightSpeed);

  OLED_PrintText("\nBattery(V): ");
  OLED_PrintNum(vBattery);

  OLED_PrintText("\nBattery(%): ");
  OLED_PrintNum(percent);

  if (batteryCritical) {
    OLED_PrintText("\nCRITICAL: STOP");
  }
  else if (batteryLow) {
    OLED_PrintText("\nLOW BATTERY");
  }

  OLED_Update();
}

void App_Logic() {
  currentTime = millis();

  if (ESPNOW_HasNewPacket()) {
    App_ProcessNewDrivePacket();
  }

  App_UpdateBattery();
  App_ApplySafetyOverrides();
  App_UpdateMotionRamp();

  Motor_Set(currentLeftSpeed, currentRightSpeed);
  Motor_Update();

  App_UpdateOLED();
}