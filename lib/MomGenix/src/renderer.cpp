#include "renderer.h"
#include "font_data.h"
#include <string.h>
namespace momgenix {
uint16_t severityColor(Severity severity) {
  switch (severity) {
    case Severity::WARNING: return 0xfd20; // amber
    case Severity::ERROR: case Severity::CRITICAL: case Severity::FATAL: return 0xf800;
    case Severity::INFO: return 0x07ff;
  }
  return kWhite;
}
void Renderer::text(const char* value, int x, int y, int scale, uint16_t color) {
  for (size_t i = 0; value[i]; ++i) {
    const uint8_t c = static_cast<uint8_t>(value[i]);
    for (int col = 0; col < 5; ++col) {
      const uint8_t bits = c == 0x7f ? ((col % 2 == 0) ? 0x40 : 0) : kFont5x7[c * 5 + col];
      for (int row = 0; row < 8; ++row)
        if (bits & (1 << row)) canvas_.rect(x + col*scale, y + row*scale, scale, scale, color);
    }
    x += 6 * scale;
  }
}
void Renderer::centered(const char* value, int y, int scale, uint16_t color) {
  text(value, (kWidth - static_cast<int>(strlen(value))*6*scale)/2, y, scale, color);
}
void Renderer::message(size_t index) {
  canvas_.rect(0, 20, kWidth, 184, kBlack);
  const Message& entry = kMessages[index];
  const Lines heading = wrap(entry.heading, kTextWidth / (6*kHeadingScale));
  int scale = kBodyScale;
  Lines body = wrap(entry.body, kTextWidth / (6*scale));
  if (!body.fits || static_cast<int>(body.count)*8*scale > kBodyHeight) {
    scale = kFallbackScale;
    body = wrap(entry.body, kTextWidth / (6*scale));
  }
  fits_ = heading.fits && heading.count <= kHeadingLines && body.fits &&
          static_cast<int>(body.count)*8*scale <= kBodyHeight;
  // Never silently clip new content. This is a visible authoring error.
  if (!fits_) { centered("LAYOUT ERROR", kBodyY, 2, 0xf800); return; }
  for (size_t i = 0; i < heading.count; ++i)
    centered(heading.text[i], kHeadingY + i*8*kHeadingScale, kHeadingScale, severityColor(entry.severity));
  for (size_t i = 0; i < body.count; ++i)
    centered(body.text[i], kBodyY + i*8*scale, scale, kWhite);
}
void Renderer::update(const StateMachine& state, uint32_t now) {
  const bool first = !initialized_;
  const bool leavingStartup = !first && mode_ == Mode::STARTUP && state.mode() != Mode::STARTUP;
  if (first || leavingStartup) {
    canvas_.rect(0, 0, kWidth, kHeight, kBlack);
    if (state.mode() == Mode::STARTUP) {
      centered("MOM-GENIX", 70, 4, kWhite);
      centered("DOMESTIC UNIT 1965", 126, 2, kWhite);
    } else centered("MOM-GENIX / DOMESTIC UNIT 1965", 6, 1, kMuted);
    initialized_ = true;
  }
  if (state.mode() != Mode::STARTUP) {
    if (first || leavingStartup || index_ != state.index()) message(state.index());
    if (first || leavingStartup || mode_ != state.mode()) {
      canvas_.rect(0, kFooterY, kWidth, 18, kBlack);
      centered(state.mode() == Mode::AUTO ? "AUTO" : "PAUSED", kFooterY, 2, kWhite);
    }
    const int width = static_cast<int>(state.progressMs(now)*kTextWidth / kMessages[state.index()].durationMs);
    if (first || leavingStartup || index_ != state.index() || width != barWidth_) {
      canvas_.rect(12, kBarY, kTextWidth, kBarHeight, 0x2104);
      if (width) canvas_.rect(12, kBarY, width, kBarHeight, severityColor(kMessages[state.index()].severity));
      barWidth_ = width;
    }
  }
  mode_ = state.mode();
  index_ = state.index();
}
}
