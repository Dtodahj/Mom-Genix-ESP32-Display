#pragma once
#include <stdint.h>
namespace momgenix {
constexpr uint32_t kStartupMs = 3000;
constexpr uint32_t kPressDebounceMs = 50;
constexpr uint32_t kHoldMs = 2000;
// Engineering defaults, NOT calibrated. Record replacements on physical board.
// XPT2046 v1.4 reports zero below its internal Z=400 cutoff.
constexpr int16_t kPressThreshold = 600;
constexpr int16_t kReleaseThreshold = 450;
constexpr uint32_t kReleaseDebounceMs = 50;
static_assert(kReleaseThreshold >= 400 && kReleaseThreshold < kPressThreshold,
              "Keep measurable hysteresis above the library cutoff");
}
