#pragma once

// #ifdef USE_ESP32

// #include "../i2s_audio.h"
#include "../m5atoms3_audio.h"


#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#include "esphome/components/speaker/speaker.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace m5atoms3_audio {

static const size_t BUFFER_SIZE = 1024;

enum class TaskEventType : uint8_t {
  STARTING = 0,
  STARTED,
  PLAYING,
  STOPPING,
  STOPPED,
  WARNING = 255,
};

struct TaskEvent {
  TaskEventType type;
  esp_err_t err;
};

struct DataEvent {
  bool stop;
  size_t len;
  std::vector<uint8_t> data; // dynamic size
};

class I2SAudioSpeaker : public Component, public speaker::Speaker, public I2SAudioOut {
 public:
  float get_setup_priority() const override { return esphome::setup_priority::LATE; }

  void setup() override;
  void loop() override;

  void set_dma_buf_count(uint8_t count) { this->dma_buf_count_ = count; }
  void set_buffer_size(size_t rate) { this->buffer_size_ = rate; }
  void set_sample_rate(uint32_t rate) { this->sample_rate_ = rate; }
  void start() override;
  void stop() override;

  size_t play(const uint8_t *data, size_t length) override;

  bool has_buffered_data() const override;

 protected:
  void start_();
  // void stop_();
  void watch_();

  uint8_t dma_buf_count_ = 8;
  size_t buffer_size_ = 1024;
  uint32_t sample_rate_ = 16000;
  static void player_task(void *params);
  
  TaskHandle_t player_task_handle_{nullptr};
  QueueHandle_t buffer_queue_;
  QueueHandle_t event_queue_;

  
};

}  // namespace i2s_audio
}  // namespace esphome

// #endif  // USE_ESP32
