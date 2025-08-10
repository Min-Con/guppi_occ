import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID, CONF_ADDRESS, CONF_I2C_ID, CONF_UPDATE_INTERVAL,
    DEVICE_CLASS_DISTANCE, STATE_CLASS_MEASUREMENT, UNIT_MILLIMETER,
)

CODEOWNERS = ["@you"]
AUTO_LOAD = ["i2c"]

ns = cg.namespace("esphome").namespace("sparkfun_vl53l1x")
SparkfunVL53L1X = ns.class_("SparkfunVL53L1X", cg.PollingComponent, i2c.I2CDevice)

CONF_DISTANCE_MODE = "distance_mode"     # "short"|"medium"|"long"
CONF_TIMING_BUDGET = "timing_budget"     # ms
DISTANCE_MODES = {"short": 0, "medium": 1, "long": 2}

CONFIG_SCHEMA = sensor.sensor_schema(unit_of_measurement=UNIT_MILLIMETER) \
    .extend({
        cv.GenerateID(CONF_ID): cv.declare_id(SparkfunVL53L1X),
        cv.GenerateID(CONF_I2C_ID): cv.use_id(i2c.I2CComponent),
        cv.Optional(CONF_ADDRESS, default=0x29): cv.i2c_address,
        cv.Optional(CONF_UPDATE_INTERVAL, default="100ms"): cv.positive_time_period_milliseconds,
        cv.Optional(CONF_DISTANCE_MODE, default="long"): cv.one_of(*DISTANCE_MODES.keys(), lower=True),
        cv.Optional(CONF_TIMING_BUDGET, default="50ms"): cv.positive_time_period_milliseconds,
    })

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await i2c.register_i2c_device(var, config)

    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))

    sens = await sensor.new_sensor(config)
    cg.add(sens.set_device_class(DEVICE_CLASS_DISTANCE))
    cg.add(sens.set_state_class(STATE_CLASS_MEASUREMENT))
    cg.add(var.set_distance_sensor(sens))

    dm = config.get(CONF_DISTANCE_MODE, "long")
    cg.add(var.set_distance_mode(DISTANCE_MODES[dm]))
    tb = int(config.get(CONF_TIMING_BUDGET).total_milliseconds)
    cg.add(var.set_timing_budget_ms(tb))
