#include <Arduino.h>
#include <math.h> // Essencial para a função log() do NTC
#include "wifi_con.h"
#include "mqtt.h"
#include "tft.h"

// Garante que o compilador encontre as funções de WiFi
extern void wifiInit(const char* ssid, const char* password);
extern bool wifiConnected();

const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ========= DEFINIÇÃO DOS PINOS =========
#define NTC_PIN 5   // NTC agora no pino 5
#define POT_PIN 6   // Potenciômetro no pino 6
#define LED_PIN 14
#define BTN_PIN 4

float temperatura = 0;
float setpoint = 180;
bool fornoLigado = false;
bool resistenciaAtiva = false;
float histerese = 2.0;

unsigned long ultimoEnvio = 0;
const long intervalo = 2000;
bool estadoAnteriorBotao = HIGH;
unsigned long lastTft = 0;

int lerSensorComFiltro(int pino) {
  long soma = 0;
  for (int i = 0; i < 10; i++) {
    soma += analogRead(pino);
    delay(5);
  }
  return soma / 10;
}

void controlarForno() {
  if (!fornoLigado) {
    resistenciaAtiva = false;
  } else {
    if (temperatura < (setpoint - histerese)) {
      resistenciaAtiva = true;
    } else if (temperatura >= setpoint) {
      resistenciaAtiva = false;
    }
  }
  digitalWrite(LED_PIN, resistenciaAtiva ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(POT_PIN, INPUT);
  pinMode(NTC_PIN, INPUT);

  // Animação de Boot
  tftBegin();               
  tftShowBootScreen1(); 
  delay(3000);          
  tftShowBootScreen2(); 
  delay(3000);          
  tftShowReadyScreen(); 
  delay(1000);

  wifiInit(ssid, password); 
  mqttInit();               
  
  Serial.println("Forno IoT Pronto!");
  lastTft = millis(); 
}

void loop() {
  bool estadoAtualBotao = digitalRead(BTN_PIN);
  if (estadoAtualBotao == LOW && estadoAnteriorBotao == HIGH) {
    fornoLigado = !fornoLigado;
    Serial.println(fornoLigado ? "SISTEMA LIGADO" : "SISTEMA DESLIGADO");
    delay(50);
  }
  estadoAnteriorBotao = estadoAtualBotao;

  // 1. LEITURA DO SETPOINT
  int leituraPot = lerSensorComFiltro(POT_PIN);
  setpoint = map(leituraPot, 0, 4095, 20, 300); 

  // 2. LEITURA DO NTC COM A ESCALA MAPEADA
  int leituraNTC = lerSensorComFiltro(NTC_PIN);
  if (leituraNTC > 0 && leituraNTC < 4095) {
    float rNTC = 10000.0 / (4095.0 / leituraNTC - 1.0);
    float temperaturaK = 1.0 / (1.0 / 298.15 + (1.0 / 3950.0) * log(rNTC / 10000.0));
    
    // Descobre qual é a temperatura no Wokwi (-24 a 80)
    float tempWokwi = temperaturaK - 273.15; 
    
    // A MÁGICA AQUI: Converte a escala do Wokwi para a escala do Forno (20 a 300)
    temperatura = map(tempWokwi, -24, 80, 20, 300);
    
    // Travas de segurança para o display não passar dos limites
    if (temperatura < 20) temperatura = 20;
    if (temperatura > 300) temperatura = 300;
  }

  controlarForno();

  if (millis() - lastTft > 1000) {
    tftUpdateStatus((int)temperatura, (int)setpoint, fornoLigado, resistenciaAtiva, wifiConnected());
    lastTft = millis();
  }

  if (millis() - ultimoEnvio > intervalo) {
    mqttPublishStatus(temperatura, setpoint, fornoLigado, resistenciaAtiva); 
    ultimoEnvio = millis();
  }

  mqttLoop();
}