#include <Arduino.h>
#include "hardware.h"
#include <WiFi.h>
#include "renderer.h"
#ifdef MOM_GENIX_DIAGNOSTIC
#include "diagnostic_renderer.h"
#endif
momgenix::Hardware hardware;
class TftCanvas final : public momgenix::Canvas {
 public:
  void rect(int x, int y, int w, int h, uint16_t color) override {
    hardware.display().fillRect(x, y, w, h, color);
  }
};
TftCanvas canvas;
momgenix::Renderer renderer(canvas);
momgenix::StateMachine state;
uint32_t lastSample = 0;
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);
  btStop();
  hardware.begin();
  Serial.println("MOM-GENIX Tier 1A; touch thresholds UNCALIBRATED until physical bring-up");
#ifdef MOM_GENIX_DIAGNOSTIC
  momgenix::drawDiagnostic(hardware.display());
#else
  state = momgenix::StateMachine(millis());
  renderer.update(state, millis());
#endif
  hardware.setBrightness(momgenix::kFixedBrightness);
#ifndef MOM_GENIX_DIAGNOSTIC
  // Start the three seconds when the complete identity screen becomes visible.
  state = momgenix::StateMachine(millis());
#endif
}
void loop() {
  uint32_t now = millis();
  if (uint32_t(now - lastSample) < momgenix::kTouchSampleMs) return;
  lastSample = now;
  const auto sample = hardware.sampleTouch();
  // Timestamp after the SPI transaction; process input before timer expiry.
  now = millis();
#ifdef MOM_GENIX_DIAGNOSTIC
  static uint32_t loggedAt = now - momgenix::kDiagnosticSampleMs;
  if (uint32_t(now - loggedAt) >= momgenix::kDiagnosticSampleMs) {
    loggedAt = now;
    Serial.printf("down=%d x=%d y=%d z=%d\n", sample.down, sample.x, sample.y, sample.pressure);
    momgenix::drawTouchDiagnostic(hardware.display(), sample);
  }
#else
  state.tick(now, sample.pressure);
  renderer.update(state, now);
#endif
}
