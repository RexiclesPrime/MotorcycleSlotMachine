#pragma once

// WeAct 4.2" (400x300, SSD1683) on an ESP32 e-Paper Driver Board.
// 0 = GDEY042T81 (WeAct's usual driver). 1 = GYE042A87 if the picture is blank or inverted.
#define WEACT_PANEL_ALT 0

// Native 4.2" is already landscape. Change only if the slot is sideways (1 or 3).
#define DISPLAY_ROTATION 0

// Stay awake so you can respin from the BOOT button or serial.
// Set 0 later: one spin, then deep sleep (the image stays on the glass).
#define TEST_MODE 1

#define PIN_SPIN_BUTTON 0  // BOOT on the driver board. -1 disables.

// Serial 115200: s = spin, 1-4 = R7/R9/TT/HS, j = jackpot
#define SERIAL_COMMANDS 1

// Four bikes equal. Jackpot is rare. 2/198 ≈ 1.01%.
#define W_R7 49
#define W_R9 49
#define W_TT 49
#define W_HS 49
#define W_JP 2
