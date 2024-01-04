// #include "i2s_audio.h"
#include "m5cores3_audio.h"

// #ifdef USE_ESP32

#include "esphome/core/log.h"

namespace esphome {
namespace m5cores3_audio {

static const char *const TAG = "m5cores3.audio";

void M5CoreS3AudioComponent::setup() {
  // static i2s_port_t next_port_num = I2S_NUM_0;

  // if (next_port_num >= I2S_NUM_MAX) {
  //   ESP_LOGE(TAG, "Too many I2S Audio components!");
  //   this->mark_failed();
  //   return;
  // }

  // this->port_ = next_port_num;
  // next_port_num = (i2s_port_t) (next_port_num + 1);

  ESP_LOGI(TAG, "setup");
}

}  // namespace i2s_audio
}  // namespace esphome

// #endif  // USE_ESP32
