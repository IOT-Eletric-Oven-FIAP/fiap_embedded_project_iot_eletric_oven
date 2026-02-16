#ifndef HTTP_LIB_H
#define HTTP_LIB_H

#include <Arduino.h>

// Inicializa/configura a camada HTTP (credenciais ficam dentro do http.cpp)
void httpInit();

// Envia a telemetria para os serviços configurados (Ubidots + ThingSpeak)
void httpEnviarTelemetria(int temp, int sp, bool forno, bool res);

#endif