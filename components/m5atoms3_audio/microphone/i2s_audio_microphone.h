#pragma once

// #ifdef USE_ESP32

#include "../m5atoms3_audio.h"

#include "esphome/components/microphone/microphone.h"
#include "esphome/core/component.h"

namespace esphome {
namespace m5atoms3_audio {

class I2SAudioMicrophone : public I2SAudioIn, public microphone::Microphone, public Component {
 public:
  void setup() override;
  void start() override;
  void stop() override;

  void loop() override;

  size_t read(int16_t *buf, size_t len);

 protected:
  void start_();
  void stop_();
  void read_();

};

}  // namespace i2s_audio
}  // namespace esphome

// #endif  // USE_ESP32
