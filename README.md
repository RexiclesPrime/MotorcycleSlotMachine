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

## Load it onto a board

Step-by-step for a first-time Arduino user (plug in the screen, install the IDE, upload): **[HOW-TO-LOAD.md](HOW-TO-LOAD.md)**.
