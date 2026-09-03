// Motorcycle Slot Machine
// WeAct 4.2" e-paper (400x300, SSD1683) + ESP32 e-Paper Driver Board.
// On power-up the reels spin and land on today's bike (or a jackpot).

#include <Arduino.h>
#include <SPI.h>
#include <esp_sleep.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

#define ENABLE_GxEPD2_GFX 0
#include <GxEPD2_BW.h>
#include "config.h"
#include "bitmaps.h"

// Driver board FPC: CS 15, DC 27, RST 26, BUSY 25, SCK 13, MOSI 14 (HSPI, SCK/MOSI swapped vs default).
static const int PIN_CS = 15;
static const int PIN_DC = 27;
static const int PIN_RST = 26;
static const int PIN_BUSY = 25;
static const int PIN_SCK = 13;
static const int PIN_MISO = 12;
static const int PIN_MOSI = 14;

#if WEACT_PANEL_ALT
using Panel = GxEPD2_420_GYE042A87;
#else
using Panel = GxEPD2_420_GDEY042T81;
#endif

static GxEPD2_BW<Panel, Panel::HEIGHT> epd(Panel(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY));
static SPIClass epdBus(HSPI);

enum Badge : uint8_t { R7 = 0, R9, TT, HS, JP, BADGE_N };

static const char* kCode[BADGE_N] = {"R7", "R9", "TT", "HS", "JP"};
static const char* kName[BADGE_N] = {
    "Yamaha R7", "BMW R9T", "Triumph Thruxton", "Honda Shadow", "JACKPOT"};
static const uint16_t kWeight[BADGE_N] = {W_R7, W_R9, W_TT, W_HS, W_JP};
static const unsigned char* kBmp[BADGE_N] = {bmp_r7, bmp_r9, bmp_tt, bmp_hs, bmp_jp};

struct Window {
  int16_t x, y, w, h;
};

static Window gReel[3];
static int16_t gW, gH;

static uint16_t weightSum() {
  uint16_t s = 0;
  for (uint8_t i = 0; i < BADGE_N; i++) s = (uint16_t)(s + kWeight[i]);
  return s;
}

static Badge pickBadge(uint32_t rng) {
  uint16_t n = (uint16_t)(rng % weightSum());
  for (uint8_t i = 0; i < BADGE_N; i++) {
    if (n < kWeight[i]) return (Badge)i;
    n = (uint16_t)(n - kWeight[i]);
  }
  return R7;
}

static Badge scramble() {
  return (Badge)(esp_random() % BADGE_N);
}

static void centerText(const char* s, int16_t cx, int16_t cy, const GFXfont* font, uint16_t color) {
  epd.setFont(font);
  epd.setTextColor(color);
  int16_t x, y;
  uint16_t w, h;
  epd.getTextBounds(s, 0, 0, &x, &y, &w, &h);
  epd.setCursor((int16_t)(cx - (int16_t)w / 2 - x), (int16_t)(cy - (int16_t)h / 2 - y));
  epd.print(s);
}

static void paintBadge(const Window& win, Badge b) {
  const bool jack = (b == JP);
  const uint16_t bg = jack ? GxEPD_BLACK : GxEPD_WHITE;
  const uint16_t fg = jack ? GxEPD_WHITE : GxEPD_BLACK;
  epd.fillRect(win.x, win.y, win.w, win.h, bg);
  const int16_t bx = (int16_t)(win.x + (win.w - (int16_t)bmp_r7_w) / 2);
  const int16_t by = (int16_t)(win.y + (win.h - (int16_t)bmp_r7_h) / 2 - 8);
  epd.drawBitmap(bx, by, kBmp[b], bmp_r7_w, bmp_r7_h, fg);
  centerText(kCode[b], (int16_t)(win.x + win.w / 2), (int16_t)(win.y + win.h - 14),
             &FreeSansBold9pt7b, fg);
}

static void paintChrome(const Badge shown[3], const char* line1, const char* line2) {
  epd.fillScreen(GxEPD_WHITE);

  epd.fillRect(0, 0, gW, 36, GxEPD_BLACK);
  centerText("WHICH BIKE?", (int16_t)(gW / 2), 18, &FreeSansBold12pt7b, GxEPD_WHITE);

  for (int i = 0; i < 3; i++) {
    const Window& f = gReel[i];
    epd.drawRoundRect(f.x - 6, f.y - 6, f.w + 12, f.h + 12, 8, GxEPD_BLACK);
    epd.drawRoundRect(f.x - 4, f.y - 4, f.w + 8, f.h + 8, 6, GxEPD_BLACK);
    paintBadge(f, shown[i]);
  }

  if (line1 && line1[0]) {
    centerText(line1, (int16_t)(gW / 2), (int16_t)(gH - 40), &FreeSansBold12pt7b, GxEPD_BLACK);
  }
  if (line2 && line2[0]) {
    centerText(line2, (int16_t)(gW / 2), (int16_t)(gH - 18), &FreeSansBold9pt7b, GxEPD_BLACK);
  }
}

static void fullFrame(const Badge shown[3], const char* line1, const char* line2) {
  epd.setFullWindow();
  epd.firstPage();
  do {
    paintChrome(shown, line1, line2);
  } while (epd.nextPage());
}

static void flipReels(const Badge shown[3]) {
  const int16_t x = gReel[0].x;
  const int16_t y = gReel[0].y;
  const int16_t w = (int16_t)(gReel[2].x + gReel[2].w - gReel[0].x);
  const int16_t h = gReel[0].h;
  epd.setPartialWindow(x, y, w, h);
  epd.firstPage();
  do {
    epd.fillScreen(GxEPD_WHITE);
    for (int i = 0; i < 3; i++) paintBadge(gReel[i], shown[i]);
  } while (epd.nextPage());
}

static void layOut() {
  gW = epd.width();
  gH = epd.height();

  const int16_t gap = 10;
  const int16_t side = 18;
  const int16_t top = 52;
  const int16_t bottom = 58;
  const int16_t rw = (int16_t)((gW - 2 * side - 2 * gap) / 3);
  const int16_t rh = (int16_t)(gH - top - bottom);

  for (int i = 0; i < 3; i++) {
    gReel[i].x = (int16_t)(side + i * (rw + gap));
    gReel[i].y = top;
    gReel[i].w = rw;
    gReel[i].h = rh;
  }
}

static void spinTo(Badge landOn) {
  Badge face[3] = {scramble(), scramble(), scramble()};

  Serial.printf("land -> %s  %s\n", kCode[landOn], kName[landOn]);
  fullFrame(face, "SPINNING", "");

  int whirl = 3;
  int settle = 1;
  if (epd.epd2.hasFastPartialUpdate) {
    whirl = 6;
    settle = 2;
  } else if (!epd.epd2.hasPartialUpdate) {
    whirl = 0;
    settle = 0;
  }

  for (int n = 0; n < whirl; n++) {
    face[0] = scramble();
    face[1] = scramble();
    face[2] = scramble();
    flipReels(face);
  }
  face[0] = landOn;
  for (int n = 0; n < settle; n++) {
    face[1] = scramble();
    face[2] = scramble();
    flipReels(face);
  }
  face[1] = landOn;
  for (int n = 0; n < settle; n++) {
    face[2] = scramble();
    flipReels(face);
  }
  face[0] = face[1] = face[2] = landOn;

  if (landOn == JP) {
    fullFrame(face, "JACKPOT", "Buy another motorcycle.");
  } else {
    fullFrame(face, "TODAY'S RIDE", kName[landOn]);
  }
}

static void spinRandom() { spinTo(pickBadge(esp_random())); }

static bool bootHeld() {
#if PIN_SPIN_BUTTON < 0
  return false;
#else
  return digitalRead(PIN_SPIN_BUTTON) == LOW;
#endif
}

static void eatSerial() {
#if SERIAL_COMMANDS
  if (Serial.available() <= 0) return;
  const int c = Serial.read();
  if (c == 's' || c == 'S') spinRandom();
  else if (c == '1') spinTo(R7);
  else if (c == '2') spinTo(R9);
  else if (c == '3') spinTo(TT);
  else if (c == '4') spinTo(HS);
  else if (c == 'j' || c == 'J') spinTo(JP);
#endif
}

static void nap() {
  Serial.println("sleep — tap BOOT to spin again");
  epd.hibernate();
#if PIN_SPIN_BUTTON >= 0
  pinMode(PIN_SPIN_BUTTON, INPUT_PULLUP);
  esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_SPIN_BUTTON, 0);
#endif
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  delay(80);
  Serial.println("Motorcycle Slot Machine");
  Serial.printf("JP chance %u/%u\n", W_JP, weightSum());
#if TEST_MODE
  Serial.println("s=spin  1=R7 2=R9 3=TT 4=HS  j=jackpot");
#endif

#if PIN_SPIN_BUTTON >= 0
  pinMode(PIN_SPIN_BUTTON, INPUT_PULLUP);
#endif

  epdBus.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_CS);
  epd.epd2.selectSPI(epdBus, SPISettings(4000000, MSBFIRST, SPI_MODE0));
  epd.init(115200, true, 50, false);
  epd.setRotation(DISPLAY_ROTATION);
  epd.setTextWrap(false);
  layOut();
  Serial.printf("panel %dx%d  partial=%d fast=%d\n", gW, gH, epd.epd2.hasPartialUpdate,
                epd.epd2.hasFastPartialUpdate);

  spinRandom();
#if !TEST_MODE
  nap();
#endif
}

void loop() {
#if TEST_MODE
  eatSerial();

  if (bootHeld()) {
    delay(40);
    if (bootHeld()) {
      uint32_t t = millis();
      while (bootHeld() && millis() - t < 1500) delay(10);
      spinRandom();
    }
  }
  delay(20);
#else
  delay(1000);
#endif
}
