// #include "i2s_audio.h"
#include "m5atoms3_audio.h"

// #ifdef USE_ESP32

#include "esphome/core/log.h"

namespace esphome {
namespace m5atoms3_audio {

static const char *const TAG = "m5atoms3.audio";

void M5AtomS3AudioComponent::setup() {
  ESP_LOGI(TAG, "setup");
}

}  // namespace i2s_audio
}  // namespace esphome

// #endif  // USE_ESP32
