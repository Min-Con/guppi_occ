#include "sparkfun_vl53l1x.h"
#include "esphome/core/log.h"
#include <vector>

extern "C" {
#include "third_party/vl53l1x_uld/vl53l1x_api.h"
#include "third_party/vl53l1x_uld/vl53l1_platform.h"
}

namespace esphome {
namespace sparkfun_vl53l1x {

static const char *const TAG = "sparkfun_vl53l1x";

// Forward-declare a singleton handle for ULD I2C glue (simple, one-device case).
// If you need multiple sensors, replace with a small registry keyed by I²C address.
extern "C" {
  static SparkfunVL53L1X *g_vl53 = nullptr;
}

// -------------------- ESPHome lifecycle --------------------

void SparkfunVL53L1X::setup() {
  ESP_LOGI(TAG, "VL53L1X: setup()");
  // Provide global handle for C-linkage I2C shims
  g_vl53 = this;

  // Bind ULD device context
  // Many ULD ports treat I2cDevAddr as 7-bit; the API takes care of R/W bit.
  this->dev_.I2cDevAddr = this->address_;
  this->dev_.comms_speed_khz = 400;
  this->dev_.comms_type = 1; // I2C

  if (!this->uld_init_()) {
    ESP_LOGE(TAG, "VL53L1X init failed");
    this->mark_failed();
    return;
  }

  if (!this->uld_apply_config_()) {
    ESP_LOGW(TAG, "VL53L1X initial config could not be fully applied");
  }

  this->initialized_ = true;
}

void SparkfunVL53L1X::dump_config() {
  ESP_LOGCONFIG(TAG, "VL53L1X (SparkFun-style shim):");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  Distance mode: %d (0=short,1=medium,2=long)", this->distance_mode_);
  ESP_LOGCONFIG(TAG, "  Timing budget: %d ms", this->timing_budget_ms_);
  LOG_UPDATE_INTERVAL(this);
}

void SparkfunVL53L1X::update() {
  if (!this->initialized_)
    return;

  if (this->reconfigure_) {
    ESP_LOGI(TAG, "Applying new prefs: mode=%d, tBudget=%dms",
             this->distance_mode_, this->timing_budget_ms_);
    if (!this->uld_apply_config_()) {
      ESP_LOGW(TAG, "Reconfiguration failed; keeping previous settings");
    }
    this->reconfigure_ = false;
  }

  uint16_t mm = 0;
  if (!this->uld_read_distance_mm_(mm)) {
    if (this->distance_sensor_ != nullptr)
      this->distance_sensor_->publish_state(NAN);
    return;
  }

  if (this->distance_sensor_ != nullptr)
    this->distance_sensor_->publish_state(static_cast<float>(mm));
}

// -------------------- ULD glue (init/config/read) --------------------

bool SparkfunVL53L1X::uld_init_() {
  // 1) Wait for boot & probe status
  uint8_t booted = 0;
  // Try up to ~10ms, keep it snappy to avoid blocking the ESPHome loop
  uint32_t t0 = millis();
  while ((millis() - t0) <= 10) {
    if (VL53L1X_BootState(&this->dev_, &booted) != 0) {
      // If platform not ready yet, brief yield
      delay(1);
      continue;
    }
    if (booted) break;
    delay(1);
  }
  if (!booted) {
    ESP_LOGW(TAG, "Sensor did not report booted within 10 ms");
    // Continue; some ULDs allow proceeding to SensorInit anyway
  }

  // 2) Initialize sensor with ULD defaults
  if (VL53L1X_SensorInit(&this->dev_) != 0) {
    ESP_LOGE(TAG, "VL53L1X_SensorInit failed");
    return false;
  }

  // 3) Set a sane inter-measurement period (>= timing budget)
  uint16_t inter_ms = static_cast<uint16_t>(std::max(this->timing_budget_ms_ + 5, 33));
  (void)VL53L1X_SetInterMeasurementInMs(&this->dev_, inter_ms);

  // 4) Start continuous ranging; actual mode/budget applied in uld_apply_config_
  if (VL53L1X_StartRanging(&this->dev_) != 0) {
    ESP_LOGE(TAG, "VL53L1X_StartRanging failed");
    return false;
  }

  return true;
}

bool SparkfunVL53L1X::uld_apply_config_() {
  // Map our 0/1/2 (short/med/long) to ULD enum.
  // Some ULD variants use: 1=short, 2=long, (no 3). Others: 1=short,2=medium,3=long.
  uint8_t uld_mode = 3; // try "long" first
  switch (this->distance_mode_) {
    case 0: uld_mode = 1; break; // short
    case 1: uld_mode = 2; break; // medium (or long in 2-mode ULDs)
    case 2: default: uld_mode = 3; break; // long (if not supported, we'll fall back)
  }

  int st = VL53L1X_SetDistanceMode(&this->dev_, uld_mode);
  if (st != 0) {
    // Fallback for 2-mode ULDs (1=short, 2=long)
    uint8_t fallback = (this->distance_mode_ == 0) ? 1 : 2;
    st = VL53L1X_SetDistanceMode(&this->dev_, fallback);
    if (st != 0) {
      ESP_LOGW(TAG, "SetDistanceMode failed (mode=%d)", this->distance_mode_);
      // continue; keep defaults
    }
  }

  // Timing budget (µs in ULD). Datasheet recommends >=20ms.
  uint32_t tb_us = static_cast<uint32_t>(this->timing_budget_ms_) * 1000UL;
  if (tb_us < 20000UL) tb_us = 20000UL;
  st = VL53L1X_SetTimingBudgetInUs(&this->dev_, tb_us);
  if (st != 0) {
    ESP_LOGW(TAG, "SetTimingBudgetInUs(%lu) failed", (unsigned long)tb_us);
  }

  // Inter-measurement period must be >= timing budget.
  uint16_t inter_ms = static_cast<uint16_t>(std::max(this->timing_budget_ms_ + 5, 33));
  (void)VL53L1X_SetInterMeasurementInMs(&this->dev_, inter_ms);

  // Restart continuous ranging to apply changes deterministically
  (void)VL53L1X_StopRanging(&this->dev_);
  st = VL53L1X_StartRanging(&this->dev_);
  if (st != 0) {
    ESP_LOGW(TAG, "StartRanging after reconfig failed");
  }

  return true;
}

bool SparkfunVL53L1X::uld_read_distance_mm_(uint16_t &mm) {
  // Quick poll for data ready (tiny bounded loop)
  uint8_t ready = 0;
  for (int i = 0; i < 3; ++i) {
    if (VL53L1X_CheckForDataReady(&this->dev_, &ready) != 0) break;
    if (ready) break;
    delay(1);
  }

  VL53L1X_Result_t res{};
  if (VL53L1X_GetResult(&this->dev_, &res) != 0) {
    ESP_LOGD(TAG, "GetResult failed");
    return false;
  }

  // Clear interrupt to advance to next frame
  (void)VL53L1X_ClearInterrupt(&this->dev_);

  // RangeStatus==0 → valid. We still publish distance; upstream can filter if needed.
  mm = res.Distance;
  return true;
}

// -------------------- I2C helpers used by the ULD glue --------------------
// These do not block for long and use ESPHome's i2c::I2CDevice API.

bool SparkfunVL53L1X::i2c_write_(uint16_t reg, uint8_t *data, uint16_t len) {
  // Build [reg_hi, reg_lo, payload...]
  std::vector<uint8_t> buf;
  buf.reserve(static_cast<size_t>(2 + len));
  buf.push_back(static_cast<uint8_t>(reg >> 8));
  buf.push_back(static_cast<uint8_t>(reg & 0xFF));
  if (data != nullptr && len > 0) {
    buf.insert(buf.end(), data, data + len);
  }

  auto err = this->write(buf.data(), buf.size(), true /* send STOP */);
  if (err != i2c::ERROR_OK) {
    ESP_LOGV(TAG, "I2C write reg=0x%04X len=%u err=%d", reg, (unsigned)len, (int)err);
    return false;
  }
  return true;
}

bool SparkfunVL53L1X::i2c_read_(uint16_t reg, uint8_t *data, uint16_t len) {
  // Write register (no STOP), then read
  uint8_t hdr[2] = {
      static_cast<uint8_t>(reg >> 8),
      static_cast<uint8_t>(reg & 0xFF),
  };
  auto werr = this->write(hdr, 2, false /* no STOP: repeated start */);
  if (werr != i2c::ERROR_OK) {
    ESP_LOGV(TAG, "I2C write(reg addr) failed reg=0x%04X err=%d", reg, (int)werr);
    return false;
  }
  auto rerr = this->read(data, len);
  if (rerr != i2c::ERROR_OK) {
    ESP_LOGV(TAG, "I2C read failed reg=0x%04X len=%u err=%d", reg, (unsigned)len, (int)rerr);
    return false;
  }
  return true;
}

}  // namespace sparkfun_vl53l1x
}  // namespace esphome

// -------------------- C-linkage ULD platform hooks --------------------
// Many ULD drops call these to perform I2C. We route them to the active shim instance.

extern "C" {

// The simple approach: route to the single active instance (g_vl53).
// For multiple sensors, implement a registry keyed by dev7 (7-bit address).

extern esphome::sparkfun_vl53l1x::SparkfunVL53L1X *g_vl53;

// Return 0 on success, nonzero on error (per several ULD examples)
uint8_t VL53L1_I2CWrite(uint16_t dev7, uint16_t reg, uint8_t *data, uint16_t len) {
  (void)dev7;  // address already selected in I2CDevice
  if (!g_vl53) return 1;
  return g_vl53->i2c_write_(reg, data, len) ? 0 : 1;
}

uint8_t VL53L1_I2CRead(uint16_t dev7, uint16_t reg, uint8_t *data, uint16_t len) {
  (void)dev7;
  if (!g_vl53) return 1;
  return g_vl53->i2c_read_(reg, data, len) ? 0 : 1;
}

} // extern "C"
