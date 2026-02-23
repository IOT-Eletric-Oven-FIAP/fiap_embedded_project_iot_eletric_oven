#ifndef WIFI_LIB_H
#define WIFI_LIB_H

#include <WiFi.h>

void wifiInit(const char* ssid, const char* password);
bool wifiConnected();

#endif