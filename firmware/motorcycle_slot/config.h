#pragma once

// WeAct 4.2" (400x300, SSD1683) on an ESP32 e-Paper Driver Board.
// 1 = black/white/RED glass (GDEY042Z98). Needed to actually see red paint/bow.
// 0 = black/white only (GDEY042T81). Use this if the screen goes blank or crazy.
#define PANEL_3COLOR 1
// Only used when PANEL_3COLOR is 0. 1 = GYE042A87 if the BW picture is blank or inverted.
#define WEACT_PANEL_ALT 0

// Native 4.2" is already landscape. Change only if the slot is sideways (1 or 3).
#define DISPLAY_ROTATION 0

// Stay awake so you can respin from the BOOT button or serial.
// Set 0 later: one spin, then deep sleep (the image stays on the glass).
#define TEST_MODE 1

#define PIN_SPIN_BUTTON 0  // BOOT on the driver board. -1 disables.

// E-ink frames. Each partial refresh is ~0.5–2s on the 4.2". Keep this tiny.
#define SPIN_WHIRL 1
#define SPIN_SETTLE 1

// Serial 115200: s = spin, 1-4 = R6/R12/TT/HS, j = jackpot
#define SERIAL_COMMANDS 1

// 1 = draw the 112x80 side-view sprites in bitmaps.h. 0 = letter codes only.
#define USE_SPRITES 1
// 1 = swap black/white in the sprite (try this if the bikes look like empty outlines or inverted blobs).
#define SPRITE_INVERT 0

// Four bikes equal. Jackpot is rare. 2/198 ≈ 1.01%.
#define W_R7 49
#define W_R9 49
#define W_TT 49
#define W_HS 49
#define W_JP 2
