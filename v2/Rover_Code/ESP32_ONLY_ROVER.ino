#include "app_logic.h"
#include "motor_driver.h"
#include <Arduino.h>

const int AIN1 = 6;
const int AIN2 = 5;
const int PWMA = 4;
const int STBY = 7;

void setup() {
  App_Init();
}




void loop() {
  App_Logic();
}
//
