#ifndef MQTT_LIB_H
#define MQTT_LIB_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

extern WiFiClient espClient;
extern PubSubClient mqttClient;

void mqttInit();
void mqttLoop();
void mqttPublishStatus(float temp, float sp, bool forno, bool res);

#endif