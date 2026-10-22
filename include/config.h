#pragma once
#include <stdint.h>
#include "timing_config.h"
namespace momgenix {
constexpr uint8_t kBacklightPin = 21;
constexpr uint8_t kTouchClock = 25;
constexpr uint8_t kTouchMiso = 39;
constexpr uint8_t kTouchMosi = 32;
constexpr uint8_t kTouchCs = 33;
constexpr uint8_t kTouchIrq = 36;
constexpr bool kAmbientBrightnessEnabled = false;
constexpr bool kRgbIndicatorEnabled = false;
static_assert(!kAmbientBrightnessEnabled, "Tier 1B ambient sensing is deferred; keep disabled");
static_assert(!kRgbIndicatorEnabled, "Tier 1B RGB behavior is deferred; keep disabled");
constexpr uint8_t kRgbRed = 4, kRgbGreen = 16, kRgbBlue = 17;
// v1.3 proposed 60% fixed fallback, adjustable after mounted/power testing.
constexpr uint8_t kFixedBrightness = 153;
constexpr uint8_t kBacklightChannel = 0;
constexpr uint32_t kDiagnosticSampleMs = 100;
constexpr uint32_t kTouchSampleMs = 5;
constexpr bool kTouchCalibrated = false;
}
