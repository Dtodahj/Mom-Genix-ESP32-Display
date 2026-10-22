#include "state_machine.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <initializer_list>
using namespace momgenix;
static int checks = 0;
#define CHECK(expr) do { ++checks; if (!(expr)) { fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, #expr); exit(1); } } while (0)
static StateMachine ready(uint32_t base = 0) {
  StateMachine state(base); state.tick(base + kStartupMs, 0); return state;
}
static void release(StateMachine& s, uint32_t edge) {
  s.tick(edge, 0); s.tick(edge + kReleaseDebounceMs, 0);
}
static void pause(StateMachine& s, uint32_t edge) {
  s.tick(edge, 1000); s.tick(edge + 50, 1000); s.tick(edge + 2000, 1000);
  release(s, edge + 2100);
}
static void startup_auto_wrap() {
  StateMachine s;
  s.tick(2999, 1000); CHECK(s.mode() == Mode::STARTUP);
  s.tick(3000, 0); CHECK(s.mode() == Mode::AUTO); CHECK(s.index() == 0);
  for (size_t i = 1; i <= 10; ++i) {
    s.tick(3000 + i*7000 - 1, 0); CHECK(s.index() == (i-1)%10);
    s.tick(3000 + i*7000, 0); CHECK(s.index() == i%10);
  }
  StateMachine reset; CHECK(reset.mode() == Mode::STARTUP); CHECK(reset.index() == 0);
}
static void taps_and_paused() {
  auto s = ready(); s.tick(4000, 1000); s.tick(4050, 1000);
  s.tick(4100, 0); CHECK(s.index() == 0); // Await stable release.
  s.tick(4149, 0); CHECK(s.index() == 0);
  s.tick(4150, 0); CHECK(s.index() == 1); CHECK(s.mode() == Mode::AUTO);
  s.tick(11149, 0); CHECK(s.index() == 1);
  s.tick(11150, 0); CHECK(s.index() == 2);
  auto p = ready(); pause(p, 4000); CHECK(p.mode() == Mode::PAUSED);
  p.tick(30000, 0); CHECK(p.index() == 0);
  p.tick(31000, 1000); p.tick(31050, 1000); release(p, 31100);
  CHECK(p.index() == 1); CHECK(p.mode() == Mode::PAUSED);
  p.tick(60000, 0); CHECK(p.index() == 1);
}
static void noise_and_thresholds() {
  auto s = ready(); s.tick(9999, kPressThreshold); CHECK(!s.touching());
  s.tick(10000, kPressThreshold); CHECK(s.index() == 1); // Strictly exceeds.
  auto n = ready(); n.tick(9990, 1000); n.tick(10000, 0);
  n.tick(10049, 0); CHECK(n.touching()); CHECK(n.index() == 0);
  n.tick(10050, 0); CHECK(!n.touching()); CHECK(n.index() == 0);
  n.tick(17049, 0); CHECK(n.index() == 0);
  n.tick(17050, 0); CHECK(n.index() == 1);
  auto exactly = ready(); exactly.tick(4000, 1000); release(exactly, 4050);
  CHECK(exactly.index() == 1); // 50 ms accepted.
  auto brief = ready(); brief.tick(4000, 1000); release(brief, 4049);
  CHECK(brief.index() == 0); // 49 ms rejected, no tap.
  auto pressure = ready(); pressure.tick(4000, 1000); pressure.tick(4020, 500);
  pressure.tick(4100, 500); release(pressure, 4110);
  CHECK(pressure.index() == 0); // Not continuously above threshold for 50 ms.
  auto hysteresis = ready(); hysteresis.tick(4000, 1000); hysteresis.tick(4050, 1000);
  hysteresis.tick(4100, 500); CHECK(hysteresis.touching());
  release(hysteresis, 4200); CHECK(hysteresis.index() == 1);
}
static void hold_and_release() {
  auto s = ready(); s.tick(4000, 1000); s.tick(4050, 1000);
  s.tick(5999, 1000); CHECK(s.mode() == Mode::AUTO);
  s.tick(6000, 1000); CHECK(s.mode() == Mode::PAUSED); CHECK(s.index() == 0);
  s.tick(20000, 1000); CHECK(s.mode() == Mode::PAUSED); CHECK(s.index() == 0);
  release(s, 21000); CHECK(s.mode() == Mode::PAUSED); CHECK(s.index() == 0);
  s.tick(22000, 1000); s.tick(22050, 1000); s.tick(24000, 1000);
  CHECK(s.mode() == Mode::AUTO); CHECK(s.index() == 0);
  s.tick(40000, 1000); CHECK(s.index() == 0); // Resume still held: frozen.
  release(s, 41000); CHECK(s.index() == 0);
  s.tick(48049, 0); CHECK(s.index() == 0);
  s.tick(48050, 0); CHECK(s.index() == 1);
}
static void exact_release_boundaries() {
  for (uint32_t duration : {1999u, 2000u, 2001u}) {
    auto s = ready(); s.tick(4000, 1000); s.tick(4050, 1000);
    release(s, 4000 + duration);
    CHECK(s.index() == (duration < 2000 ? 1u : 0u));
    CHECK(s.mode() == (duration < 2000 ? Mode::AUTO : Mode::PAUSED));
  }
}
static void expiry_and_freeze() {
  auto s = ready(); s.tick(10000, 1000); CHECK(s.index() == 0);
  CHECK(s.touching()); CHECK(s.progressMs(11000) == 7000);
  s.tick(10050, 1000); release(s, 10100); CHECK(s.index() == 1);
  s.tick(17149, 0); CHECK(s.index() == 1);
  s.tick(17150, 0); CHECK(s.index() == 2);
  auto h = ready(); h.tick(10000, 1000); h.tick(10050, 1000);
  h.tick(12000, 1000); CHECK(h.mode() == Mode::PAUSED); CHECK(h.index() == 0);
  auto p = ready(); p.tick(4000, 1000); CHECK(p.progressMs(5999) == 1000);
}
static void stable_release_and_bounce() {
  auto s = ready(); s.tick(4000, 1000); s.tick(4050, 1000);
  s.tick(4100, 0); s.tick(4149, 500); CHECK(s.index() == 0);
  s.tick(4200, 0); s.tick(4249, 0); CHECK(s.index() == 0);
  s.tick(4250, 0); CHECK(s.index() == 1);
  auto h = ready(); h.tick(4000, 1000); h.tick(4050, 1000);
  h.tick(6000, 1000); h.tick(6100, 0); h.tick(6120, 1000);
  CHECK(h.mode() == Mode::PAUSED); CHECK(h.index() == 0);
  release(h, 6200); CHECK(h.index() == 0); CHECK(h.mode() == Mode::PAUSED);
  // Release bounce that straddles 2s does not add release debounce to a tap.
  auto t = ready(); t.tick(4000, 1000); t.tick(4050, 1000);
  release(t, 5999); CHECK(t.mode() == Mode::AUTO); CHECK(t.index() == 1);
}
static void rollover() {
  StateMachine startup(0xfffffff0u);
  startup.tick(uint32_t(0xfffffff0u + 2999u), 0); CHECK(startup.mode() == Mode::STARTUP);
  startup.tick(uint32_t(0xfffffff0u + 3000u), 0); CHECK(startup.mode() == Mode::AUTO);
  const uint32_t base = 0xfffff000u;
  auto s = ready(base); const uint32_t autoAt = base + 3000u;
  s.tick(autoAt + 6999u, 0); CHECK(s.index() == 0);
  s.tick(autoAt + 7000u, 0); CHECK(s.index() == 1);
  auto h = ready(0xffffe000u); const uint32_t down = 0xfffffff0u;
  h.tick(down, 1000); h.tick(down + 50u, 1000);
  h.tick(down + 1999u, 1000); CHECK(h.mode() == Mode::AUTO);
  h.tick(down + 2000u, 1000); CHECK(h.mode() == Mode::PAUSED);
  release(h, down + 2100u); CHECK(h.index() == 0);
  auto r = ready(0xffffe000u); r.tick(0xffffff80u, 1000); r.tick(0xffffffb2u, 1000);
  release(r, 0xfffffff0u); CHECK(r.index() == 1); CHECK(!r.touching());
}
struct CheckedCanvas : Canvas {
  int calls = 0, full = 0;
  void rect(int x, int y, int w, int h, uint16_t) override {
    CHECK(x >= 0 && y >= 0 && w >= 0 && h >= 0 && x+w <= kWidth && y+h <= kHeight);
    ++calls; if (w == kWidth && h == kHeight) ++full;
  }
};
static void layout_and_dirty_regions() {
  CheckedCanvas c; Renderer r(c); StateMachine s;
  r.update(s, 0); CHECK(c.full == 1);
  int before = c.calls; r.update(s, 1000); CHECK(c.calls == before);
  s.tick(3000, 0); r.update(s, 3000); CHECK(c.full == 2);
  for (size_t i = 0; i < kMessageCount; ++i) {
    const uint32_t now = 3000 + static_cast<uint32_t>(i)*7000;
    s.tick(now, 0); r.update(s, now); CHECK(r.layoutFits());
    before = c.calls; r.update(s, now); CHECK(c.calls == before);
  }
  CHECK(c.full == 2); // No full-screen clear on message changes.
  before = c.calls; r.update(s, 66100); CHECK(c.calls <= before + 2); // Bar only.
  s.tick(66200, 1000); s.tick(66250, 1000); s.tick(68200, 1000);
  before = c.calls; r.update(s, 68200); CHECK(c.calls > before); CHECK(c.full == 2);
  CHECK(kMessages[8].severity == Severity::INFO);
  CHECK(strstr(kMessages[7].body, "\xE2\x80\xA6") != nullptr);
}
int main() {
  struct Test { const char* name; void (*run)(); } tests[] = {
    {"startup, 70-second cycle and wrap", startup_auto_wrap},
    {"tap AUTO and PAUSED", taps_and_paused},
    {"noise, strict pressure and hysteresis", noise_and_thresholds},
    {"2000ms hold while down; release suppression; fresh resume", hold_and_release},
    {"1999/2000/2001ms release boundaries", exact_release_boundaries},
    {"touch at deadline and progress freeze", expiry_and_freeze},
    {"stable release and bounce", stable_release_and_bounce},
    {"startup, cycle, hold and release rollover", rollover},
    {"all message layouts and regional drawing", layout_and_dirty_regions},
  };
  for (const auto& test : tests) { test.run(); printf("PASS %s\n", test.name); }
  printf("%zu groups passed; %d checks (including drawing bounds)\n", sizeof(tests)/sizeof(tests[0]), checks);
}
