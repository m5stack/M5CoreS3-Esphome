import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID, CONF_MODE
from esphome.components import esp32, speaker

from .. import (
    CONF_I2S_AUDIO_ID,
    I2SAudioComponent,
    I2SAudioOut,
    i2s_audio_ns,
)

CONF_BUFFER_SIZE = "buffer_size"
CONF_DMA_BUF_COUNT = "buffer_count"

CODEOWNERS = ["@jesserockz"]
# DEPENDENCIES = ["i2s_audio"]
DEPENDENCIES = ["m5atoms3_audio"]

I2SAudioSpeaker = i2s_audio_ns.class_(
    "I2SAudioSpeaker", cg.Component, speaker.Speaker, I2SAudioOut
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(I2SAudioSpeaker),
        cv.GenerateID(CONF_I2S_AUDIO_ID): cv.use_id(I2SAudioComponent),
        cv.Optional(CONF_BUFFER_SIZE, default=1024): cv.int_range(min=256, max=2048),
        cv.Optional(CONF_DMA_BUF_COUNT, default=8): cv.int_range(min=2, max=16) 
    }
)


async def to_code(config):

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await speaker.register_speaker(var, config)
    await cg.register_parented(var, config[CONF_I2S_AUDIO_ID])

    if config[CONF_BUFFER_SIZE]:
        cg.add(var.set_buffer_size(config[CONF_BUFFER_SIZE]))
    if config[CONF_DMA_BUF_COUNT]:
        cg.add(var.set_buffer_size(config[CONF_DMA_BUF_COUNT]))
