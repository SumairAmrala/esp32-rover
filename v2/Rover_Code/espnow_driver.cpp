#include "espnow_driver.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

typedef struct {
  int SpeedL;
  int SpeedR;
} DrivePacket;

static bool newPacketReceived = false;
static unsigned long lastReceivedTime = 0;

static int latestSpeedL = 0;
static int latestSpeedR = 0;

static void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len != sizeof(DrivePacket)) {
    return;
  }

  DrivePacket packet;
  memcpy(&packet, incomingData, sizeof(packet));

  latestSpeedL = packet.SpeedL;
  latestSpeedR = packet.SpeedR;

  newPacketReceived = true;
  lastReceivedTime = millis();
}

void ESPNOW_Init() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  delay(100);

  Serial.print("Rover MAC Address: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("ESP-NOW receiver ready");
}

bool ESPNOW_HasNewPacket() {
  return newPacketReceived;
}

void ESPNOW_ClearNewPacket() {
  newPacketReceived = false;
}

int ESPNOW_GetSpeedL() {
  return latestSpeedL;
}

int ESPNOW_GetSpeedR() {
  return latestSpeedR;
}

unsigned long ESPNOW_LastReceiveTime() {
  return lastReceivedTime;
}