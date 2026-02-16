#include "wifi.h"
#include "http.h"
#include "tft.h"

// ========= CONFIGURAÇÕES DE REDE =========
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ========= DEFINIÇÃO DOS PINOS (HARDWARE) =========
#define NTC_PIN 12
#define POT_PIN 11
#define LED_PIN 14
#define BTN_PIN 27

// ========= VARIÁVEIS DE CONTROLE DO SISTEMA =========
float temperatura = 0;
float setpoint = 180;
bool fornoLigado = false;
bool resistencia = false;
float histerese = 2.0;

// ========= VARIÁVEIS DE TEMPO E ESTADO =========
unsigned long ultimoEnvio = 0;
const long intervalo = 2000;
bool estadoAnteriorBotao = HIGH;

// ====== TFT timing (1 Hz) ======
unsigned long lastTft = 0;

// ========= FUNÇÃO: LEITURA DOS SENSORES =========
float lerTemperatura() {
  return map(analogRead(NTC_PIN), 0, 4095, 20, 300);
}

// ========= FUNÇÃO: LÓGICA DO TERMOSTATO =========
void controlarForno() {
  if (!fornoLigado) {
    resistencia = false;
  } else {
    if (temperatura < setpoint - histerese) resistencia = true;
    if (temperatura > setpoint + histerese) resistencia = false;
  }
  digitalWrite(LED_PIN, resistencia);
}

// ========= FUNÇÃO: ENVIO DE DADOS =========
void enviarTelemetria() {
  Serial.println("------------ STATUS DO FORNO ------------");
  Serial.print("Temperatura Atual: "); Serial.print(temperatura); Serial.println(" °C");
  Serial.print("Setpoint: ");          Serial.print(setpoint);    Serial.println(" °C");
  Serial.print("Forno Ligado: ");      Serial.println(fornoLigado ? "SIM" : "NAO");
  Serial.print("Resistencia Ativa: "); Serial.println(resistencia ? "SIM" : "NAO");
  Serial.println("------------------------------------------\n");

  httpEnviarTelemetria((int)temperatura, (int)setpoint, fornoLigado, resistencia);
}

// ========= SETUP =========
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  // TFT
  tftBegin();
  tftShowBootScreen1();
  delay(2000);
  tftShowBootScreen2();
  delay(2000);
  tftShowReadyScreen();

  // WiFi + HTTP
  wifiInit(ssid, password);
  httpInit();
}

// ========= LOOP =========
void loop() {
  bool estadoAtualBotao = digitalRead(BTN_PIN);

  if (estadoAtualBotao == LOW && estadoAnteriorBotao == HIGH) {
    fornoLigado = !fornoLigado;

    if (fornoLigado) {
      Serial.println("SISTEMA LIGADO");
    } else {
      Serial.println("SISTEMA DESLIGADO");
      enviarTelemetria();
    }
    delay(150);
  }
  estadoAnteriorBotao = estadoAtualBotao;

  temperatura = lerTemperatura();
  delay(1500);

  setpoint = map(analogRead(POT_PIN), 0, 4095, 50, 250);
  controlarForno();

  unsigned long tempoAtual = millis();

  // TFT 1 Hz
  if (tempoAtual - lastTft >= 1000) {
    lastTft = tempoAtual;
    tftUpdateStatus((int)temperatura, (int)setpoint, fornoLigado, resistencia, wifiConnected());
  }

  // Envio periódico (quando ligado)
  if (fornoLigado && (tempoAtual - ultimoEnvio >= intervalo)) {
    ultimoEnvio = tempoAtual;
    enviarTelemetria();
  }
}