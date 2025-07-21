# ESPHome Configuration Breakdown Guide

This guide provides a comprehensive breakdown of all available configurations organized by purpose, with detailed explanations of differences within each category.

## Table of Contents

- [Board Configurations](#board-configurations)
- [Network Configurations](#network-configurations)
- [Sensor Components](#sensor-components)
- [Default Entity Configurations](#default-entity-configurations)
- [Configuration Selection Guide](#configuration-selection-guide)

---

## Board Configurations

Board configurations define the hardware-specific settings for different ESP32 variants, including pin assignments, framework settings, and hardware-specific optimizations.

### ESP32 Classic (Original ESP32) - Dual Core

| Configuration | Framework | Key Features | Best For |
|---------------|-----------|--------------|----------|
| `d1-mini-esp32dev.yaml` | ESP-IDF | WS2812 RGB LED support, comprehensive pin mapping | Projects needing addressable LEDs |
| `esp32-wroom-arduino.yaml` | Arduino | Simple configuration, basic features | Arduino IDE compatibility, beginners |
| `esp32-wroom-esp-idf.yaml` | ESP-IDF | Optimized performance, advanced features | Production deployments |
| `m5-atom-lite-pico.yaml` | ESP-IDF | Stability optimizations for ESP32 Pico | M5 Atom hardware, constrained memory |
| `m5-stick-cplus.yaml` | ESP-IDF | Memory optimizations, BLE + Display support | M5 Stick C Plus with display |

**Key Differences:**
- **Arduino vs ESP-IDF**: Arduino framework is easier but less optimized; ESP-IDF offers better performance and more features
- **Memory Management**: M5 configurations include specific memory optimizations for their constrained hardware
- **LED Support**: D1 Mini config includes WS2812 RGB LED driver configuration
- **Stability**: M5 Atom configs include specific stability fixes for ESP32 Pico chip variants

### ESP32-C3 (Single-core RISC-V)

| Configuration | Framework | Key Features | Best For |
|---------------|-----------|--------------|----------|
| `esp32-c3-devkitm-1-esp-idf.yaml` | ESP-IDF | Standard DevKit configuration | Development and prototyping |
| `esp32-c3-super-mini.yaml` | ESP-IDF | Compact form factor, basic LED | Small form factor projects |
| `seeed_xiao_esp32c3-esp-idf.yaml` | ESP-IDF | XIAO-specific optimizations, higher flash freq | Seeed XIAO ESP32-C3 boards |

**Key Differences:**
- **Form Factor**: Super Mini is optimized for the smallest ESP32-C3 boards
- **Flash Frequency**: XIAO variant supports 80MHz flash vs 40MHz for others
- **Pin Mapping**: Each has board-specific pin assignments and available GPIO
- **Performance**: All use ESP-IDF for better single-core performance

### ESP32-C6 (Single-core RISC-V + WiFi 6 + Thread/Zigbee)

| Configuration | Framework | Key Features | Best For |
|---------------|-----------|--------------|----------|
| `esp32-c6-super-mini.yaml` | ESP-IDF | WiFi 6, WS2812 RGB LED, Thread/Zigbee ready | Next-gen connectivity projects |
| `esp32-c6-zero.yaml` | ESP-IDF | Minimal footprint, WiFi 6 support | Compact WiFi 6 deployments |
| `seeed_xiao_esp32c6-esp-idf.yaml` | ESP-IDF | XIAO form factor, native USB, WiFi 6 | Professional XIAO C6 projects |

**Key Differences:**
- **Connectivity**: All support WiFi 6, Thread, and Zigbee (disabled by default)
- **LED Options**: Super Mini has WS2812 RGB; others have simple LEDs
- **USB Support**: XIAO variant has optimized native USB Serial/JTAG
- **Power Management**: Optimized for low power applications

### ESP32-S3 (Dual-core + PSRAM Support)

| Configuration | Framework | Key Features | Best For |
|---------------|-----------|--------------|----------|
| `esp32-s3-devkitc-1-arduino.yaml` | Arduino | Basic Arduino support only | Arduino IDE users |
| `esp32-s3-devkitc-1-esp-idf.yaml` | ESP-IDF | Full ESP-IDF features, PSRAM support | High-performance applications |
| `esp32-s3-zero.yaml` | ESP-IDF | Compact S3 board, full features | Small form factor with S3 power |
| `seeed_xiao_esp32s3-arduino.yaml` | Arduino | XIAO + Arduino, PSRAM support | XIAO boards with Arduino IDE |
| `seeed_xiao_esp32s3-esp-idf.yaml` | ESP-IDF | XIAO optimized, PSRAM, native USB | Professional XIAO S3 development |
| `m5-atom-s3.yaml` | ESP-IDF | M5 Atom S3, PSRAM, WS2812C RGB LED | M5 Atom S3 specific projects |
| `m5-atom-s3-lite.yaml` | ESP-IDF | M5 Atom S3 Lite, comprehensive I/O | M5 Atom S3 Lite with full features |

**Key Differences:**
- **PSRAM**: Most S3 configs support PSRAM for additional memory
- **USB**: Native USB CDC support for direct computer connection
- **Performance**: Dual-core provides better multitasking capability
- **M5 Variants**: Include specific M5 hardware features like buttons and Grove connectors

---

## Network Configurations

Network configurations handle WiFi and Bluetooth connectivity with different optimization strategies.

### WiFi Configurations

| Configuration | Optimization | Best For | Key Features |
|---------------|--------------|----------|--------------|
| `wifi.yaml` | Standard single network | Most applications | Fast connect, single SSID, power save |
| `wifi-multi.yaml` | Multiple networks | Mobile/roaming devices | Multiple SSIDs, automatic switching |
| `wifi2.yaml` | High performance | Dual-core boards | No power saving, optimal for S3/classic |
| `wifi3.yaml` | Memory optimized | Single-core boards | Minimal memory usage, C3/C6 optimized |

**Key Differences:**
- **Power Management**: wifi2 disables power saving for maximum performance; others use LIGHT mode
- **Network Count**: Multi supports up to 3 networks; others support single network with fast connect
- **Memory Usage**: wifi3 minimizes memory footprint for constrained devices
- **Fallback AP**: Some disable fallback hotspot to save memory

### Bluetooth Configurations

| Configuration | Purpose | Control Method | Best For |
|---------------|---------|----------------|----------|
| `bletracker.yaml` | BLE tracking + proxy | API connection controlled | Smart presence detection |
| `bleproxy.yaml` | Proxy only | Always active | Simple Bluetooth proxy |
| `bletracker-disabled.yaml` | Disabled example | Manual control | Reference/troubleshooting |

**Key Differences:**
- **Smart Control**: bletracker starts/stops based on API connection status
- **Resource Usage**: Proxy-only uses fewer resources than full tracking
- **Use Case**: Tracker is better for presence detection; proxy for device connectivity

---

## Sensor Components

Sensor components provide pre-configured sensor integrations with various feature levels.

### Environmental Sensors

#### BME280 Variants (Temperature, Humidity, Pressure)

| Configuration | Features | Complexity | Best For |
|---------------|----------|------------|----------|
| `bme280.yaml` | Basic readings + manual offsets | Simple | Basic environmental monitoring |
| `bme280v2.yaml` | Derived sensors + comfort analysis | Advanced | Comprehensive weather analysis |
| `bme280v3.yaml` | Weather trends + daily min/max | Comprehensive | Weather station applications |

**Key Differences:**
- **Derived Sensors**: v2/v3 add dew point, heat index, absolute humidity calculations
- **Analysis**: v3 includes weather tendency and pressure trend analysis
- **Daily Tracking**: v3 tracks daily temperature extremes with midnight reset
- **Complexity**: v1 is simple readings; v3 is a complete weather analysis system

#### BME680 Variants (Air Quality + Environmental)

| Configuration | Features | Readings | Best For |
|---------------|----------|----------|----------|
| `bme680.yaml` | Basic BSEC2 + offsets | AQI, VOC, CO2 equivalent | Air quality monitoring |
| `bme680v2.yaml` | Full environmental + weather analysis | All BME680 + weather trends | Complete indoor monitoring |

**Key Differences:**
- **BSEC2 Library**: Both use Bosch's advanced BSEC2 algorithm for air quality
- **Weather Analysis**: v2 adds comprehensive weather and comfort analysis
- **Offset Compensation**: Both support user-configurable sensor calibration
- **Complexity**: v2 includes everything from BME280v3 plus air quality

#### Other Environmental Sensors

| Configuration | Sensor Type | Features | Best For |
|---------------|-------------|----------|----------|
| `bh1750.yaml` | Light sensor | Calibrated lux readings | Light level monitoring |
| `ens160-aht21.yaml` | Air quality + temp/humidity | ENS160 AQI + AHT21 environmental | Dual-sensor air monitoring |
| `ens160-ccs811.yaml` | Dual air quality | ENS160 + CCS811 comparison | Research/validation setups |
| `scd41.yaml` | CO2 sensor | Placeholder for SCD41 | CO2 monitoring (incomplete) |

### Radar/Motion Sensors

#### LD2410 Variants (Single Target Radar)

| Configuration | Features | GPIO Support | Best For |
|---------------|----------|--------------|----------|
| `ld2410.yaml` | Full featured + GPIO motion | Yes | Complete presence detection |
| `ld2410-no-gp.yaml` | Radar only, no GPIO | No | UART-only installations |
| `ld2410v2.yaml` | Advanced zones + comprehensive controls | Yes | Professional installations |

**Key Differences:**
- **GPIO Integration**: Full versions include hardware motion pin support
- **Zone Detection**: All support configurable zone-based presence detection
- **Feature Completeness**: v2 includes all possible LD2410 features and controls
- **Complexity**: v2 has most configuration options; no-gp is simplest

#### LD2450 Variants (Multi-Target Radar)

| Configuration | Implementation | Features | Best For |
|---------------|----------------|----------|----------|
| `ld2450.yaml` | Custom/legacy | Multi-target tracking with zones | Advanced multi-target detection |
| `ld2450v2.yaml` | Native ESPHome | Built-in ESPHome LD2450 support | Standard multi-target radar |

**Key Differences:**
- **Implementation**: v1 uses custom code; v2 uses native ESPHome component
- **Complexity**: v1 has extensive custom zone configuration; v2 is simpler
- **Maintenance**: v2 is easier to maintain and update
- **Features**: v1 has more advanced zone exclusion and tracking features

---

## Default Entity Configurations

Default entity configurations provide standard diagnostic sensors and controls for all devices.

| Configuration | Purpose | Features | Best For |
|---------------|---------|----------|----------|
| `entity-defaults.yaml` | Standard diagnostics | Uptime, WiFi, temperature, memory | All projects |
| `entity-defaults2.yaml` | Alternative variant | Similar with different light handling | Special use cases |

**Key Differences:**
- **Light Control**: Different approaches to startup light blinking
- **Sensor Set**: Both include comprehensive diagnostic sensors
- **Memory Reporting**: Both track ESP memory usage and system health
- **Restart Controls**: Factory reset and reboot buttons included

---

## Configuration Selection Guide

### By Board Type

#### Single-Core Boards (C3, C6)
- **Network**: Use `wifi3.yaml` for memory optimization
- **BLE**: Consider `bleproxy.yaml` for lighter resource usage
- **Sensors**: Choose simpler sensor variants to preserve memory

#### Dual-Core Boards (Classic ESP32, S3)
- **Network**: Use `wifi2.yaml` for high performance or `wifi.yaml` for standard use
- **BLE**: `bletracker.yaml` works well with smart control
- **Sensors**: Can use comprehensive sensor variants (v2/v3)

### By Use Case

#### Basic Presence Detection
```yaml
# ESP32-C3 Super Mini Example
files: [
  shared-configs/boards/esp32-c3-super-mini.yaml,
  shared-configs/defaults/entity-defaults.yaml,
  shared-configs/network/wifi3.yaml,
  shared-configs/components/ld2410.yaml
]
```

#### Weather Station
```yaml
# ESP32-S3 Zero Example
files: [
  shared-configs/boards/esp32-s3-zero.yaml,
  shared-configs/defaults/entity-defaults.yaml,
  shared-configs/network/wifi2.yaml,
  shared-configs/components/bme280v3.yaml,
  shared-configs/components/bh1750.yaml
]
```

#### Air Quality Monitor
```yaml
# ESP32-C6 Super Mini Example
files: [
  shared-configs/boards/esp32-c6-super-mini.yaml,
  shared-configs/defaults/entity-defaults.yaml,
  shared-configs/network/wifi3.yaml,
  shared-configs/components/bme680v2.yaml
]
```

#### Advanced Multi-Target Detection
```yaml
# M5 Atom S3 Example
files: [
  shared-configs/boards/m5-atom-s3.yaml,
  shared-configs/defaults/entity-defaults.yaml,
  shared-configs/network/wifi2.yaml,
  shared-configs/components/ld2450v2.yaml,
  shared-configs/components/ens160-aht21.yaml
]
```

### Framework Selection Guidelines

#### Choose Arduino Framework When:
- Using Arduino IDE
- Need simple, familiar development environment
- Working with basic sensor readings
- Prioritizing ease of use over performance

#### Choose ESP-IDF Framework When:
- Need maximum performance and features
- Using advanced sensors (BME680 with BSEC2)
- Requiring BLE functionality
- Building production systems
- Working with memory-constrained devices

### Memory Optimization Tips

#### For Constrained Devices (C3, C6, M5 Atom):
- Use `wifi3.yaml` for network configuration
- Choose basic sensor variants (v1 instead of v2/v3)
- Consider `bleproxy.yaml` instead of full BLE tracking
- Monitor memory usage with built-in sensors

#### For High-Performance Devices (S3, Classic ESP32):
- Use `wifi2.yaml` for maximum network performance
- Utilize comprehensive sensor variants (v2/v3)
- Enable full BLE tracking with `bletracker.yaml`
- Take advantage of PSRAM when available

This guide should help you select the optimal combination of configurations for your specific hardware and use case requirements.