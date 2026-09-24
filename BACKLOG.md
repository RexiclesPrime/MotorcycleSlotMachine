# Punch list

Reviewed `main` at `788aa67` on 2026-09-24: firmware, art tool, odds check, and the wall case. Boot is in good shape. Power-on draws the result with a full refresh, then deep sleep. The old "spins forever" hang is capped.

The BMW R12 reel uses the studio photo from `R12.webp`, mirrored to face right with the other bikes. Reel art is `assets/pixel/src/r9.sprite.png` (128×96). The untouched photo is `assets/pixel/src/r12.webp`. Rebuild with `python tools/png_to_bitmap.py`.

## Decided

- The bench glass is black/white/red. `PANEL_COLORS` is `3`, and that build lands on the result. Set it to `2` for the black-and-white panels. The reels animate only on 2-color glass.
- The button may reroll. "TODAY'S RIDE" is the label, not a once-a-day lock.
- On a 2-color spin, a reel may flash the jackpot. That fake-out stays.
- License is MIT (`LICENSE`). Photographs in `assets/pixel/src/` are not part of that grant.

## P0 — overlapping reels, or a case that will not screw together

1. **Reel frames overlap, and the sprite is wider than its window.** On a 400×300 panel, `layOut()` in `motorcycle_slot.ino` makes each window 125×198 with a 6px gap. The frame is drawn 6px outside the window, so neighboring frames sit on the same pixels. `paintBadge()` then centers the 128px-wide bitmap in that 125px window, so 1–2px of every bike lands on the frame. Widen the gap, or shrink the art to the window.

2. **The wall case has no plastic around the screws.** In `hardware/enclosure/slot_wall_case.scad`, the M3 heat-set holes are 7mm deep, cut through a 2.6mm back wall, with no boss. An insert there has nothing to grip. Add corner bosses at least as deep as the insert before the first print.

## P1 — names, the spin button, and the art tool

3. **R6 and R12 are still called R7 and R9 in the code.** The glass labels are R6 and R12. The enum, the weight macros, the bitmap symbols, and the source files are still `R7` / `R9`. `tools/odds_check.py` prints `R9` for the BMW. One set of names, used everywhere.

4. **Holding the spin button can roll a second time.** Production arms `esp_sleep_enable_ext0_wakeup` on GPIO4, level low, then sleeps. Ext0 wakes if the pin is already low, so a press that outlasts the refresh starts another spin. Test mode already ignores a stuck pin. Production should wait until GPIO4 is high, then sleep. Confirm on the board.

5. **`SPRITE_INVERT` flips only the black plane.** In `paintBadge()`, the red bitmap is always drawn with `drawBitmap`. On a 3-color panel, inverting the bikes leaves the Shadow's paint and the jackpot bow alone.

6. **The art tool flattens a dark bike.** `tools/png_to_bitmap.py` marks any pixel darker than 200 as ink, then nearest-neighbor scales. A black motorcycle becomes a blob, and a gray floor shadow becomes a puddle. That is why the R12 is an authored `r9.sprite.png` instead of a straight threshold of the photo. Downscale first, and drop the near-white background, before the next photo.

## P2 — housekeeping

7. **The test splash still says "sprites on".** `bootSplash()` is leftover debug copy. Production never shows it. A real title, or no splash.

8. **The odds table is copied in two places.** Weights live in `config.h` and again in `odds_check.py`, which is how the checker ended up saying R9. One table.

9. **GPIO12 is claimed as MISO.** The e-paper board does not need a MISO line, and GPIO12 is an ESP32 strapping pin (flash voltage). Not claiming it avoids a boot failure if the ribbon floats that pin.

10. **Charger wiring in the enclosure notes.** `hardware/enclosure/README.md` says a TP4056 with no OUT pads can feed the ESP32 from B+ / B-. That is fine until the board is charging and running at the same time. Call for a load-sharing charger if this lives on USB-C in the garage.

11. **No check that the art and the odds still match.** A small script run (odds check, plus a diff of `bitmaps.h` after `png_to_bitmap.py`) would catch the next drift. Nothing in the repo runs either today.

## Leave alone

- `esp_random() % 198` is a hair off a perfect 2/198 jackpot. Nobody will see it.
- The 4-page cap in the draw loops. With a full-height buffer the draw is one page. That cap is what stopped the boot hang.
