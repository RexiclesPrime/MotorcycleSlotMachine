# Garage wall enclosure

Two-part PETG case: 4.2" e-paper on the front, ESP32 driver board + 18650 + charger behind, 16 mm **SPIN** button under the screen, keyholes to hang it.

Open `slot_wall_case.scad` in [OpenSCAD](https://openscad.org/). Set `part = "front";` then **F6 → Export STL**. Repeat with `part = "back";`.

Print **PETG** (a summer garage will warp PLA). 0.2 mm layers, 4 perimeters, 25 % infill, no supports if the USB cutouts print with bridging.

Outside is about **100 × 120 × 40 mm**. Measure your glass before the first print; the pocket is 91.5 × 77.5 mm for the usual 91 × 77 × 1.2 mm 4.2" panel.

## Spin button

Use a **16 mm stainless anti-vandal momentary (1NO)** panel switch. Garage dust and gloves, no arcade-button grit trap. Search: `16mm anti-vandal momentary 1NO stainless`. About $6. Panel thickness here is 2.6 mm (the switch’s spec range is usually 1–10 mm).

Skip the LED unless you want it — the ESP32 is asleep most of the time, so a lit button is extra drain.

Wire:

- One switch pin → **GND**
- Other switch pin → **IO4** on the ESP32 driver board (RTC pin, firmware `PIN_SPIN_BUTTON 4`)

Do **not** use BOOT (GPIO0). That is for flashing.

## Battery

Waveshare’s **5V** pin is specified **3.6–5.5 V**, so a single Li-ion cell can feed it directly.

| Part | Why |
|------|-----|
| **Protected 18650** (Panasonic NCR18650B or similar, with the PCB cap) | Garage-safe, ~2500–3400 mAh. Deep sleep + e-ink is months, not hours. |
| **TP4056 Type-C charger with protection** (DW01 / FS8205) | Charge through a hole in the bottom. Do not buy the tiny unprotected-only boards. |

Wiring:

```
USB-C  →  TP4056  B+ / B-  →  18650
                 OUT+  →  ESP32  5V pin
                 OUT-  →  ESP32  GND
```

If your TP4056 has no separate OUT pads, B+ / B- *are* the load — that is still 3.7–4.2 V into the 5V pin, which the board allows.

Leave the ESP32’s own USB for firmware, accessed through the side window or by popping the back.

## Assembly

1. Press four **M3 heat-set inserts** into the back bosses.
2. Stick a 1 mm EVA / craft-foam ring on the back’s glass rib (only the frame, not the picture area).
3. Drop the glass into the front pocket. FPC folds down into the bay — do not crease it sharply.
4. Seat the ESP32 (USB toward the side window), 18650 in the trough, TP4056 USB-C in the bottom cutout. Hot-glue the charger if it rattles.
5. Screw the back on with **M3×12**. Do not pinch the glass; the foam should just kiss the border.
6. Hang on two #8 pan-head screws, 60 mm apart, 14 mm down from the top.

Power on: wait 15–20 s for **TODAY'S RIDE**. Tap **SPIN** to wake and roll again.
