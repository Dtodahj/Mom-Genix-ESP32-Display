#pragma once
#include "layout.h"
#include "state_machine.h"
namespace momgenix {
class Canvas {
 public:
  virtual ~Canvas() = default;
  virtual void rect(int x, int y, int w, int h, uint16_t color) = 0;
};
class Renderer {
 public:
  explicit Renderer(Canvas& canvas) : canvas_(canvas) {}
  void update(const StateMachine& state, uint32_t now);
  bool layoutFits() const { return fits_; }
 private:
  void text(const char* value, int x, int y, int scale, uint16_t color);
  void centered(const char* value, int y, int scale, uint16_t color);
  void message(size_t index);
  Canvas& canvas_;
  bool initialized_ = false, fits_ = true;
  Mode mode_ = Mode::STARTUP;
  size_t index_ = kMessageCount;
  int barWidth_ = -1;
};
uint16_t severityColor(Severity severity);
}
