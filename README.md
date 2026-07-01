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

- **RP2040** (with WiFi): Raspberry Pi Pico W, Adafruit QT Py RP2040, etc.
- **RP2350** (with WiFi): Raspberry Pi Pico 2 W, etc.
- **ESP32-S2 / S3** (with WiFi and USB HID support)

> **Note**: The board must support USB HID (keyboard emulation) and have WiFi.

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

**Available environments**:

- `rp2040` – Pico W (RP2040)
- `rp2350` – Pico 2 W (RP2350)
- `esp32s2` – ESP32-S2
- `esp32s3` – ESP32-S3
- `rp2040_debug`, `rp2350_debug`, etc. – Debug builds with verbose logs

> **Note**: Most boards do not require pressing BOOTSEL manually. PlatformIO handles the upload automatically.

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

## 🗂️ Configuration

Settings are stored in a `config.json` file on the device's LittleFS partition.

Default values:

```json
{
  "wifi_ssid": "WiFiQuack",
  "wifi_passphrase": "WiFiQuack",
  "keyboard_layout": "FR"
}
```

You can also edit the file manually if you mount the LittleFS partition.

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

## Credits

- Inspired by [Spacehuhn](https://github.com/SpacehuhnTech) and the WiFi Duck project.

---

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.
