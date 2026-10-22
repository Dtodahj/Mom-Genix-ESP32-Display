#pragma once
#include "messages.h"
#include "timing_config.h"
namespace momgenix {
enum class Mode : uint8_t { STARTUP, AUTO, PAUSED };
class StateMachine {
 public:
  explicit StateMachine(uint32_t now = 0) : startupAt_(now), cycleAt_(now) {}
  void tick(uint32_t now, int16_t pressure);
  Mode mode() const { return mode_; }
  size_t index() const { return index_; }
  bool touching() const { return tracking_; }
  uint32_t progressMs(uint32_t now) const;
 private:
  void advance(uint32_t now);
  void toggleHold();
  Mode mode_ = Mode::STARTUP;
  size_t index_ = 0;
  uint32_t startupAt_, cycleAt_;
  uint32_t downAt_ = 0, qualifyAt_ = 0, releaseAt_ = 0, frozenMs_ = 0;
  bool tracking_ = false, valid_ = false, held_ = false;
  bool qualifying_ = false, releasing_ = false;
};
}
