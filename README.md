# Motorcycle Slot Machine

On power-on, an ESP32 picks today's bike on a three-reel e-ink slot.

The glass on the bench is black, white, and red. That panel lands on the result. A black-and-white panel spins the reels, then settles. The switch is `PANEL_COLORS` in `firmware/motorcycle_slot/config.h`: `3` or `2`.

| Code | Bike |
|------|------|
| **R6** | 2020 Yamaha R6 |
| **R12** | BMW R12 |
| **TT** | Triumph Thruxton R |
| **HS** | Honda Shadow (red bodywork) |
| **JP** | Jackpot — *Buy another motorcycle.* (red bow) |

Bikes are equally likely. Jackpot is about 1% (`2/198`). Reels draw 128×96 side-view sprites. After the result the ESP32 sleeps; tap a button on **GPIO 4** (to GND) to roll again. Wall case: `hardware/enclosure/`. Sources live in `assets/pixel/`; rebuild with `python tools/png_to_bitmap.py`.

## Hardware

- WeAct 4.2" e-paper, 400×300
  - 3-color, black/white/red (GDEY042Z98) — `PANEL_COLORS 3`
  - 2-color, black/white (GDEY042T81) — `PANEL_COLORS 2`
- ESP32 e-Paper Driver Board (USB 5V, 24-pin FPC)

## Load it onto a board

Step-by-step for a first-time Arduino user (plug in the screen, install the IDE, upload): **[HOW-TO-LOAD.md](HOW-TO-LOAD.md)**.

## License

[MIT](LICENSE) for the firmware, the case, and the project docs. Photographs in `assets/pixel/src/` are reference art for the sprites and are not covered by that license.
