#include "state_machine.h"
namespace momgenix {
void StateMachine::advance(uint32_t now) {
  index_ = (index_ + 1) % kMessageCount;
  cycleAt_ = now;
  frozenMs_ = 0;
}
void StateMachine::toggleHold() {
  mode_ = mode_ == Mode::AUTO ? Mode::PAUSED : Mode::AUTO;
  held_ = true;
}
uint32_t StateMachine::progressMs(uint32_t now) const {
  if (mode_ == Mode::STARTUP) return 0;
  const uint32_t duration = kMessages[index_].durationMs;
  const uint32_t elapsed = tracking_ || mode_ == Mode::PAUSED ? frozenMs_ : uint32_t(now - cycleAt_);
  return elapsed < duration ? elapsed : duration;
}
void StateMachine::tick(uint32_t now, int16_t pressure) {
  if (mode_ == Mode::STARTUP) {
    if (uint32_t(now - startupAt_) < kStartupMs) return;
    mode_ = Mode::AUTO;
    cycleAt_ = now;
  }
  // Touch has priority even at exactly the automatic deadline.
  if (!tracking_ && pressure > kPressThreshold) {
    frozenMs_ = progressMs(now);
    tracking_ = true;
    valid_ = held_ = releasing_ = false;
    qualifying_ = true;
    downAt_ = qualifyAt_ = now;
  }
  if (tracking_) {
    if (pressure <= kReleaseThreshold) {
      if (!releasing_) {
        releasing_ = true;
        releaseAt_ = now;
        // A release edge closes the preceding sampled high-pressure interval.
        if (qualifying_ && uint32_t(now - qualifyAt_) >= kPressDebounceMs) valid_ = true;
        if (valid_ && !held_ && uint32_t(now - downAt_) >= kHoldMs) toggleHold();
      }
      if (uint32_t(now - releaseAt_) >= kReleaseDebounceMs) {
        // Release debounce must not turn a 1999 ms tap into a 2000 ms hold.
        if (valid_ && !held_) advance(now);
        tracking_ = qualifying_ = releasing_ = false;
        // Both noise and any completed gesture restart AUTO's full interval.
        if (mode_ == Mode::AUTO) { cycleAt_ = now; frozenMs_ = 0; }
      }
      return; // Timer remains frozen through release qualification.
    }
    if (releasing_) {
      releasing_ = false; // Contact bounce: same gesture, never rearm here.
      if (!valid_) { qualifying_ = false; }
    }
    if (!valid_) {
      if (pressure > kPressThreshold) {
        if (!qualifying_) { qualifying_ = true; qualifyAt_ = now; }
        if (uint32_t(now - qualifyAt_) >= kPressDebounceMs) valid_ = true;
      } else {
        qualifying_ = false; // Must exceed press threshold continuously for 50 ms.
      }
    }
    if (valid_ && !held_ && uint32_t(now - downAt_) >= kHoldMs) toggleHold();
    return;
  }
  if (mode_ == Mode::AUTO && uint32_t(now - cycleAt_) >= kMessages[index_].durationMs)
    advance(now);
}
}
