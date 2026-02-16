#include "tft.h"

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <pgmspace.h>

// ----------------------------------------------------
// Boot images (320x180) - arrays from img1.cpp / img2.cpp
// ----------------------------------------------------
extern const uint16_t img1[320 * 180];
extern const uint16_t img2[320 * 180];
static const int16_t BOOT_IMG_W = 320;
static const int16_t BOOT_IMG_H = 180;

// ------------------------------
// Pinos escolhidos
// ------------------------------
#define TFT_CS   42
#define TFT_DC   40
#define TFT_RST  41

#define TFT_SCK  38
#define TFT_MOSI 39
#define TFT_MISO 36

static Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);
static bool layoutDrawn = false;

// Helpers de dimensões
static inline int16_t W() { return (int16_t)tft.width(); }
static inline int16_t H() { return (int16_t)tft.height(); }

// Ajuste visual
static const int16_t UI_MARGIN = 8;
static const int16_t UI_INNER  = 3;

// ------------------------------
// Texto centralizado
// ------------------------------
static int16_t centerXForText(const char* text, uint8_t textSize) {
  int16_t x1, y1;
  uint16_t w, h;

  tft.setTextSize(textSize);
  tft.getTextBounds((char*)text, 0, 0, &x1, &y1, &w, &h);

  int16_t x = (W() - (int16_t)w) / 2;
  if (x < 0) x = 0;
  return x;
}

static void printCentered(int y, const char* text, uint8_t textSize, uint16_t color) {
  tft.setTextSize(textSize);
  tft.setTextColor(color);
  tft.setCursor(centerXForText(text, textSize), y);
  tft.print(text);
}

// ------------------------------
// Moldura/Header/Footer
// ------------------------------
static void drawFrame() {
  const int16_t x = UI_MARGIN;
  const int16_t y = UI_MARGIN;
  const int16_t w = W() - (2 * UI_MARGIN);
  const int16_t h = H() - (2 * UI_MARGIN);

  tft.drawRect(x, y, w, h, ILI9341_DARKGREY);
  tft.drawRect(x + UI_INNER, y + UI_INNER, w - 2 * UI_INNER, h - 2 * UI_INNER, ILI9341_DARKGREY);
}

static void drawHeader(const char* title) {
  drawFrame();

  const int16_t x1 = UI_MARGIN + 6;
  const int16_t x2 = W() - UI_MARGIN - 7;
  const int16_t y  = UI_MARGIN + 34;

  tft.drawLine(x1, y, x2, y, ILI9341_DARKGREY);

  if (title && title[0] != '\0') {
    printCentered(UI_MARGIN + 12, title, 2, ILI9341_WHITE);
  }
}

static void drawFooter(const char* footerText) {
  const int16_t x1 = UI_MARGIN + 6;
  const int16_t x2 = W() - UI_MARGIN - 7;
  const int16_t y  = H() - UI_MARGIN - 35;

  tft.drawLine(x1, y, x2, y, ILI9341_DARKGREY);

  if (footerText && footerText[0] != '\0') {
    printCentered(H() - UI_MARGIN - 26, footerText, 2, ILI9341_WHITE);
  }
}

// ------------------------------
// Boot image 320x180 (PROGMEM) por linhas
// ------------------------------
static void drawBootImage320x180(const unsigned short* img) {
  const int16_t x0 = 0;
  const int16_t y0 = (tft.height() - BOOT_IMG_H) / 2; // 240->30

  static uint16_t lineBuf[BOOT_IMG_W];

  tft.startWrite();
  tft.setAddrWindow(x0, y0, BOOT_IMG_W, BOOT_IMG_H);

  const bool SWAP_BYTES = false;

  for (int16_t y = 0; y < BOOT_IMG_H; y++) {
    for (int16_t x = 0; x < BOOT_IMG_W; x++) {
      uint32_t idx = (uint32_t)y * (uint32_t)BOOT_IMG_W + (uint32_t)x;
      uint16_t c = pgm_read_word(&img[idx]);
      if (SWAP_BYTES) c = (uint16_t)((c << 8) | (c >> 8));
      lineBuf[x] = c;
    }
    tft.writePixels(lineBuf, BOOT_IMG_W, true);
  }

  tft.endWrite();
}

// ------------------------------
// Layout fixo (Forno)
// ------------------------------
static void drawStaticLayout() {
  tft.fillScreen(ILI9341_BLACK);

  drawHeader("Status do Forno");
  drawFooter("FIAP + CPQD + SOFTEX");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);

  // Labels fixos
  const int16_t xL = UI_MARGIN + 12;
  tft.setCursor(xL, 55);  tft.print("Temp atual:");
  tft.setCursor(xL, 85);  tft.print("Setpoint:");
  tft.setCursor(xL, 115); tft.print("Forno:");
  tft.setCursor(xL, 145); tft.print("Resist.:");
  tft.setCursor(xL, 175); tft.print("WiFi:");

  layoutDrawn = true;
}

// ------------------------------
// API pública
// ------------------------------
void tftBegin() {
  SPI.begin(TFT_SCK, TFT_MISO, TFT_MOSI, TFT_CS);
  SPI.setFrequency(20000000); // 20 MHz

  tft.begin();
  tft.setRotation(1); // 320x240 landscape

  tft.fillScreen(ILI9341_BLACK);
  drawHeader("");
  drawFooter("");
  printCentered(120, "Display inicializado", 2, ILI9341_WHITE);
  delay(400);
}

void tftShowBootScreen1() {
  layoutDrawn = false;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("");
  drawFooter("");
  drawBootImage320x180(img1);
}

void tftShowBootScreen2() {
  layoutDrawn = false;
  tft.fillScreen(ILI9341_BLACK);
  drawBootImage320x180(img2);
}

void tftShowReadyScreen() {
  layoutDrawn = false;
  tft.fillScreen(ILI9341_BLACK);
  drawHeader("Status do Forno");
  drawFooter("FIAP + CPQD + SOFTEX");
  printCentered(110, "Inicializando...", 2, ILI9341_WHITE);
  delay(200);
}

void tftUpdateStatus(int temperatura,
                     int setpoint,
                     bool fornoLigado,
                     bool resistenciaAtiva,
                     bool wifiOn) {
  if (!layoutDrawn) {
    drawStaticLayout();
  }

  // Área de valores (coluna direita)
  const int16_t xV = UI_MARGIN + 170;
  const int16_t wV = (W() - UI_MARGIN) - xV - 12; // sobra até a borda interna
  const int16_t hLine = 22;

  auto clearValueLine = [&](int16_t y) {
    tft.fillRect(xV, y, wV, hLine, ILI9341_BLACK);
  };

  tft.setTextSize(2);

  // Temp
  clearValueLine(55);
  tft.setCursor(xV, 55);
  tft.setTextColor(ILI9341_WHITE);
  tft.print(temperatura);
  tft.print(" C");

  // Setpoint
  clearValueLine(85);
  tft.setCursor(xV, 85);
  tft.setTextColor(ILI9341_WHITE);
  tft.print(setpoint);
  tft.print(" C");

  // Forno
  clearValueLine(115);
  tft.setCursor(xV, 115);
  tft.setTextColor(fornoLigado ? ILI9341_GREEN : ILI9341_RED);
  tft.print(fornoLigado ? "LIGADO" : "DESLIG.");

  // Resistência
  clearValueLine(145);
  tft.setCursor(xV, 145);
  tft.setTextColor(resistenciaAtiva ? ILI9341_GREEN : ILI9341_RED);
  tft.print(resistenciaAtiva ? "ON" : "OFF");

  // WiFi
  clearValueLine(175);
  tft.setCursor(xV, 175);
  tft.setTextColor(wifiOn ? ILI9341_GREEN : ILI9341_RED);
  tft.print(wifiOn ? "ON" : "OFF");
}