# How to load this onto the hardware

You need:

- A **WeAct 4.2" e-paper** module (the glass + short ribbon cable)
- An **ESP32 e-Paper Driver Board** (the small USB board with a ribbon-cable socket)
- A USB cable that actually carries data (not charge-only)
- A computer with [Arduino IDE](https://www.arduino.cc/en/software) (version 2 is fine)

No extra jumper wires. The screen plugs into the driver board; the driver board plugs into the computer.

## 1. Connect the screen to the driver board

E-paper glass is fragile. Hold it by the edges. Do not press the face.

1. On the driver board, find the **24-pin FPC socket** (the thin slot with a plastic latch).
2. Gently lift the latch.
3. Slide the screen’s ribbon in **straight and fully**. Gold contacts go toward the contacts in the socket. If it fights you, you have it upside down — flip the ribbon, don’t force it.
4. Press the latch back down so the cable is locked.
5. Set the small **display / resistor switch** to **A** (often labeled **3R**). Leave the SPI switch on **4-line** if the board has one.

Then plug the driver board into the computer with USB. A power LED should light. If Windows pops up “USB device,” that is normal.

## 2. Install Arduino IDE and the ESP32 board package

1. Install Arduino IDE and open it.
2. **File → Preferences**. In *Additional boards manager URLs* paste:

   `https://espressif.github.io/arduino-esp32/package_esp32_index.json`

   Click OK.
3. **Tools → Board → Boards Manager…**
4. Search **esp32**. Install **esp32** by **Espressif Systems**. Close the manager.

## 3. Install the three libraries

This step is required. Installing only GxEPD2 is not enough — the fonts live in Adafruit GFX.

**Sketch → Include Library → Manage Libraries…** Search each name below, click **Install**, and accept **Install all** if it asks for dependencies.

| Search for | Author to pick |
|------------|----------------|
| `GxEPD2` | Jean-Marc Zingg |
| `Adafruit GFX Library` | Adafruit |
| `Adafruit BusIO` | Adafruit |

To confirm GFX is actually there: **Sketch → Include Library** should list **Adafruit GFX Library**. If it does not, install it again (close and reopen the Library Manager if the list looks stale).

## 4. Open this project and pick the board

1. Get a **fresh** copy: GitHub **Code → Download ZIP**, unzip it, and throw away any older `MotorcycleSlotMachine-main` folder. Do not copy one new file into an old sketch folder — `bitmaps.h` has to sit next to the `.ino`.
2. In Arduino IDE: **File → Open…** and choose

   `firmware/motorcycle_slot/motorcycle_slot.ino`

   That folder must contain `motorcycle_slot.ino`, `config.h`, and `bitmaps.h`.
3. **Tools → Board → esp32 → ESP32 Dev Module**
4. **Tools → Port** → the new COM port that appeared when you plugged the board in.

If there is no port: try another USB cable, another USB jack, or install a **CP2102** (or **CH340**) USB driver, then unplug and replug.

## 5. Upload

Click **Upload** (the right arrow). Wait until the bottom log says **Done uploading**.

If it sits on “Connecting…” or fails to sync:

1. Hold **BOOT** on the driver board.
2. Tap **EN** / **RESET** once.
3. Release **BOOT**.
4. Click Upload again.

## 6. What you should see

Production boot: the board spins **once**, draws **TODAY'S RIDE** (or **JACKPOT**), then sleeps. The picture stays with power off. A 3-color refresh can take 15–20 seconds — wait for the footer, not a splash screen.

Optional **spin button**: 16 mm momentary switch from **GPIO 4 (IO4)** to **GND**. Tap it to wake and spin again. **BOOT is only for flashing**, not for spinning. See `hardware/enclosure/` for the wall case.

If the screen goes blank or crazy, your glass is probably black/white only: set `#define PANEL_3COLOR 0` in `config.h` and upload again. If the bikes look inverted, set `#define SPRITE_INVERT 1`.

Serial **115200**: `land ->` then `done`. To stay awake and force bikes, set `TEST_MODE` to `1` (`s` / `1`–`4` / `j`).

## If compile fails

**`Fonts/FreeSansBold9pt7b.h: No such file or directory`**

Adafruit GFX is missing. Repeat section 3 and install **Adafruit GFX Library** by Adafruit (and **Adafruit BusIO** if prompted). Then **Sketch → Verify/Compile** again. Restart Arduino IDE if it still cannot see the library.

**`GxEPD2_BW.h: No such file or directory`**

Install **GxEPD2** by Jean-Marc Zingg.

## If the screen stays blank

Work down this list. Change **one** thing at a time, then upload again.

1. Ribbon fully seated, latch closed, switch on **A**.
2. Open `firmware/motorcycle_slot/config.h` and set `#define WEACT_PANEL_ALT 1`
3. Still blank: try the switch on **B**.
4. Picture is there but sideways: set `#define DISPLAY_ROTATION 1` (then `3` if needed).
5. Picture is inverted or garbage: `WEACT_PANEL_ALT` the other way (`0` or `1`).
