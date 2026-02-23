#include "wifi_con.h"
#include "mqtt.h"
#include "tft.h"

// ========= CONFIGURAÇÕES DE REDE =========
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ========= DEFINIÇÃO DOS PINOS (HARDWARE) =========
#define NTC_PIN 12
#define POT_PIN 1
#define LED_PIN 14
#define BTN_PIN 4

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
  
  static float tempSimulada = 25.0;

  if (resistencia) {
    tempSimulada += 0.5;
  } else {
    tempSimulada -= 0.2;
  }

  if (tempSimulada < 20) tempSimulada = 20;
  if (tempSimulada > 300) tempSimulada = 300;

  return tempSimulada;
}

// ========= FUNÇÃO: LÓGICA DO TERMOSTATO =========
void controlarForno() {
  if (!fornoLigado || setpoint == 0) {
    resistencia = false;
  } 
  else {

    if (setpoint > temperatura + histerese) {
      resistencia = true;
    }

    if (setpoint <= temperatura) {
      resistencia = false;
    }
  }

  digitalWrite(LED_PIN, resistencia);
}

// ========= SETUP =========
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);

  tftBegin();
  tftShowBootScreen1();
  delay(2000);
  tftShowBootScreen2();
  delay(2000);
  tftShowReadyScreen();

  wifiInit(ssid, password);
  mqttInit();
}

// ========= LOOP =========
void loop() {

  mqttLoop();

  unsigned long tempoAtual = millis();

  bool estadoAtualBotao = digitalRead(BTN_PIN);

  if (estadoAtualBotao == LOW && estadoAnteriorBotao == HIGH) {
    fornoLigado = !fornoLigado;

    mqttPublishStatus(temperatura, setpoint, fornoLigado, resistencia);

    if (fornoLigado) {
      Serial.println("SISTEMA LIGADO");
    } else {
      Serial.println("SISTEMA DESLIGADO");
    }

    delay(50);
  }

  estadoAnteriorBotao = estadoAtualBotao;

  temperatura = lerTemperatura();
  int leituraPot = analogRead(POT_PIN);

  if (leituraPot < 100) { 
    setpoint = 0; 
  } else {
    setpoint = map(leituraPot, 0, 4095, 0, 250);
  }

  controlarForno();

  if (tempoAtual - lastTft >= 1000) {
    lastTft = tempoAtual;
    tftUpdateStatus((int)temperatura, (int)setpoint, fornoLigado, resistencia, wifiConnected());
  }

  if (tempoAtual - ultimoEnvio >= intervalo) {
    ultimoEnvio = tempoAtual;
    mqttPublishStatus(temperatura, setpoint, fornoLigado, resistencia);
  }
}
