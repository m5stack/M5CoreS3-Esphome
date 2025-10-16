#pragma once

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace m5atoms3_audio {

class M5AtomS3AudioComponent;

class I2SAudioIn : public Parented<M5AtomS3AudioComponent> {};

class I2SAudioOut : public Parented<M5AtomS3AudioComponent> {};

class M5AtomS3AudioComponent : public Component {
 public:
  void setup() override;

  void lock() { this->lock_.lock(); }
  bool try_lock() { return this->lock_.try_lock(); }
  void unlock() { this->lock_.unlock(); }

 protected:
  Mutex lock_;

};

}  // namespace m5atoms3_audio
}  // namespace esphome
