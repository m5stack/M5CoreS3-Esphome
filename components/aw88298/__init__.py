"""AW88298 I2S Class-D speaker amplifier (M5Stack CoreS3).

ESPHome has no built-in driver for the AW88298. This component is power-managed:
the amp stays off at boot, and power_on()/power_off() gate the rail and run the
CoreS3 enable / un-mute register sequence (the same one M5Unified uses) around
playback, so the amp and boost don't drain the battery while idle. Audio data is
fed separately via the i2s_audio speaker; this only manages the amplifier.

See README.md for usage.
"""

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import i2c
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]

CONF_SAMPLE_RATE = "sample_rate"
CONF_ENABLE_PIN = "enable_pin"

aw88298_ns = cg.esphome_ns.namespace("aw88298")
AW88298 = aw88298_ns.class_("AW88298", cg.Component, i2c.I2CDevice)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(AW88298),
            # Must match the i2s_audio speaker's sample_rate so the amp's I2S
            # clocking lines up; used to compute register 0x06.
            cv.Optional(CONF_SAMPLE_RATE, default=16000): cv.int_range(min=8000, max=96000),
            # Power-gate for the amp rail (CoreS3 = AW9523 P0_2). Driven high in
            # setup() before any I2C to the amp, or it won't ACK at 0x36.
            cv.Optional(CONF_ENABLE_PIN): pins.gpio_output_pin_schema,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x36))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))
    if CONF_ENABLE_PIN in config:
        enable = await cg.gpio_pin_expression(config[CONF_ENABLE_PIN])
        cg.add(var.set_enable_pin(enable))
