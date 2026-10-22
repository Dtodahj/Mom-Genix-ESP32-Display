#include "hardware.h"
#ifdef TFT_BL
#error "TFT_eSPI must not own the backlight"
#endif
#ifdef TOUCH_CS
#error "Touch must use its dedicated VSPI bus"
#endif
namespace momgenix {
void Hardware::begin() {
  // Reference onboard RGB is active-low. Disabled means electrically off.
  digitalWrite(kRgbRed, HIGH); pinMode(kRgbRed, OUTPUT);
  digitalWrite(kRgbGreen, HIGH); pinMode(kRgbGreen, OUTPUT);
  digitalWrite(kRgbBlue, HIGH); pinMode(kRgbBlue, OUTPUT);
  pinMode(kBacklightPin, OUTPUT);
  digitalWrite(kBacklightPin, LOW);
  ledcSetup(kBacklightChannel, 5000, 8);
  ledcAttachPin(kBacklightPin, kBacklightChannel);
  setBrightness(0);
  display_.init();
  display_.setRotation(1);
  display_.fillScreen(TFT_BLACK);
  touchBus_.begin(kTouchClock, kTouchMiso, kTouchMosi, kTouchCs);
  touch_.begin(touchBus_);
  touch_.setRotation(1);
}
void Hardware::setBrightness(uint8_t brightness) {
  ledcWrite(kBacklightChannel, brightness);
}
TouchSample Hardware::sampleTouch() {
  const auto point = touch_.getPoint();
  return {point.z > kPressThreshold, point.x, point.y, point.z};
}
}
