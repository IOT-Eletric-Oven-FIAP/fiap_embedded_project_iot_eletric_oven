#include "mqtt.h"
#include <WiFi.h>
#include <PubSubClient.h>

static const char* MQTT_BROKER = "broker.hivemq.com";
static const int   MQTT_PORT   = 1883;
static const char* MQTT_TOPIC  = "equipeX/forno/status";

WiFiClient espClient;
PubSubClient client(espClient);

void mqttReconnect() {
  if (client.connected()) return;
  Serial.print("Conectando MQTT... ");
  if (client.connect("ESP32_Forno_EquipeX")) {
    Serial.println("MQTT conectado!");
  } else {
    Serial.print("Falhou, rc=");
    Serial.println(client.state());
  }
}

void mqttInit() {
  client.setServer(MQTT_BROKER, MQTT_PORT);
}

void mqttLoop() {
  if (!client.connected()) {
    mqttReconnect();
  }
  client.loop();
}

void mqttPublishStatus(float temp, float sp, bool forno, bool res) {
  if (!client.connected()) {
    Serial.println("MQTT nao conectado, nao publicou.");
    return;
  }

  char payload[200];
  snprintf(payload, sizeof(payload),
    "{"
    "\"temperatura\": %.2f,"
    "\"setpoint\": %.2f,"
    "\"forno\": %s,"
    "\"resistencia\": %s"
    "}",
    temp, sp, forno ? "true" : "false", res ? "true" : "false"
  );

  client.publish(MQTT_TOPIC, payload);
  Serial.print("MQTT Publicado: ");
  Serial.println(payload);
}