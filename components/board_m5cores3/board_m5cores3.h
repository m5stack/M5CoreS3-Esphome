// Ref: https://github.com/thegroove/esphome-custom-component-examples.git

#pragma once

#include <M5Unified.h>
#include "esphome/core/component.h"


namespace esphome {
namespace board_m5cores3 {

class BoardM5CoreS3 : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
};


}  // namespace empty_component
}  // namespace esphome