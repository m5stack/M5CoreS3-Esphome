#include "esphome/core/log.h"
#include "board_m5cores3.h"

namespace esphome {
namespace board_m5cores3 {

static const char *TAG = "board.m5cores3";

void BoardM5CoreS3::setup() {
    ESP_LOGI(TAG, "setup");
    M5.begin();
    // M5.Display.fillScreen(TFT_BLUE);
    // M5.Mic.end();
    // M5.Speaker.end();
}

void BoardM5CoreS3::loop() {

}

void BoardM5CoreS3::dump_config(){
    ESP_LOGCONFIG(TAG, "config");
}


}  // namespace empty_component
}  // namespace esphome