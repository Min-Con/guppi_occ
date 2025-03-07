# Zerø Effort

## Minimum Conatus

---

# Generally Unstable and Poorly Planned Implementations (GUPPI) - Occupancy and Presence Tracking Devices

A DIY home automation project using ESPHome on ESP32 hardware for occupancy and presence detection, with capabilities to monitor air quality, climate, illumination, and more. This project is designed to integrate seamlessly with Home Assistant, enabling users to create custom automations and dashboards based on sensor data.

---

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
  - ESP32-based device
    - Tested on the following:
      - ESP32-C3 Super Mini
      - ESP32-S3 Zero
      - Seeed Studio XIAO ESP32-C3
      - Seeed Studio XIAO ESP32-S3

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
  Monitor occupancy in an entryway using multiple sensors.

- **Sensors Included:**  
  - **Motion Sensor:** Detects movement (configured as a binary sensor).
  - **Ambient Light Sensor:** Measures light levels (configured as an ADC sensor).
  - *Optional:* Temperature and humidity sensors or additional sensors may be integrated as needed.

### Example Configuration: Entryway Occupancy Device Setup

This example demonstrates how to create a fully configured Entryway Occupancy device in ESPHome Builder by leveraging the provided templates and package selections. The workflow is as follows:

1. **Create New Device:**  
   Open ESPHome Builder and create a new device. Give it a meaningful name (e.g., `entryway_occupancy`).

2. **Copy Template Contents:**  
   Copy the contents of `defaults/00 - New Device Template.yaml` into your new device configuration. This template contains the foundational configuration needed for any new device.

3. **Update the Configuration:**  
   - Make any changes necessary as described in the template file (such as device-specific parameters).
   - Adjust WiFi settings, API, OTA passwords, etc.

4. **Include Required Packages:**  
   Open `defaults/packages.yaml` to see the available configuration packages. Then, update the `packages:` block in your configuration with the files you require for your device. This might include base configurations, sensor definitions, or extra features.

5. **Install the Device:**  
   Once your configuration is complete, run the install in ESPHome Builder to flash the device with the newly created configuration.

Below is an example configuration for an Entryway Occupancy device:

~~~yaml
substitutions:
# Specify below pin numbers for UART, GPIO, and I2C connections
# These are device specific and may need to be adjusted

  tx_pin_uart: GPIO21
  rx_pin_uart: GPIO20
  light_gpio: GPIO8
  motion_gpio: GPIO10
  sda_pin_i2c: GPIO8
  scl_pin_i2c: GPIO9

# Include required packages as defined in defaults/packages.yaml.
# Open defaults/packages.yaml for available package options and select the ones you need.
packages:
  project_configs: 
#    url: github://min-con/guppi_occ
    url: https://github.com/min-con/guppi_occ
    files: [defaults/GUPPI.ST-OCC-GP-2410-BLE-DEFAULTS.yaml, shared-configs/boards/esp32-s3-zero.yaml, shared-configs/network/wifi.yaml, shared-configs/network/bletracker.yaml, shared-configs/components/ld2410.yaml]
    ref: main
    refresh: 0s

esphome:
  name: entryway_occupancy
  friendly_name: "Entryway Occupancy Sensor"
  platform: ESP32
  board: esp32dev
  comment: "${comment_text}"
  project: 
    name: "${project_name}"
    version: "${project_version}"

# API integration for Home Assistant – update the password as needed.
api:
  password: "YOUR_API_KEY"
~~~

**Notes:**

- **Template & Packages:**  
  The above configuration is built on top of the default template (`defaults/00 - New Device Template.yaml`). Ensure that you follow the instructions within that file to make any device-specific changes.

- **Packages Block:**  
  The `packages:` block should be updated based on your needs and the options available in `defaults/packages.yaml`. You may add or remove package includes as required.

- **Installation:**  
  After finalizing your configuration in ESPHome Builder, click the install button to flash your ESP32 device with this new configuration.

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
  - Support for additional ESP32 boards.
  - Integration of advanced sensors such as the multi-target LD2450.
  - Expansion of environmental sensor options.
  - Implementation of visual and audio feedback features.

- **Documentation:**  
  Update this README and other documentation as needed to reflect your changes. Please review similar ESPHome/Home Assistant projects for community best practices.

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
- This project is inspired by various community projects and contributions.

---

For any questions or feedback, please open an issue on GitHub or contact the maintainer.
