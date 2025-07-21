# Common Pins Configuration

These pins are configured as defaults for the purposes of these specific ESPHome projects. They do not necessarily reflect the hardware default pin assignments for each ESP32 microcontroller variant, but represent commonly used pin assignments for consistent project configurations.

## ESP32 Boards Pin Assignments

| Board                        | LED/Light GPIO     | UART TX  | UART RX  | I²C SDA | I²C SCL | Mic ADC Pin | Motion GPIO | Notes |
| ---------------------------- | ------------------ | -------- | -------- | ------- | ------- | ----------- | ----------- | ----- |
| **ESP32 D1 Mini**            | GPIO 2             | GPIO 17  | GPIO 16  | GPIO 21 | GPIO 22 | GPIO 36     | GPIO 13     | Built-in LED on some boards |
| **ESP32 WROOM (Arduino)**    | GPIO 2             | GPIO 17  | GPIO 16  | GPIO 21 | GPIO 22 | GPIO 36     | GPIO 4      | Standard ESP32 dev board |
| **ESP32 WROOM (ESP-IDF)**    | GPIO 2             | GPIO 17  | GPIO 16  | GPIO 21 | GPIO 22 | GPIO 36     | GPIO 4      | Standard ESP32 dev board |
| **ESP32-C3 DevKitM-1**      | GPIO 8             | GPIO 21  | GPIO 20  | GPIO 8  | GPIO 9  | -           | GPIO 10     | Simple LED |
| **ESP32-C3 Super Mini**     | GPIO 4             | GPIO 21  | GPIO 20  | GPIO 8  | GPIO 9  | GPIO 0      | GPIO 10     | Simple LED |
| **ESP32-C6 Super Mini**     | GPIO 8             | GPIO 16  | GPIO 17  | GPIO 6  | GPIO 7  | GPIO 2      | GPIO 10     | WS2812 RGB LED |
| **ESP32-C6 Zero**           | GPIO 8             | GPIO 5   | GPIO 4   | GPIO 21 | GPIO 22 | GPIO 2      | GPIO 7      | Simple LED |
| **ESP32-S3 DevKitC-1**      | GPIO 21            | GPIO 43  | GPIO 44  | GPIO 6  | GPIO 7  | -           | GPIO 2      | Simple LED |
| **ESP32-S3 Zero**           | GPIO 21            | GPIO 43  | GPIO 44  | GPIO 4  | GPIO 5  | GPIO 36     | GPIO 8      | Simple LED |
| **Seeed XIAO ESP32-C3**     | GPIO 8             | GPIO 21  | GPIO 20  | GPIO 6  | GPIO 7  | GPIO 3      | GPIO 4      | Simple LED |
| **Seeed XIAO ESP32-C6**     | GPIO 15            | GPIO 6   | GPIO 7   | GPIO 4  | GPIO 5  | GPIO 2      | GPIO 3      | Onboard LED |
| **Seeed XIAO ESP32-S3**     | GPIO 21            | GPIO 43  | GPIO 44  | GPIO 5  | GPIO 6  | GPIO 36*    | GPIO 4*     | Simple LED |
| **M5 Atom Lite/Pico**       | GPIO 27            | GPIO 21  | GPIO 19  | GPIO 25 | GPIO 22 | GPIO 33     | GPIO 23     | Simple LED |
| **M5 Atom S3**              | GPIO 35            | GPIO 43  | GPIO 44  | GPIO 38 | GPIO 39 | -           | GPIO 10     | WS2812C RGB LED |
| **M5 Atom S3 Lite**         | GPIO 35            | GPIO 5   | GPIO 6   | GPIO 7  | GPIO 8  | GPIO 38     | GPIO 39     | WS2812C RGB LED |
| **M5 Stick C Plus**         | GPIO 10            | GPIO 32  | GPIO 33  | GPIO 0  | GPIO 32 | GPIO 36     | GPIO 25     | Simple LED |

## Special Pin Considerations

### LED/Light Configuration Types
- **Simple LED**: Binary on/off LED using GPIO output
- **WS2812/WS2812C RGB LED**: Addressable RGB LED requiring special driver
- **Built-in LED**: Some boards have onboard status LEDs

### UART Pin Notes
- Most configurations use hardware UART2 (not UART0 which is typically used for programming)
- UART0 pins (GPIO1/GPIO3) are generally avoided to prevent programming conflicts
- Some boards use different UART pin assignments based on available GPIO

### ADC Pin Considerations
- **ESP32 Classic**: GPIO36 is input-only, ideal for ADC applications
- **ESP32-C3/C6**: GPIO0-GPIO6 have ADC capability, but GPIO0 should be used carefully (boot pin)
- **ESP32-S3**: Multiple ADC channels available, GPIO36 commonly used when available

### I2C Pin Standards
- **ESP32 Classic**: GPIO21 (SDA) / GPIO22 (SCL) are hardware defaults
- **ESP32-C3**: Often uses GPIO4/GPIO5 or GPIO6/GPIO7 pairs
- **ESP32-S3**: GPIO4/GPIO5 or GPIO6/GPIO7 commonly used
- **ESP32-C6**: GPIO4/GPIO5 or GPIO6/GPIO7 standard pairs

### Pins to Generally Avoid
- **GPIO0**: Boot mode selection (use with caution)
- **GPIO6-GPIO11**: Connected to flash memory on most boards (DO NOT USE)
- **GPIO15**: Boot strapping pin (has pull-down resistor)
- **Programming pins**: GPIO1/GPIO3 (UART0) should be avoided for peripherals

### Framework Considerations
- **Arduino Framework**: Generally more permissive with pin assignments
- **ESP-IDF Framework**: More optimized, some configurations require specific sdkconfig options
- **PSRAM**: Available on some ESP32-S3 and M5 Atom S3 boards for additional memory

## Usage Notes

1. **Substitutions**: All pin assignments use ESPHome substitutions and can be overridden in specific device configurations
2. **Board Variants**: Some boards may have different pin layouts even within the same model line
3. **Power Considerations**: High-current devices should not be directly connected to GPIO pins
4. **Pull-up/Pull-down**: Some pins have built-in resistors that may affect circuit behavior
5. **Input-only Pins**: Some GPIO pins (like GPIO36 on ESP32) are input-only and cannot drive outputs

## Framework and Optimization Notes

- Most configurations include performance optimizations in sdkconfig_options
- BLE support is enabled where hardware supports it
- WiFi compatibility options are configured for various network types
- Watchdog timeouts are extended to prevent unnecessary reboots during development