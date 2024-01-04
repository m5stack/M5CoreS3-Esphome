#include "i2s_audio_microphone.h"

// #ifdef USE_ESP32

// #include <driver/i2s.h>
#include <M5Unified.h>

#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace m5cores3_audio {

static const size_t BUFFER_SIZE = 512;

static const char *const TAG = "m5cores3.microphone";

void I2SAudioMicrophone::setup() {
//   ESP_LOGCONFIG(TAG, "Setting up I2S Audio Microphone...");
// #if SOC_I2S_SUPPORTS_ADC
//   if (this->adc_) {
//     if (this->parent_->get_port() != I2S_NUM_0) {
//       ESP_LOGE(TAG, "Internal ADC only works on I2S0!");
//       this->mark_failed();
//       return;
//     }
//   } else
// #endif
//       if (this->pdm_) {
//     if (this->parent_->get_port() != I2S_NUM_0) {
//       ESP_LOGE(TAG, "PDM only works on I2S0!");
//       this->mark_failed();
//       return;
//     }
//   }



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
  ESP_LOGI(TAG, "start mic");


  this->state_ = microphone::STATE_RUNNING;
  this->high_freq_.start();
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
  this->high_freq_.stop();
}

size_t I2SAudioMicrophone::read(int16_t *buf, size_t len) {
  // size_t bytes_read = 0;
  // esp_err_t err = i2s_read(this->parent_->get_port(), buf, len, &bytes_read, (100 / portTICK_PERIOD_MS));
  // if (err != ESP_OK) {
  //   ESP_LOGW(TAG, "Error reading from I2S microphone: %s", esp_err_to_name(err));
  //   this->status_set_warning();
  //   return 0;
  // }
  // if (bytes_read == 0) {
  //   this->status_set_warning();
  //   return 0;
  // }
  // this->status_clear_warning();
  // if (this->bits_per_sample_ == I2S_BITS_PER_SAMPLE_16BIT) {
  //   return bytes_read;
  // } else if (this->bits_per_sample_ == I2S_BITS_PER_SAMPLE_32BIT) {
  //   std::vector<int16_t> samples;
  //   size_t samples_read = bytes_read / sizeof(int32_t);
  //   samples.resize(samples_read);
  //   for (size_t i = 0; i < samples_read; i++) {
  //     int32_t temp = reinterpret_cast<int32_t *>(buf)[i] >> 14;
  //     samples[i] = clamp<int16_t>(temp, INT16_MIN, INT16_MAX);
  //   }
  //   memcpy(buf, samples.data(), samples_read * sizeof(int16_t));
  //   return samples_read * sizeof(int16_t);
  // } else {
  //   ESP_LOGE(TAG, "Unsupported bits per sample: %d", this->bits_per_sample_);
  //   return 0;
  // }


  // size_t bytes_read = 0;
  // esp_err_t err = i2s_read(this->parent_->get_port(), buf, len, &bytes_read, (100 / portTICK_PERIOD_MS));
  // if (err != ESP_OK) {
  //   ESP_LOGW(TAG, "Error reading from I2S microphone: %s", esp_err_to_name(err));
  //   this->status_set_warning();
  //   return 0;
  // }
  // if (bytes_read == 0) {
  //   this->status_set_warning();
  //   return 0;
  // }





  // // Record into buffer 
  // ESP_LOGI(TAG, "rec %d", BUFFER_SIZE);


  // M5.Mic.record(buf, len >> 1, 16000);
  M5.Mic.record(buf, 256, 16000);
  // M5.Mic.record(buf, 512, 16000);
  while (M5.Mic.isRecording());
  // delay(500);
  // ESP_LOGI(TAG, "done");

  this->status_clear_warning();

  // return len;
  // return 256;
  return 512;


  // this->status_set_warning();
  // return 0;





  // this->status_clear_warning();
  // if (this->bits_per_sample_ == I2S_BITS_PER_SAMPLE_16BIT) {
  //   return bytes_read;
  // } else if (this->bits_per_sample_ == I2S_BITS_PER_SAMPLE_32BIT) {
  //   std::vector<int16_t> samples;
  //   size_t samples_read = bytes_read / sizeof(int32_t);
  //   samples.resize(samples_read);
  //   for (size_t i = 0; i < samples_read; i++) {
  //     int32_t temp = reinterpret_cast<int32_t *>(buf)[i] >> 14;
  //     samples[i] = clamp<int16_t>(temp, INT16_MIN, INT16_MAX);
  //   }
  //   memcpy(buf, samples.data(), samples_read * sizeof(int16_t));
  //   return samples_read * sizeof(int16_t);
  // } else {
  //   ESP_LOGE(TAG, "Unsupported bits per sample: %d", this->bits_per_sample_);
  //   return 0;
  // }
}

void I2SAudioMicrophone::read_() {
  std::vector<int16_t> samples;
  samples.resize(BUFFER_SIZE);
  size_t bytes_read = this->read(samples.data(), BUFFER_SIZE / sizeof(int16_t));
  samples.resize(bytes_read / sizeof(int16_t));
  this->data_callbacks_.call(samples);


  // std::vector<int16_t> samples;
  // samples.resize(BUFFER_SIZE);

  // ESP_LOGI(TAG, "start record %d", BUFFER_SIZE);
  // M5.Mic.record(samples.data(), BUFFER_SIZE, 16000);
  // while (M5.Mic.isRecording());
  // ESP_LOGI(TAG, "done");

  // this->data_callbacks_.call(samples);
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
