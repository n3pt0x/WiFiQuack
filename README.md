# 🦆 WiFiQuack

**Wireless keystroke injection tool for RP2040, RP2350 and ESP32-S2/S3.**

WiFiQuack turns your compatible board into a BadUSB device with a web interface. Connect to its WiFi, write DuckyScript payloads in your browser, and run them with a single click.

---

## ✨ Features

- 🖥️ **DuckyScript support** – Write and execute payloads using the standard DuckyScript syntax.
- 🌐 **Web interface** – Manage scripts and device settings from any browser.
- 📡 **WiFi AP** – The device creates its own WiFi network for easy access.
- 💾 **Persistent settings** – SSID, password, and keyboard layout are saved across reboots.
- ⌨️ **Multi-layout support** – Choose from DE, US, ES, FR, IT, PT, SE, DK.
- 🔄 **Cross-platform** – Works on Windows, macOS, Linux.

---

## 📋 Supported Hardware

- **RP2040** (with WiFi): Raspberry Pi Pico W.
- **RP2350** (with WiFi): Raspberry Pi Pico 2 W.
- **ESP32-S2 / S3** (with WiFi and USB HID support)

> **Note**: The board must support USB HID (keyboard emulation) and have WiFi.

### 🔌 Compatible USB Dongles (ESP32-S2/S3)

- [LILYGO T-Dongle-S3](https://wiki.lilygo.cc/products/t-dongle-series/t-dongle-s3/)
- [AtomS3U ESP32S3 (USB-A)](https://shop.m5stack.com/products/atoms3u?variant=43911717945601)
- [ESP32-S2 USB-A (aliexpress)](https://fr.aliexpress.com/item/1005008859522143.html)

---

## ⚙️ Installation & Compilation

### Requirements

- [PlatformIO](https://platformio.org/) (recommended) or Arduino IDE
- USB cable
- Compatible board

### Build & Flash

```bash
# Clone the repository
git clone https://github.com/yourusername/WiFiQuack.git
cd WiFiQuack

# Build and upload
pio run -e <PIO_ENV> -t upload

# Optional: monitor serial output
pio device monitor -e <PIO_ENV>
```

#### Flash release

**ESP32-S2 / ESP32-S3 (esptool)**

1. Download `esp32_s2_firmware.factory.bin` or `esp32_s3_firmware.factory.bin` from the release page.
2. Connect the board in bootloader mode.
3. Flash with:

```bash
esptool.py --chip esp32s2 write_flash 0x0 esp32_s2_firmware.factory.bin
```

**Raspberry Pi Pico W / Pico 2 W (picotool)**

1. Download `pico_w_firmware.uf2` or `pico_2_w_firmware.uf2` from the release page.
2. Press and hold the **BOOTSEL** button.
3. Connect the Pico to your computer via USB.
4. Release the BOOTSEL button.

```bash
picotool load -f pico_w_firmware.uf2
picotool reboot
```

**Available environments**:

- `rp2040` – Pico W (RP2040)
- `rp2350` – Pico 2 W (RP2350)
- `esp32s2` – ESP32-S2
- `esp32s3` – ESP32-S3
- `rp2040_debug`, `rp2350_debug`, etc. – Debug builds with verbose logs

---

## 🕹️ Usage

### 1. Connect to the WiFi

After flashing, the device creates a WiFi network:

- **SSID**: `WiFiQuack`
- **Password**: `WiFiQuack`

### 2. Access the web interface

Open your browser and go to: [`http://192.168.4.1`](http://192.168.4.1)

### 3. Write and run a payload

1. Enter your DuckyScript in the text area.
2. Click **Run**.
3. The script will execute on the target machine.

### 4. Modify settings

Click **Settings** to change:

- WiFi SSID / password
- Keyboard layout (DE, US, ES, FR, IT, PT, SE, DK)

After saving, **reboot** the device for changes to take effect.

### 5. Reset to default

Use the **Reset Settings** button in the settings page to restore factory defaults.

---

## ⌨️ DuckyScript Reference

WiFiQuack supports a subset of DuckyScript, extended with mouse commands.

### 📝 Basic Syntax

- One command per line (case-insensitive)
- Comments: `#` or `REM`
- Parameters are space-separated

```text
REM This is a comment
STRING Hello, World!
ENTER
```

### ⌨️ Keyboard Commands

| Command                          | Syntax                                                  | Description                           |
| -------------------------------- | ------------------------------------------------------- | ------------------------------------- |
| `STRING`                         | `STRING <text>`                                         | Types the given text                  |
| `DELAY`                          | `DELAY <ms>`                                            | Waits for the specified milliseconds  |
| `DEFAULTDELAY`                   | `DEFAULTDELAY <ms>`                                     | Sets a default delay between commands |
| `ENTER`                          | `ENTER`                                                 | Presses Enter/Return                  |
| `CTRL` / `SHIFT` / `ALT` / `GUI` | `CTRL <key>`                                            | Modifier + key combination            |
| `COMBO`                          | `COMBO <key1> <key2> ...`                               | Presses multiple keys simultaneously  |
| `REPEAT` / `REPLAY` (block)      | `REPEAT <N> ... ENDREPEAT` / `REPLAY <N> ... ENDREPLAY` | Repeats a block N times               |
| `LOCALE`                         | `LOCALE <DE\|US\|ES\|FR\|IT\|PT\|SE\|DK>`               | Changes keyboard layout               |

**Modifier commands** (`CTRL`, `SHIFT`, `ALT`, `GUI`) support:

- Single character: `CTRL c`
- Named key: `CTRL F4`
- Right-side modifiers: `CTRL_RIGHT`, etc.

**Special Keys:** `ESC`, `BACKSPACE`, `TAB`, `SPACE`, `MENU`, `INSERT`, `DELETE`, `HOME`, `END`, `PAGEUP`, `PAGEDOWN`, `CAPSLOCK`, `PRINTSCREEN`, `SCROLLLOCK`, `PAUSE`, `UP`/`DOWN`/`LEFT`/`RIGHT`, `F1`–`F12`, `NUM_LOCK`, `NUM_0`–`NUM_9`, `NUM_DOT`, `NUM_ENTER`, `NUM_PLUS`, `NUM_MINUS`.

**Power Commands:** `POWER`, `RESET`, `SLEEP`

### 🖱️ Mouse Commands

| Command         | Syntax                                           | Description                                    |
| --------------- | ------------------------------------------------ | ---------------------------------------------- |
| `MOVE`          | `MOVE <x> <y>`                                   | Moves cursor by (x, y) pixels                  |
| `SCROLL`        | `SCROLL <amount>`                                | Scrolls wheel (positive = up, negative = down) |
| `CLICK`         | `CLICK [LEFT\|RIGHT\|MIDDLE\|MOUSE_ALL]`         | Clicks a button (default: LEFT)                |
| `MOUSE_PRESS`   | `MOUSE_PRESS [LEFT\|RIGHT\|MIDDLE\|MOUSE_ALL]`   | Presses and holds a button                     |
| `MOUSE_RELEASE` | `MOUSE_RELEASE [LEFT\|RIGHT\|MIDDLE\|MOUSE_ALL]` | Releases a button                              |

**Aliases:** `MOUSE_CLICK`, `MOUSE_MOVE`, `MOUSE_SCROLL`

### 📦 Advanced Commands

| Command          | Syntax                     | Description                              |
| ---------------- | -------------------------- | ---------------------------------------- |
| `KEYCODE`        | `KEYCODE <mod> <key>`      | Sends raw HID key codes (hex or decimal) |
| `COMBO`          | `COMBO <key1> <key2> ...`  | Presses multiple keys simultaneously     |
| `REPEAT` (block) | `REPEAT <N> ... ENDREPEAT` | Repeats a block N times                  |

### ⚠️ Notes

- **`DEFAULTDELAY`** affects all subsequent commands except `DELAY` itself.
- **`REPEAT`** without `ENDREPEAT` repeats only the **immediately previous line**.
- **`COMBO`** requires keys to be separated by spaces (e.g., `COMBO CTRL SHIFT ESC`).
- **`LOCALE`** changes the keyboard layout for all subsequent keyboard commands.

---

## 🔧 Troubleshooting

| Issue                         | Solution                                                                                           |
| :---------------------------- | :------------------------------------------------------------------------------------------------- |
| **Cannot connect to WiFi**    | Check the SSID/password. Reset settings if needed.                                                 |
| **Web interface not loading** | Ensure you are connected to the correct network and using `http://192.168.4.1`.                    |
| **Payload not executing**     | Verify the DuckyScript syntax. Check serial logs for errors.                                       |
| **Serial logs missing**       | The USB port may reset during keyboard initialization. Open the monitor after the device is ready. |

---

## 📄 License

This project is licensed under the **MIT License**. See the [LICENSE](LICENSE) file for details.

---

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.
