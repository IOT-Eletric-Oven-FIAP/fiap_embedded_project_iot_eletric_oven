#ifndef MQTT_LIB_H
#define MQTT_LIB_H

#include <Arduino.h>

void mqttInit();
void mqttLoop();
void mqttPublishStatus(float temp, float sp, bool forno, bool res);

#endif
