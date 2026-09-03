# Motorcycle Slot Machine

On power-on, an ESP32 spins a three-reel e-ink slot and picks today's bike.

| Code | Bike |
|------|------|
| **R6** | 2020 Yamaha R6 |
| **R12** | BMW R12 |
| **TT** | Triumph Thruxton R |
| **HS** | Honda Shadow (red bodywork) |
| **JP** | Jackpot — *Buy another motorcycle.* (red bow) |

Bikes are equally likely. Jackpot is about 1% (`2/198`). Reels draw 128×96 side-view sprites. Power-on spins once, then the ESP32 sleeps; tap a button on **GPIO 32** (to GND) to spin again. Sources live in `assets/pixel/`; rebuild with `python tools/png_to_bitmap.py`.

## Hardware

- WeAct 4.2" b/w e-paper, 400×300, SSD1683
- ESP32 e-Paper Driver Board (USB 5V, 24-pin FPC)

## Load it onto a board

Step-by-step for a first-time Arduino user (plug in the screen, install the IDE, upload): **[HOW-TO-LOAD.md](HOW-TO-LOAD.md)**.
