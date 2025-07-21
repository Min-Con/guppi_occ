# Zerø Effort <!-- omit in toc -->

## Generally Unstable and Poorly Produced Instruments (GUPPI) - Occupancy, Presence, and Environmental Monitoring Devices <!-- omit in toc -->

![GitHub commit activity](https://img.shields.io/github/commit-activity/m/Min-Con/guppi_occ?color=%2388B04B)
[![GitHub License](https://img.shields.io/github/license/Min-Con/guppi_occ?style=plastic&color=orange)](https://img.shields.io/github/license/Min-Con/guppi_occ?style=plastic&color=%23FF6F61)

![Static Badge](https://img.shields.io/badge/Espressif-gray?style=plastic&logo=espressif)
![Static Badge](https://img.shields.io/badge/ESPHome-green?style=plastic&logo=esphome)
![Static Badge](https://img.shields.io/badge/Home_Assistant-blue?style=plastic&logo=homeassistant)

Example configurations for DIY home automation projects using ESPHome with Home Assistant on ESP32 hardware for occupancy and presence detection, with capabilities to monitor air quality, climate, illumination, and more. This project is designed to integrate seamlessly with Home Assistant, enabling users to create custom automations and dashboards based on sensor data. <!-- omit in toc -->

---

- [Overview](#overview)
- [Repository Structure](#repository-structure)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Setup and Flashing](#setup-and-flashing)
- [Example: Entryway Occupancy](#example-entryway-occupancy)
  - [Overview](#overview-1)
  - [Example Configuration: Entryway Occupancy Device Setup](#example-configuration-entryway-occupancy-device-setup)
  - [Integration with Home Assistant](#integration-with-home-assistant)
- [Contribution Guidelines](#contribution-guidelines)
- [External Resources](#external-resources)
- [Future Enhancements \& Placeholders](#future-enhancements--placeholders)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Overview

- **Purpose:**  
  Provide a configurable, ESPHome-based platform for presence detection and occupancy monitoring in various rooms, with the flexibility to integrate additional sensors and functionalities.

- **Key Features:**  
  - Uses ESP32 hardware and ESPHome firmware.
  - Designed for basic users with an introduction to ESPHome and Home Assistant.
  - Modular configuration using defaults, device-specific settings, and shared configuration snippets.
  - Easily extendable with new sensor integrations, board support, and visual/audio feedback options.

---

## Repository Structure

- **defaults/**  
  Contains the basic configuration defaults used as a foundation for all devices. This folder is intended to be expanded with additional default configurations over time.

- **devices/**  
  Contains various examples of device-specific configurations. Although existing examples are provided for reference, they are not exhaustive.  
  *Note: Instead of focusing on an existing device example, we use the “Entryway Occupancy” example below as a reference.*

- **shared-configs/**  
  Contains configurations for commonly re-used sections as well as specific component configurations that can be combined with the defaults to create custom devices. This modular approach allows users to mix and match features (such as different boards, sensors and outputs) to suit specific needs.

---

## Getting Started

### Prerequisites

- **Hardware:**  
  - ***Required:*** ESP32-based device
    - Tested on the following:
      - ESP32 WROOM32
      - ESP32 D1 Mini
      - ESP32-C3 Super Mini
      - ESP32-S3 Zero
      - M5 Atom S3 / S3 Lite
      - M5 Stick C Plus
      - M5 Atom Lite / Pico
      - Seeed Studio XIAO ESP32-C3
      - Seeed Studio XIAO ESP32-S3
  - ***Presence:*** HiLink LD2410B, LD2410C, LD2450
  - ***Optional:*** BME680 (AQI), BME280 (Temp/Humidity), SCD41 (CO2), BH1750 (LUX)

- **Software:**  
  - [ESPHome Official Documentation](https://esphome.io/)
  - [Home Assistant Official Documentation](https://www.home-assistant.io/)

- **Experience Level:**  
  - Basic knowledge of ESPHome and Home Assistant configuration is assumed.

### Setup and Flashing

1. **Prepare Your Environment:**  
   Follow the [ESPHome Getting Started Guide](https://esphome.io/guides/getting_started_command_line.html) for instructions on preparing and flashing new devices.

2. **Configure the Device:**  
   - Start by copying the default configuration from the `defaults/` directory.
   - Combine with desired snippets from `shared-configs/` to include extra features.
   - Detailed guidance on configuring device-specific settings is provided below.

3. **Integrate with Home Assistant:**  
   - Once flashed and connected to your WiFi, the device will appear automatically in Home Assistant via the ESPHome integration.
   - Refer to the [Home Assistant ESPHome Integration Guide](https://www.home-assistant.io/integrations/esphome/) for further details on how to connect and manage your devices.

---

## Example: Entryway Occupancy

This example demonstrates how to set up an "Entryway Occupancy" device using a configuration based on the "Luke's Office Gopes 20250301.yaml" template (found in the `devices/` directory). The example below highlights sensor integration options.

### Overview

- **Function:**  
  Monitor presence in an entryway using multiple sensors.

- **Sensors Included:**  
  - **Bluetooth Tracker:** Detects the presence of tracked Bluetooth Low-Energy signals and updates the corresponding device_tracker information.
  - **Motion Sensor:** Detects movement (configured as a binary sensor).
  - **Radar Target:** Determines whether there is a target detected in any zone (configured as a binary sensor).
  - **Radar Moving Target:** Determines whether there is a target moving within any zone (configured as a binary sensor).
  - **Radar Still Target:** Determines whether there is a non-moving target within any zone (configured as a binary sensor).
  - **Other Sensors:** Multiple additional sensors to monitor location, distance, energy level, etc. of targets (configured as number and text sesnors).
  - *Optional:* Temperature, humidity, AQI, CO2, Altitude, Dew Point, and Luminance sensors or additional sensors may be integrated as needed.

### Example Configuration: Entryway Occupancy Device Setup

This example demonstrates how to create a fully configured Entryway Occupancy device in ESPHome Builder by leveraging the provided templates and package selections. The workflow is as follows:

1. **Create New Device:**  
   Open ESPHome Builder and create a new device. Give it a meaningful name (e.g., `entryway_occupancy`).

        BE SURE TO MAKE A COPY OF YOUR API KEY!!

2. **Copy Template Contents:**  
   Copy the contents of `defaults/00 - New Device Template.yaml`, according to the included instructions, into your new device configuration. This template contains the foundational configuration needed for any new device.

3. **Update the Configuration:**  
   - Make any changes necessary as described in the template file (such as device-specific parameters).
   - Adjust WiFi settings, API, OTA passwords, etc.

4. **Include Required Packages:**  
   Open `defaults/packages.yaml` to see the available configuration packages. Then, update the `packages:` block in your configuration with the files you require for your device. This might include base configurations, sensor definitions, or extra features.

5. **Install the Device:**  
   Once your configuration is complete, run the install in ESPHome Builder to flash the device with the newly created configuration.

Below is an example configuration for an Entryway Occupancy device:

~~~yaml
####################################
# Start Substitutions
####################################

substitutions:
# These substitutions may be used in multiple places within the configuration where they'll be replaced with these values when the configuration is compiled.
  name: entryway_occupancy_sensor
  friendly_name: Entryway Occupancy Sensor

# Specify below pin numbers for UART, GPIO, and I2C connections to override the defaults.
# These are device specific and may need to be adjusted if you choose different pins than those defined in the board configuration.
# Delete if not needed
#  tx_pin_uart: GPIO21
#  rx_pin_uart: GPIO20
#  light_gpio: GPIO8
#  motion_gpio: GPIO4
#  sda_pin_i2c: GPIO6
#  scl_pin_i2c: GPIO7

####################################
# End Substitutions
####################################

# Define external packages that ESPHome will fetch at compile time.
# Uncomment the dependencies you need, and add any additional ones you require.
# Delete any dependencies you don't need.
packages:
  project_configs: 
    url: https://github.com/min-con/guppi_occ
    files: [defaults/GUPPI.ST-OCC-GP-2410-DEFAULTS.yaml, shared-configs/defaults/entity-defaults.yaml, shared-configs/boards/seeed_xiao_esp32c3-esp-idf.yaml, shared-configs/network/wifi2.yaml, shared-configs/components/ld2410v2.yaml]
    ref: main
    refresh: 1s

esphome:
  name: ${name}
  friendly_name: ${friendly_name}

# API integration for Home Assistant – update with the API Key generated by ESPHome.
api:
  password: "YOUR_API_KEY"

ota:
  - platform: esphome
    id: ota_esphome
    password: "YOUR_PASSWORD"

~~~

**Notes:**

- **Template & Packages:**  
  The above configuration is built on top of the default template (`defaults/00 - New Device Template.yaml`). Ensure that you follow the instructions within that file to make any device-specific changes.

- **Packages Block:**  
  The `packages:` block should be updated based on your needs and the options described in the packages.md documentation. You may add or remove package includes as required.

- **Installation:**  
  After finalizing your configuration in ESPHome Builder, consider running Clean Build Files to confirm there are no issues before clicking the install button to flash your ESP32 device with this new configuration.

Following this workflow will result in a fully configured device that integrates seamlessly with Home Assistant, automatically exposing your sensors as entities for use in dashboards, automations, and more.

### Integration with Home Assistant

- **Automatic Entity Creation:**  
  When the device connects, its sensors and binary sensors will appear as entities in Home Assistant.
- **Usage in Automations:**  
  These entities can be used in cards, scenes, and automations. For example, an automation could trigger hallway lighting when occupancy is detected in the entryway.

---

## Contribution Guidelines

We welcome contributions to the GUPPI Occupancy Monitor project. Please follow these guidelines:

- **Fork & Branch:**  
  Fork the repository and create a feature branch for your contributions.

- **Coding Style:**  
  Follow the existing configuration style and maintain consistency with YAML formatting.

- **Testing:**  
  Ensure changes are tested on a compatible ESP32 device before submitting a pull request.

- **Future Enhancements:**  
  Potential areas for future additions include:
  - standardization of sensor and entities across different components of the same class.
  - Support for additional ESP32 boards.
  - Integration of additional sensors and components, such as more environmental sensors or displays.
  - Expansion of environmental sensor options.
  - Implementation of visual and audio feedback features.
  - Implementation of Improv protocol to simplify device configuration and updates.

- **Documentation:**  
  Perpetually in progress.

---

## External Resources

- **ESPHome Documentation:**  
  [https://esphome.io/](https://esphome.io/)

- **Home Assistant Documentation:**  
  [https://www.home-assistant.io/](https://www.home-assistant.io/)

- **Community Forums:**  
  - [ESPHome Community Forum](https://community.home-assistant.io/c/esphome)
  - [Home Assistant Community Forum](https://community.home-assistant.io)
  - Relevant discussions on Reddit (e.g., r/homeassistant, r/esp32)

---

## Future Enhancements & Placeholders

- **FAQs and Troubleshooting:**  
  *[Placeholder for future FAQ and troubleshooting documentation]*

- **Additional Configuration Examples:**  
  *[Placeholder for further sensor integrations and configuration options]*

---

## License

This project is licensed under the [MIT License](LICENSE).

---

## Acknowledgements

- Many thanks to the ESPHome and Home Assistant communities for their comprehensive documentation and support.
- This project is inspired by numerous community projects and contributions. There are too many to keep track of, but special thanks to those who have shared their work and knowledge.

