#include "i2s_audio_microphone.h"

// #ifdef USE_ESP32

#include <M5Unified.h>

#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5atoms3_audio {

static const size_t BUFFER_SIZE = 512;

static const char *const TAG = "m5atoms3.microphone";

void I2SAudioMicrophone::setup() {

  ESP_LOGI(TAG, "setup");

}

void I2SAudioMicrophone::start() {
  if (this->is_failed())
    return;
  if (this->state_ == microphone::STATE_RUNNING)
    return;  // Already running
  this->state_ = microphone::STATE_STARTING;
}
void I2SAudioMicrophone::start_() {
  if (!this->parent_->try_lock()) {
    return;  // Waiting for another i2s to return lock
  }
//   i2s_driver_config_t config = {
//       .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_RX),
//       .sample_rate = 16000,
//       .bits_per_sample = this->bits_per_sample_,
//       .channel_format = this->channel_,
//       .communication_format = I2S_COMM_FORMAT_STAND_I2S,
//       .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
//       .dma_buf_count = 4,
//       .dma_buf_len = 256,
//       .use_apll = false,
//       .tx_desc_auto_clear = false,
//       .fixed_mclk = 0,
//       .mclk_multiple = I2S_MCLK_MULTIPLE_DEFAULT,
//       .bits_per_chan = I2S_BITS_PER_CHAN_DEFAULT,
//   };

// #if SOC_I2S_SUPPORTS_ADC
//   if (this->adc_) {
//     config.mode = (i2s_mode_t) (config.mode | I2S_MODE_ADC_BUILT_IN);
//     i2s_driver_install(this->parent_->get_port(), &config, 0, nullptr);

//     i2s_set_adc_mode(ADC_UNIT_1, this->adc_channel_);
//     i2s_adc_enable(this->parent_->get_port());
//   } else
// #endif
//   {
//     if (this->pdm_)
//       config.mode = (i2s_mode_t) (config.mode | I2S_MODE_PDM);

//     i2s_driver_install(this->parent_->get_port(), &config, 0, nullptr);

//     i2s_pin_config_t pin_config = this->parent_->get_pin_config();
//     pin_config.data_in_num = this->din_pin_;

//     i2s_set_pin(this->parent_->get_port(), &pin_config);
//   }


  // Start mic 
  // auto cfg = M5.Mic.config();
  // cfg.sample_rate = 16000;
  // cfg.dma_buf_count = 8;
  // cfg.dma_buf_len = 512;
  // cfg.task_priority = 15;
  // cfg.stereo = true;
  // M5.Mic.config(cfg);

  M5.Speaker.end();
  M5.Mic.begin();
  auto cfg = M5.Mic.config();
  cfg.task_priority = 15;
  M5.Mic.config(cfg);
  ESP_LOGI(TAG, "start mic");


  this->state_ = microphone::STATE_RUNNING;

}

void I2SAudioMicrophone::stop() {
  if (this->state_ == microphone::STATE_STOPPED || this->is_failed())
    return;
  if (this->state_ == microphone::STATE_STARTING) {
    this->state_ = microphone::STATE_STOPPED;
    return;
  }
  this->state_ = microphone::STATE_STOPPING;
}

void I2SAudioMicrophone::stop_() {
  // i2s_stop(this->parent_->get_port());
  // i2s_driver_uninstall(this->parent_->get_port());


  // Stop mic 
  M5.Mic.end();
  M5.Speaker.begin();
  ESP_LOGI(TAG, "mic end");


  this->parent_->unlock();
  this->state_ = microphone::STATE_STOPPED;

}


size_t I2SAudioMicrophone::read(int16_t *buf, size_t len) {
  
  M5.Mic.record(buf, 256, this->parent_ -> get_sample_rate());
  // M5.Mic.record(buf, 512, 16000);
  while (M5.Mic.isRecording());
  // delay(500);
  // ESP_LOGI(TAG, "done");

  this->status_clear_warning();

  // return len;
  // return 256;
  return 512;



}

void I2SAudioMicrophone::read_() {
  std::vector<int16_t> samples;
  samples.resize(BUFFER_SIZE);
  size_t bytes_read = this->read(samples.data(), BUFFER_SIZE / sizeof(int16_t));
  
  std::vector<uint8_t> byte_samples(reinterpret_cast<uint8_t*>(samples.data()), reinterpret_cast<uint8_t*>(samples.data()) + samples.size() * sizeof(int16_t));
  this->data_callbacks_.call(byte_samples);


}

void I2SAudioMicrophone::loop() {
  switch (this->state_) {
    case microphone::STATE_STOPPED:
      break;
    case microphone::STATE_STARTING:
      this->start_();
      break;
    case microphone::STATE_RUNNING:
      if (this->data_callbacks_.size() > 0) {
        this->read_();
      }
      break;
    case microphone::STATE_STOPPING:
      this->stop_();
      break;
  }
}

}  // namespace i2s_audio
}  // namespace esphome

// #endif  // USE_ESP32
