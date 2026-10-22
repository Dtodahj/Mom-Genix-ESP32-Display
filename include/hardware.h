#pragma once
#include <SPI.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "config.h"
namespace momgenix {
struct TouchSample { bool down; int16_t x; int16_t y; int16_t pressure; };
class Hardware {
 public:
  void begin();
  TouchSample sampleTouch();
  void setBrightness(uint8_t brightness);
  TFT_eSPI& display() { return display_; }
 private:
  TFT_eSPI display_;
  SPIClass touchBus_{VSPI};
  XPT2046_Touchscreen touch_{kTouchCs, kTouchIrq};
};
}
