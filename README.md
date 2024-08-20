# ⏲️ Arduino Countdown

[![Platform](https://img.shields.io/badge/platform-Arduino-blue.svg)](https://www.arduino.cc/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](https://opensource.org/licenses/MIT)


## 👀 Overview

The **Arduino Countdown Timer** is a project designed to count down to a specific target date and time using an OLED display. The project uses the Arduino UNO R4 integrated RTC to keep track of time and synchronize with an NTP server over Wi-Fi. The OLED display shows the remaining time in days, hours, minutes, and seconds, along with various status icons for Wi-Fi connectivity and synchronization.

## ✨ Features

- **Real-time Countdown:** Displays the remaining time to a target date on an OLED display.
- **Wi-Fi Connectivity:** Connects to a Wi-Fi network to synchronize time with an NTP server.
- **RTC Module:** Uses a Real-Time Clock (RTC) module to maintain accurate time, even if the device loses power.
- **Status Icons:** Displays various icons for Wi-Fi status, synchronization, and warnings.

## ❗ Hardware Requirements

- Arduino UNO R4 WIFI
- 0.96" OLED Display (e.g., SSD1306)
- Connecting wires
- Battery for RTC

## 📚 Libraries Used

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
- [NTPClient Library](https://github.com/arduino-libraries/NTPClient)
- [WiFiNINA Library](https://github.com/arduino-libraries/WiFiNINA)

## 🚀 Getting Started

1. **Install the Required Libraries:**
   Ensure that the following libraries are installed in your Arduino IDE:
   - Adafruit GFX Library
   - Adafruit SSD1306 Library
   - NTPClient Library
   - WiFiNINA Library

2. **Wiring:**
   - Connect the OLED display to your Arduino board (VCC to 3.3V, GND to GND, SCL to SCL, SDA to SDA).
   - Connect a battery to the Arduino RTC Pin (VRTC to 3.3V, GND to GND).

3. **Configure Wi-Fi Credentials:**
   - Create a file named `arduino_secrets.h` in the project directory and add your Wi-Fi credentials:
     ```cpp
     #define SECRET_SSID "your_SSID"
     #define SECRET_PASS "your_PASSWORD"
     ```

4. **Upload the Code:**
   - Upload the provided sketch to your Arduino board.

5. **Run the Project:**
   - Once uploaded, the OLED display will show the countdown to your specified target date. The Wi-Fi icon will indicate the connection status, and the RTC module will ensure accurate timekeeping.

## 📝 Code Structure

The code is organized into the following sections:

- **Setup Function:** Initializes the display, connects to Wi-Fi, and sets the target date.
- **Loop Function:** Continuously updates the countdown and synchronizes the time with an NTP server if available.
- **Status Bar Function:** Displays icons for Wi-Fi connectivity, synchronization, and warnings.

## 🐞 Troubleshooting

- **No Wi-Fi Connection:** Ensure that your credentials in `arduino_secrets.h` are correct and that your board is within range of the Wi-Fi network.
- **RTC Power Loss:** If the RTC loses power, it will display a warning icon. Ensure the battery is connected correctly and has sufficient charge.

## 🫱 Contributing

Contributions are welcome! For feature requests, bug reports, or other suggestions, please create an issue or submit a pull request.

---

Happy coding!


