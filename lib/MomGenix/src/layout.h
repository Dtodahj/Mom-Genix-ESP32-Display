#pragma once
#include <stddef.h>
#include <stdint.h>
namespace momgenix {
constexpr int kWidth = 320, kHeight = 240;
constexpr int kTextWidth = 296, kHeadingY = 24, kBodyY = 100;
constexpr int kHeadingScale = 4, kBodyScale = 3, kFallbackScale = 2;
constexpr int kHeadingLines = 2, kBodyHeight = 96;
constexpr int kFooterY = 210, kBarY = 234, kBarHeight = 4;
constexpr uint16_t kBlack = 0x0000, kWhite = 0xffff, kMuted = 0x8410;
struct Lines { char text[8][64] = {}; size_t count = 0; bool fits = true; };
// Converts UTF-8 ellipsis to our single-cell ellipsis glyph (0x7f).
Lines wrap(const char* text, size_t columns);
}
