#include "http.h"
#include "wifi.h"

#include <HTTPClient.h>
#include <ArduinoJson.h>

// ===================== CONFIG HTTP (EDITAR AQUI) =====================
static const char* UBIDOTS_TOKEN  = "BBUS-X7z3rOgZQmfO7DQwb4mP7FHSzBNFR6";
static const char* UBIDOTS_DEVICE = "forno_talles";
static const char* THINGSPEAK_KEY = "8X2M83ELJL3JDA4E";
// ====================================================================

static void enviarUbidotsHTTP(int temp, int sp, bool forno, bool res) {
  if (!wifiConnected()) {
    Serial.println("WiFi OFF: nao enviou Ubidots.");
    return;
  }

  HTTPClient http;
  String url = "http://industrial.api.ubidots.com/api/v1.6/devices/";
  url += UBIDOTS_DEVICE;

  http.begin(url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Auth-Token", UBIDOTS_TOKEN);

  StaticJsonDocument<200> doc;
  doc["temperatura"] = temp;
  doc["setpoint"] = sp;
  doc["forno"] = forno;
  doc["resistencia"] = res;

  String json;
  serializeJson(doc, json);

  int httpCode = http.POST(json);
  Serial.print("HTTP Ubidots Code: ");
  Serial.println(httpCode);

  http.end();
}

static void enviarThingSpeakHTTP(int temp, int sp, bool forno, bool res) {
  if (!wifiConnected()) {
    Serial.println("WiFi OFF: nao enviou ThingSpeak.");
    return;
  }

  HTTPClient http;

  String url = "http://api.thingspeak.com/update?";
  url += "api_key=" + String(THINGSPEAK_KEY);
  url += "&field1=" + String(temp);
  url += "&field2=" + String(sp);
  url += "&field3=" + String(forno);
  url += "&field4=" + String(res);

  http.begin(url);

  int httpCode = http.GET();
  Serial.print("HTTP ThingSpeak Code: ");
  Serial.println(httpCode);

  http.end();
}

void httpInit() {
  // Nada obrigatório aqui hoje, mas deixei o hook caso você queira validar config etc.
  // Ex.: Serial.println("HTTP init OK");
}

void httpEnviarTelemetria(int temp, int sp, bool forno, bool res) {
  // Centraliza o fluxo de envio HTTP inteiro
  enviarUbidotsHTTP(temp, sp, forno, res);
  enviarThingSpeakHTTP(temp, sp, forno, res);
}