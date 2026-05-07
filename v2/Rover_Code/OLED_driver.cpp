//OLED_driver.cpp
#include "OLED_driver.h"

#include <Wire.h>
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void OLED_Init(){
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println("OLED NOT FOUND");
    while(true);
  }
}

void OLED_Clear(){
  display.clearDisplay();
}

void OLED_SetText(int value){
  display.setTextSize(value);
  display.setTextColor(SSD1306_WHITE);
}

void OLED_SetPosition(int x, int y){
  display.setCursor(x, y);
}

void OLED_PrintText(const char* text){
  display.print(text);
}

void OLED_PrintNum(float value){
  display.print(value, 2);
}

void OLED_Update(){
  display.display();
}
//