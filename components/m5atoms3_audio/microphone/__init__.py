import esphome.config_validation as cv
import esphome.codegen as cg

from esphome import pins
from esphome.const import CONF_CHANNEL, CONF_ID, CONF_NUMBER
from esphome.components import microphone, esp32
from esphome.components.adc import ESP32_VARIANT_ADC1_PIN_TO_CHANNEL, validate_adc_pin

from .. import (
    i2s_audio_ns,
    I2SAudioComponent,
    I2SAudioIn,
    CONF_I2S_AUDIO_ID,
    CONF_I2S_DIN_PIN,
)

CODEOWNERS = ["@jesserockz"]
# DEPENDENCIES = ["i2s_audio"]
DEPENDENCIES = ["m5atoms3_audio"]

I2SAudioMicrophone = i2s_audio_ns.class_(
    "I2SAudioMicrophone", I2SAudioIn, microphone.Microphone, cg.Component
)

i2s_channel_fmt_t = cg.global_ns.enum("i2s_channel_fmt_t")
CHANNELS = {
    "left": i2s_channel_fmt_t.I2S_CHANNEL_FMT_ONLY_LEFT,
    "right": i2s_channel_fmt_t.I2S_CHANNEL_FMT_ONLY_RIGHT,
}
i2s_bits_per_sample_t = cg.global_ns.enum("i2s_bits_per_sample_t")
BITS_PER_SAMPLE = {
    16: i2s_bits_per_sample_t.I2S_BITS_PER_SAMPLE_16BIT,
    32: i2s_bits_per_sample_t.I2S_BITS_PER_SAMPLE_32BIT,
}

_validate_bits = cv.float_with_unit("bits", "bit")


CONFIG_SCHEMA = microphone.MICROPHONE_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(I2SAudioMicrophone),
        cv.GenerateID(CONF_I2S_AUDIO_ID): cv.use_id(I2SAudioComponent),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await cg.register_parented(var, config[CONF_I2S_AUDIO_ID])

    await microphone.register_microphone(var, config)
