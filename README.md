# GPS NMEA Parser — ESP32

A lightweight embedded C project for parsing raw NMEA 0183 sentences from a GPS module (NEO-6M) over UART on ESP32, built with ESP-IDF v6.0.

This project demonstrates:
- UART driver configuration and interrupt-driven reception on ESP32
- NMEA 0183 sentence parsing — GGA, RMC, GSA, GSV
- Extraction of latitude, longitude, UTC time, fix status, satellite count, and HDOP
- GPIO-based LED status indication
- Clean modular embedded C structure following ESP-IDF component conventions

---

## Hardware

| Component | Details |
|---|---|
| MCU | ESP32-WROOM-32E |
| GPS Module | NEO-6M (NMEA 0183 output) |
| Interface | UART1 — 9600 baud, 8N1 |
| Status LED | GPIO0 (blinks on valid NMEA frame received) |

### Wiring

| GPS Pin | ESP32 Pin | Notes |
|---|---|---|
| TX | GPIO16 (RX) | GPS transmits NMEA to ESP32 |
| RX | GPIO17 (TX) | ESP32 sends commands to GPS |
| VCC | 3.3V | NEO-6M operates at 3.3V |
| GND | GND | Common ground |

> UART1 is used so UART0 remains free for the serial monitor — no conflict between GPS data and debug output.

---

## NMEA Sentences Parsed

| Sentence | Data Extracted |
|---|---|
| `$GPGGA` | Latitude, Longitude, Fix quality, Satellite count, HDOP, Altitude |
| `$GPRMC` | Latitude, Longitude, Speed over ground, Course, UTC date/time, Status |
| `$GPGSA` | Fix type (2D/3D), DOP values (PDOP, HDOP, VDOP) |
| `$GPGSV` | Satellites in view, PRN, elevation, azimuth, SNR |

### Example parsed output

```
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
  → Fix: Valid | Lat: 48.1173°N | Lon: 11.5167°E | Sats: 8 | HDOP: 0.9 | Alt: 545.4m

$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
  → Status: Active | Speed: 22.4 knots | Course: 084.4° | Date: 23/03/94
```

---

## Project Structure

```
GPS_NEMA_Read/
├── CMakeLists.txt          # Top-level CMake (ESP-IDF project)
├── main/
│   ├── CMakeLists.txt      # Component registration
│   └── main.c              # UART init, NMEA read loop, LED indicator
├── sdkconfig               # ESP-IDF build configuration
└── README.md
```

---

## Configuration

Key defines in `main.c`:

| Define | Value | Description |
|---|---|---|
| `GPS_UART` | `UART_NUM_1` | UART peripheral — kept separate from monitor |
| `GPS_RX` | `GPIO_NUM_16` | UART RX pin |
| `GPS_TX` | `GPIO_NUM_17` | UART TX pin |
| `GPS_BAUD_RATE` | `9600` | NEO-6M default baud rate |
| `BUF_SIZE` | `512` | UART RX ring buffer size |
| `LED` | `GPIO_NUM_0` | Status LED — active LOW |

---

## Build and Flash

### Prerequisites
- ESP-IDF v6.0 installed — [setup guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
- ESP32 development board with NEO-6M connected as above

### Commands

```bash
# Set up ESP-IDF environment (once per terminal session)
. $IDF_PATH/export.sh          # Linux / macOS
%IDF_PATH%\export.bat          # Windows CMD

# Set target
idf.py set-target esp32

# Build
idf.py build

# Flash and open monitor
idf.py -p /dev/ttyUSB0 flash monitor    # Linux
idf.py -p COM3 flash monitor            # Windows
```

### Expected serial output

```
I (312) GPS: UART initialized — GPIO16 RX, GPIO17 TX, 9600 baud
I (314) GPS: Waiting for NMEA data...
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
$GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45*75
```

---

## How It Works

```
NEO-6M GPS
    │
    │ NMEA sentences @ 9600 baud
    │ (GGA, RMC, GSA, GSV — 1Hz)
    ▼
GPIO16 (UART1 RX)
    │
    ▼
ESP32 UART Driver
    │  ring buffer (512 bytes)
    │  1 second read timeout
    ▼
main loop
    ├── uart_read_bytes()
    ├── print raw NMEA to console
    └── GPIO0 LED blink → frame received
```

**NMEA frame format:**
```
$TTSSS,data,data,...,data*HH<CR><LF>
  │││                      │
  ││└── Sentence type      └── Checksum (XOR of all bytes between $ and *)
  │└─── Talker ID (GP = GPS)
  └──── Start delimiter
```

---

## Limitations and Next Steps

This project reads and prints raw NMEA sentences. It does not parse individual fields.

**Planned extensions:**
- Field-level parser — extract lat/lon, time, fix status from each sentence type
- Fix quality filtering — only process frames with valid fix (GGA field 6 = 1 or 2)
- Warm start support — send UBX AID-INI to GPS for faster lock on next boot
- FreeRTOS task — move UART read to dedicated task with queue handoff to main

---

## Requirements

- ESP-IDF v6.0+
- ESP32 (tested on ESP32-WROOM-32E)
- NEO-6M GPS module or any GPS with NMEA 0183 output at 9600 baud

---

## Related Projects

- **Offline GPS Geo-Fencing & Worker Safety Wearable** — production-grade ESP32 project using this NMEA parser as the GPS input layer, extended with Haversine geo-fence, FreeRTOS multi-task architecture, MAX30102 heart rate, MPU6050 fall detection, and BMP388 indoor floor detection *(private — MosChip internal R&D)*

---

## License

MIT — free to use, modify, and distribute.

---

## Author

**Abhishek Shiruru**
Senior Software Engineer — Embedded Systems
[LinkedIn](https://linkedin.com/in/abhishek-acharya-876abi)