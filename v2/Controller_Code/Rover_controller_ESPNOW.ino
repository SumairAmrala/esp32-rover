#include <WiFi.h>
#include <esp_now.h>
#include "joystick_driver.h"

unsigned long lastSendTime = 0;
const unsigned long sendDelay = 50;

static const int center = 2048;
static const int deadzone = 250;

typedef struct {
  int SpeedL;
  int SpeedR;
} DrivePacket;

// Replace with your rover ESP32 MAC address
uint8_t roverAddress[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

DrivePacket packet;

void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("Send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, roverAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add rover peer");
    return;
  }

  Serial.println("Remote ready");
  Joystick_Init(9, 6);
}

void loop() {
  int readingX = Joystick_GetXReading();
  int readingY = Joystick_GetYReading();

  int dx = readingX - center;
  int dy = readingY - center;

  if (abs(dx) < deadzone) dx = 0;
  if (abs(dy) < deadzone) dy = 0;

  int throttle = dy;
  int turn = dx;

  packet.SpeedL = throttle + turn;
  packet.SpeedR = throttle - turn;

  packet.SpeedL = constrain(packet.SpeedL, -2047, 2047);
  packet.SpeedR = constrain(packet.SpeedR, -2047, 2047);

  if (millis() - lastSendTime >= sendDelay) {
    esp_now_send(roverAddress, (uint8_t *)&packet, sizeof(packet));
    lastSendTime = millis();
  }
}
//