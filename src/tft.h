#ifndef TFT_H
#define TFT_H

#include <Arduino.h>

// Inicializa SPI + ILI9341
void tftBegin();

// Telas de abertura (imagens/boot)
void tftShowBootScreen1();
void tftShowBootScreen2();
void tftShowReadyScreen();

// Tela principal de status (atualização 1 Hz)
void tftUpdateStatus(int temperatura,
                     int setpoint,
                     bool fornoLigado,
                     bool resistenciaAtiva,
                     bool wifiOn);

#endif