#pragma once

#include "esphome/components/touchscreen/touchscreen.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <M5Unified.h>

namespace esphome {
namespace m5cores3 {

static const char *const TAG = "m5cores3.touchscreen";

enum VendorId {
  ID_UNKNOWN = 0,
};

class M5CoreS3Touchscreen : public touchscreen::Touchscreen {
 public:
  void setup() override {
    esph_log_config(TAG, "Setting up M5CoreS3 Touchscreen...");
    this->x_raw_max_ = M5.Display.width();
    this->y_raw_max_ = M5.Display.height();
    esph_log_config(TAG, "M5CoreS3 Touchscreen setup complete");
  }

  void update_touches() override {
    M5.update();

    auto t = M5.Touch.getDetail();

    if (t.state == 3) {  // Checking for "touch_begin"
        ESP_LOGD("Touch", "X: %d, Y: %d", t.x, t.y);
        this->add_raw_touch_position_(1, t.x, t.y);
    }
  }

  void dump_config() override {
    esph_log_config(TAG, "M5CoreS3 Touchscreen:");
  }

 protected:
  VendorId vendor_id_{ID_UNKNOWN};
};

}  // namespace m5cores3
}  // namespace esphome
