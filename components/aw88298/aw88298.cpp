#include "aw88298.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aw88298 {

static const char *const TAG = "aw88298";

i2c::ErrorCode AW88298::write_reg16_(uint8_t reg, uint16_t value) {
  // Chip expects big-endian 16-bit values: [reg][hi][lo].
  uint8_t buf[2] = {static_cast<uint8_t>(value >> 8), static_cast<uint8_t>(value & 0xFF)};
  return this->write_register(reg, buf, 2);
}

void AW88298::setup() {
  // Register 0x06 selects the I2S sample rate. Same mapping M5Unified uses on
  // the CoreS3: index a rate table by (rate + 1102) / 2205, OR'd with the
  // 16-bit*2 BCK base (0x14C0). e.g. 16 kHz -> index 3 -> 0x14C3. Computed once.
  static const uint8_t rate_tbl[] = {4, 5, 6, 8, 10, 11, 15, 20, 22, 44};
  uint32_t rate = (sample_rate_ + 1102) / 2205;
  size_t idx = 0;
  while (idx < sizeof(rate_tbl) && rate > rate_tbl[idx])
    idx++;
  if (idx >= sizeof(rate_tbl))
    idx = sizeof(rate_tbl) - 1;
  reg06_ = 0x14C0 | static_cast<uint16_t>(idx);

  // Leave the amp OFF at boot. The rail (CoreS3: AW9523 P0_2) + boost draw
  // current continuously when on, so power_on() brings it up only for playback
  // and power_off() drops it again (call them from your media-player automations).
  if (this->enable_pin_ != nullptr) {
    this->enable_pin_->setup();
    this->enable_pin_->digital_write(false);
  }
  powered_ = false;
  ESP_LOGCONFIG(TAG, "AW88298 ready (power-managed; sample_rate=%u, reg0x06=0x%04X)",
                static_cast<unsigned>(sample_rate_), reg06_);
}

void AW88298::power_on() {
  if (powered_)
    return;
  // Raise the power-gate, let the rail settle, then run M5's enable sequence.
  if (this->enable_pin_ != nullptr) {
    this->enable_pin_->digital_write(true);
    delay(10);
  }
  i2c::ErrorCode err = this->write_reg16_(0x61, 0x0673);  // boost mode
  if (err != i2c::ERROR_OK) {
    ESP_LOGW(TAG, "AW88298 power_on: no ACK at 0x%02X (err %d)", this->address_, err);
    if (this->enable_pin_ != nullptr)
      this->enable_pin_->digital_write(false);
    return;  // stays powered_=false; retried on the next tone
  }
  this->write_reg16_(0x04, 0x4040);  // I2SEN=1, AMPPD=0, PWDN=0 (un-gate amp)
  this->write_reg16_(0x05, 0x0008);  // RMSE/HAGCE/HDCCE/HMUTE = 0
  this->write_reg16_(0x06, reg06_);  // I2S sample-rate config
  this->write_reg16_(0x0C, 0x0064);  // volume (full)
  powered_ = true;
  ESP_LOGD(TAG, "AW88298 powered on");
}

void AW88298::power_off() {
  if (this->enable_pin_ != nullptr)
    this->enable_pin_->digital_write(false);  // cut the rail (amp + boost off)
  powered_ = false;
  ESP_LOGD(TAG, "AW88298 powered off");
}

void AW88298::dump_config() {
  ESP_LOGCONFIG(TAG, "AW88298 speaker amp (power-managed):");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  Sample rate: %u Hz", static_cast<unsigned>(sample_rate_));
}

}  // namespace aw88298
}  // namespace esphome
