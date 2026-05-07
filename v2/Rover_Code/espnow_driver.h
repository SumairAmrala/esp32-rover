#ifndef ESPNOW_DRIVER_H
#define ESPNOW_DRIVER_H

#include <Arduino.h>

void ESPNOW_Init();

bool ESPNOW_HasNewPacket();
void ESPNOW_ClearNewPacket();

int ESPNOW_GetSpeedL();
int ESPNOW_GetSpeedR();

unsigned long ESPNOW_LastReceiveTime();

#endif