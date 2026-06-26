#pragma once
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace aw88298 {

// Power-managed AW88298 amp. Stays OFF at boot; power_on() gates the rail and
// runs M5's CoreS3 enable sequence just before a tone, power_off() drops it
// again afterwards — so the amp + boost don't drain the battery while idle.
class AW88298 : public Component, public i2c::I2CDevice {
 public:
  void set_sample_rate(uint32_t rate) { sample_rate_ = rate; }
  void set_enable_pin(GPIOPin *pin) { enable_pin_ = pin; }
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  // Bring the amp up for playback / drop it afterwards. Idempotent.
  void power_on();
  void power_off();

 protected:
  // 16-bit register write, big-endian on the wire (MSB first) as the chip
  // expects. Returns the I2C result so power_on() can tell if the amp ACKs.
  i2c::ErrorCode write_reg16_(uint8_t reg, uint16_t value);
  uint32_t sample_rate_{16000};
  GPIOPin *enable_pin_{nullptr};
  uint16_t reg06_{0x14C3};  // sample-rate register, computed in setup()
  bool powered_{false};
};

}  // namespace aw88298
}  // namespace esphome
