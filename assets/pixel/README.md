# Pixel art

Side-view 1-bit sprites for the three reels.

| File | Bike |
|------|------|
| `src/r7.jpg` | 2020 Yamaha R6 |
| `src/r9.jpg` | BMW R12 photo, mirrored so it faces right like the other reels |
| `src/r9.sprite.png` | BMW R12 reel art, already 128×96. The tool uses this as-is |
| `src/r12.webp` | BMW R12 studio photo, original (faces left) |
| `src/tt.jpg` | Triumph Thruxton R (cafe fairing) |
| `src/hs.jpg` | Honda Shadow (red body) |
| `src/jp.jpg` | Jackpot (gift-wrapped extra bike) |

Filenames `r7` and `r9` are older names. On the glass those reels are **R6** and **R12**.

`python tools/png_to_bitmap.py` writes 128×96 `1bit/*.png` and `firmware/motorcycle_slot/bitmaps.h`. A `src/<name>.sprite.png` at exactly 128×96 is copied through. Anything else is thresholded from the photo.

The MIT license does not cover these photographs. They are references for the reel sprites.
