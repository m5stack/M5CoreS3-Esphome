import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, touchscreen
from esphome.const import CONF_ID
from .. import m5atoms3_ns

M5AtomS3TouchscreenButtonListener = m5atoms3_ns.class_("M5AtomS3TouchscreenButtonListener")
M5AtomS3Touchscreen = m5atoms3_ns.class_(
    "M5AtomS3Touchscreen",
    touchscreen.Touchscreen,
    cg.Component,
)

CONFIG_SCHEMA = touchscreen.TOUCHSCREEN_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(M5AtomS3Touchscreen),
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await touchscreen.register_touchscreen(var, config)
