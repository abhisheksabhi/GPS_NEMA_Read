# GPS NMEA Read — ESP32

An ESP-IDF project that reads raw NMEA sentences from a GPS module over UART and prints them to the serial console. An LED blinks on each successful data reception.

---

## Hardware

| Signal | ESP32 Pin |
|--------|-----------|
| GPS TX → ESP32 RX | GPIO 16 |
| GPS RX → ESP32 TX | GPIO 17 |
| LED | GPIO 0 |
| GPS VCC | 3.3V |
| GPS GND | GND |

> **Note:** The project uses `UART_NUM_1` so `UART_NUM_0` remains free for the USB/serial monitor.

---

## Requirements

- [ESP-IDF v6.0](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/)
- ESP32 development board
- GPS module with NMEA output (e.g. NEO-6M, NEO-8M)

---

## Project Structure

```
GPS_NEMA_Read/
├── CMakeLists.txt          # Top-level CMake (ESP-IDF boilerplate)
├── main/
│   ├── CMakeLists.txt      # Component registration with driver dependency
│   └── main.c              # Application entry point
├── sdkconfig               # Generated ESP-IDF configuration
└── README.md
```

---

## Build & Flash

```bash
# Set up ESP-IDF environment (run once per terminal session)
. $IDF_PATH/export.sh        # Linux/macOS
# or
%IDF_PATH%\export.bat        # Windows CMD

# Configure target
idf.py set-target esp32

# Build
idf.py build

# Flash and monitor
idf.py -p COM3 flash monitor  # Replace COM3 with your port
```

---

## How It Works

1. UART1 is configured at **9600 baud**, 8N1, no flow control.
2. Pins GPIO16 (RX) and GPIO17 (TX) are assigned to UART1.
3. The main loop reads incoming bytes with a 1-second timeout.
4. Any received data is printed as a string to the console.
5. The LED on GPIO0 blinks briefly to indicate each received frame.

### Example NMEA output

```
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
```

---

## Configuration

Key defines in `main.c`:

| Define | Value | Description |
|--------|-------|-------------|
| `GPS_UART` | `UART_NUM_1` | UART peripheral used |
| `GPS_RX` | `GPIO_NUM_16` | RX pin |
| `GPS_TX` | `GPIO_NUM_17` | TX pin |
| `GPS_BAUD_RATE` | `9600` | GPS module baud rate |
| `BUF_SIZE` | `512` | UART read buffer size |
| `LED` | `GPIO_NUM_0` | LED indicator pin |

---

## License

MIT
