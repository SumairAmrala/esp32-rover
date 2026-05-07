//OLED_driver.h

#ifndef OLED_DRIVER_H
#define OLED_DRIVER_H

void OLED_Init();
void OLED_Clear();
void OLED_SetText(int value);
void OLED_SetPosition(int x, int y);
void OLED_PrintText(const char* text);
void OLED_PrintNum(float value);
void OLED_Update();

#endif
//