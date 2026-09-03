# Motorcycle Slot Machine

On power-on, an ESP32 spins a three-reel e-ink slot and picks today's bike.

| Code | Bike |
|------|------|
| **R7** | Yamaha R7 |
| **R12** | BMW R12 |
| **TT** | Triumph Thruxton R |
| **HS** | Honda Shadow (red bodywork) |
| **JP** | Jackpot — *Buy another motorcycle.* (red bow) |

Bikes are equally likely. Jackpot is about 1% (`2/198`). Reels draw 1-bit side-view sprites (112×80) plus the letter code. Sources live in `assets/pixel/`; rebuild with `python tools/png_to_bitmap.py`.

## Hardware

- WeAct 4.2" b/w e-paper, 400×300, SSD1683
- ESP32 e-Paper Driver Board (USB 5V, 24-pin FPC)

## Load it onto a board

Step-by-step for a first-time Arduino user (plug in the screen, install the IDE, upload): **[HOW-TO-LOAD.md](HOW-TO-LOAD.md)**.
