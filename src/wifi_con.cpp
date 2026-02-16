#include "wifi_con.h"
#include <WiFi.h>
#include <Arduino.h>

void wifiInit(const char* ssid, const char* password) {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

bool wifiConnected() {
  return (WiFi.status() == WL_CONNECTED);
}