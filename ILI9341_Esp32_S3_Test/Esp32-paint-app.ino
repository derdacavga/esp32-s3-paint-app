#include <TFT_eSPI.h>
#include <SPI.h>
#include <EEPROM.h>

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

#define EEPROM_SIZE 64
#define EEPROM_CALDATA_START 0 

uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA, TFT_BLACK};
String colorNames[] = {"RED", "GREEN", "BLUE", "YELLOW", "CYAN", "MAGENTA", "BLACK"};

uint16_t currentColor = TFT_BLACK;
uint8_t penSize = 3;

#define CLEAR_BTN_W 60
#define CLEAR_BTN_H 30
#define PEN_BTN_W 40
#define PEN_BTN_H 30

void setup() {

  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_WHITE);

  touch_calibrate();

  drawColorPalette();
  drawClearButton();
  drawPenSizeButtons();
}

void loop() {
  uint16_t x, y;
  if (tft.getTouch(&x, &y)) {
    if (x < 40) {
      selectColor(y);
    }
    else if (x > (tft.width() - CLEAR_BTN_W - 10) && y < (CLEAR_BTN_H + 10)) {
      clearDrawingArea();
    }
    else if (x > (tft.width() - PEN_BTN_W - 10) && y > (CLEAR_BTN_H + 20) && y < (CLEAR_BTN_H + PEN_BTN_H + 30)) {
      penSize++;
      if (penSize > 20) penSize = 20;
      drawPenSizeButtons();
    }
    else if (x > (tft.width() - PEN_BTN_W*2 - 20) && x < (tft.width() - PEN_BTN_W - 10) && y > (CLEAR_BTN_H + 20) && y < (CLEAR_BTN_H + PEN_BTN_H + 30)) {
      if (penSize > 1) penSize--;
      drawPenSizeButtons();
    }
    else {
      tft.fillCircle(x, y, penSize, currentColor);
    }
  }
}

void drawColorPalette() {
  int boxHeight = tft.height() / (sizeof(colors) / sizeof(colors[0]));

  for (int i = 0; i < sizeof(colors) / sizeof(colors[0]); i++) {
    tft.fillRect(0, i * boxHeight, 40, boxHeight, colors[i]);
  }
}

void selectColor(int y) {
  int boxHeight = tft.height() / (sizeof(colors) / sizeof(colors[0]));
  int index = y / boxHeight;

  if (index >= 0 && index < sizeof(colors) / sizeof(colors[0])) {
    currentColor = colors[index];
  }
}

void drawClearButton() {
  tft.fillRect(tft.width() - CLEAR_BTN_W - 10, 10, CLEAR_BTN_W, CLEAR_BTN_H, TFT_DARKGREY);
  tft.drawRect(tft.width() - CLEAR_BTN_W - 10, 10, CLEAR_BTN_W, CLEAR_BTN_H, TFT_BLACK);
  tft.setTextColor(TFT_BLACK, TFT_DARKGREY);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("CLEAR", tft.width() - CLEAR_BTN_W/2 - 10, 10 + CLEAR_BTN_H/2, 2);
}

void drawPenSizeButtons() {
  tft.fillRect(tft.width() - PEN_BTN_W - 10, CLEAR_BTN_H + 20, PEN_BTN_W, PEN_BTN_H, TFT_DARKCYAN);
  tft.drawRect(tft.width() - PEN_BTN_W - 10, CLEAR_BTN_H + 20, PEN_BTN_W, PEN_BTN_H, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_DARKCYAN);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("+", tft.width() - PEN_BTN_W/2 - 10, CLEAR_BTN_H + 20 + PEN_BTN_H/2, 2);

  tft.fillRect(tft.width() - PEN_BTN_W*2 - 20, CLEAR_BTN_H + 20, PEN_BTN_W, PEN_BTN_H, TFT_DARKCYAN);
  tft.drawRect(tft.width() - PEN_BTN_W*2 - 20, CLEAR_BTN_H + 20, PEN_BTN_W, PEN_BTN_H, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_DARKCYAN);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("-", tft.width() - PEN_BTN_W*3/2 - 20, CLEAR_BTN_H + 20 + PEN_BTN_H/2, 2);

  tft.fillRect(tft.width() - PEN_BTN_W*2 - 20, CLEAR_BTN_H + PEN_BTN_H + 40, PEN_BTN_W*2 + 10, 20, TFT_WHITE);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Size: " + String(penSize), tft.width() - PEN_BTN_W - 15, CLEAR_BTN_H + PEN_BTN_H + 50, 2);
}

void clearDrawingArea() {
  tft.fillRect(40, 0, tft.width() - 40, tft.height(), TFT_WHITE);
  drawClearButton();
  drawPenSizeButtons();
}

void touch_calibrate() {
  uint16_t calData[5];

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch Calibration");
  tft.println();
  tft.println("Follow the instructions...");
  tft.println();

  delay(1000);

  tft.calibrateTouch(calData, TFT_WHITE, TFT_RED, 15);
}


bool loadCalibration() {
  return false;
}
