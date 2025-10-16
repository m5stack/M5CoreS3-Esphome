#include "i2s_audio_speaker.h"

// #ifdef USE_ESP32


#include <M5Unified.h>

#include "esphome/core/application.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5atoms3_audio {

static const size_t BUFFER_COUNT = 20;

static const char *const TAG = "m5atoms3.speaker";

void I2SAudioSpeaker::setup() {
  // ESP_LOGCONFIG(TAG, "Setting up I2S Audio Speaker...");
  ESP_LOGI(TAG, "setup");
  auto cfg = M5.Speaker.config();
  cfg.task_priority = 15;
  M5.Speaker.config(cfg);
  M5.Speaker.begin();
  M5.Speaker.setVolume(200);

  // this->buffer_queue_ = xQueueCreate(BUFFER_COUNT, sizeof(DataEvent));
  // this->event_queue_ = xQueueCreate(BUFFER_COUNT, sizeof(TaskEvent));
}

void I2SAudioSpeaker::start() { this->state_ = speaker::STATE_STARTING; }
void I2SAudioSpeaker::start_() {
  // if (!this->parent_->try_lock()) {
  //   return;  // Waiting for another i2s component to return lock
  // }
  this->state_ = speaker::STATE_RUNNING;

  // xTaskCreate(I2SAudioSpeaker::player_task, "speaker_task", 8192, (void *) this, 1, &this->player_task_handle_);
}

void I2SAudioSpeaker::player_task(void *params) {
  I2SAudioSpeaker *this_speaker = (I2SAudioSpeaker *) params;

  TaskEvent event;
  event.type = TaskEventType::STARTING;
  xQueueSend(this_speaker->event_queue_, &event, portMAX_DELAY);

  auto cfg = M5.Speaker.config();
  cfg.dma_buf_count = this->dma_buf_count_;
  cfg.dma_buf_len = this->buffer_size_ / 2;
  cfg.task_priority = 15;
  M5.Speaker.config(cfg);
  M5.Mic.end();
  M5.Speaker.begin();
  ESP_LOGI(TAG, "spk start play");

  DataEvent data_event;
  data_event.data.resize(this->buffer_size_ / 2);

  event.type = TaskEventType::STARTED;
  xQueueSend(this_speaker->event_queue_, &event, portMAX_DELAY);

  int16_t buffer[this->buffer_size / 2];

  while (true) {
    if (xQueueReceive(this_speaker->buffer_queue_, &data_event, 100 / portTICK_PERIOD_MS) != pdTRUE) {
      break;
    }
    if (data_event.stop) {
      xQueueReset(this_speaker->buffer_queue_);
      break;
    }

    memmove(buffer, data_event.data, data_event.len);
    // clear the vector for reuse
    data_event.data.clear();
    // Play the buffer directly
    M5.Speaker.playRaw(buffer, data_event.len / sizeof(int16_t), this->sample_rate_);

    event.type = TaskEventType::PLAYING;
    xQueueSend(this_speaker->event_queue_, &event, portMAX_DELAY);
  }

  M5.Speaker.end();
  ESP_LOGI(TAG, "spk play end");

  event.type = TaskEventType::STOPPING;
  xQueueSend(this_speaker->event_queue_, &event, portMAX_DELAY);

  event.type = TaskEventType::STOPPED;
  xQueueSend(this_speaker->event_queue_, &event, portMAX_DELAY);

  while (true) {
    delay(10);
  }
}

void I2SAudioSpeaker::stop() {
  if (this->state_ == speaker::STATE_STOPPED)
    return;
  if (this->state_ == speaker::STATE_STARTING) {
    this->state_ = speaker::STATE_STOPPED;
    return;
  }
  this->state_ = speaker::STATE_STOPPING;
  // DataEvent data;
  // data.stop = true;
  // xQueueSendToFront(this->buffer_queue_, &data, portMAX_DELAY);
}

void I2SAudioSpeaker::watch_() {
 
  if (M5.Speaker.isPlaying())
  {
    this->status_clear_warning();
  }
  else 
  {
    this->state_ = speaker::STATE_STOPPED;

  }

}

void I2SAudioSpeaker::loop() {
  switch (this->state_) {
    case speaker::STATE_STARTING:
      this->start_();
      break;
    case speaker::STATE_RUNNING:
    case speaker::STATE_STOPPING:
      this->watch_();
      break;
    case speaker::STATE_STOPPED:
      break;
  }
}

size_t I2SAudioSpeaker::play(const uint8_t *data, size_t length) {
  
  if (this->state_ != speaker::STATE_RUNNING && this->state_ != speaker::STATE_STARTING) {
    this->start();

  }

  M5.Speaker.playRaw((int16_t*)data, length, this->sample_rate_);
  return length;
}

bool I2SAudioSpeaker::has_buffered_data() const 
{ 
  // return uxQueueMessagesWaiting(this->buffer_queue_) > 0;
  // return false;
  
  return M5.Speaker.isPlaying();
}

}  // namespace i2s_audio
}  // namespace esphome

// #endif  // USE_ESP32
