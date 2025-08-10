#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

// Include ST ULD headers
extern "C" {
#include "third_party/vl53l1x_uld/vl53l1x_api.h"
#include "third_party/vl53l1x_uld/vl53l1_platform.h"
}

namespace esphome {
namespace sparkfun_vl53l1x {

// ESP-IDF-native shim driving the VL53L1X via ST's ULD; surface matches SparkFun features.
class SparkfunVL53L1X : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_distance_sensor(sensor::Sensor *s) { distance_sensor_ = s; }
  void set_distance_mode(int mode) { distance_mode_ = mode; }                // 0=short,1=medium,2=long
  void set_timing_budget_ms(int ms) { timing_budget_ms_ = ms; }
  void apply_prefs(int distance_mode, int timing_budget_ms) {
    distance_mode_ = distance_mode;
    timing_budget_ms_ = timing_budget_ms;
    reconfigure_ = true;
  }

  // PollingComponent
  void setup() override;
  void dump_config() override;
  void update() override;

 protected:
  // ULD glue
  bool uld_init_();
  bool uld_apply_config_();
  bool uld_read_distance_mm_(uint16_t &mm);

  // Platform hooks for ULD (implemented in platform .c via these statics)
  bool i2c_write_(uint16_t reg, uint8_t *data, uint16_t len);
  bool i2c_read_(uint16_t reg, uint8_t *data, uint16_t len);

  sensor::Sensor *distance_sensor_{nullptr};
  bool initialized_{false};
  bool reconfigure_{false};
  int distance_mode_{2};
  int timing_budget_ms_{50};

  // ULD state
  VL53L1_Dev_t dev_{};
};

}  // namespace sparkfun_vl53l1x
}  // namespace esphome
