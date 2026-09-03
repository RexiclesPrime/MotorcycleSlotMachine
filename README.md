# Motorcycle Slot Machine

On power-on, an ESP32 spins a three-reel e-ink slot and picks today's bike.

| Code | Bike |
|------|------|
| **R7** | Yamaha R7 |
| **R9** | BMW R9T |
| **TT** | Triumph Thruxton |
| **HS** | Honda Shadow |
| **JP** | Jackpot — *Buy another motorcycle.* |

Bikes are equally likely. Jackpot is about 1% (`2/198`). Reels show those codes for now; side-view pixel art comes later.

## Hardware

- WeAct 4.2" b/w e-paper, 400×300, SSD1683
- ESP32 e-Paper Driver Board (USB 5V, 24-pin FPC)

On the driver board, set the display switch to **3R / A**. If the image is faint or blank, try **0.47R / B**.

If the picture is blank or inverted after flashing, set `WEACT_PANEL_ALT` to `1` in `firmware/motorcycle_slot/config.h`. If it is sideways, change `DISPLAY_ROTATION`.

## Flash (Arduino IDE)

1. ESP32 board package (Espressif). Board: **ESP32 Dev Module**.
2. Libraries: **GxEPD2**, **Adafruit GFX**, **Adafruit BusIO**.
3. Open `firmware/motorcycle_slot/motorcycle_slot.ino`.
4. Upload. Serial **115200**.

`TEST_MODE` is on: BOOT or serial `s` respin; `1`–`4` force a bike; `j` forces jackpot.
