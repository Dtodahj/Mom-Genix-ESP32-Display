# MOM-GENIX review packet

Milestone: Tier 1A software implemented, 2026-10-22. Both board profiles compile;
actual portable state-machine tests pass; previews of all ten messages generated.
**No physical validation, calibration, power-bank runtime, upload or hardware-tested
release is claimed.** The specification's physical acceptance gate remains open.

## What changed and why

Replaced the earlier diagnostic-only default with the three-second identity screen,
ten exact v1.3 entries with explicit severities, seven-second automatic cycle,
release-qualified taps, 2000ms holds, timer freezing, stable release hysteresis and
visible AUTO/PAUSED status. Hold duration starts at candidate onset, not after 50ms.
Touch wins a simultaneous cycle deadline. Rejected noise and AUTO hold releases
restart a full interval after stable release. Short taps remain PAUSED if paused.
The 1999ms release boundary excludes release debounce from hold classification.

Kept dedicated touch VSPI, display HSPI, pinned board profiles and exclusive
application backlight control. Radios are off. Fixed duty is 153/255. Onboard RGB
is electrically off; neither Tier 1B behavior was attempted. Diagnostic builds are
retained for arrival, with serial raw readings at 10Hz. Calibration values are
clearly provisional. No Wi-Fi editor, Bluetooth feature, audio or extra hardware.

Rendering and sequencing are separate portable C++ modules. Host raster previews
and the physical display adapter execute the same renderer, bitmap font, content
and layout. Regional redraws avoid per-loop full-screen clears or full-screen sprites.
All current content fits: headings 32px, bodies 24px in 2-4 lines. Ellipsis gets a
single-cell custom glyph; no wording was shortened. Mounted readability is pending.

## Files to review

1. `docs/MOM_GENIX_Display_Specification.docx`: retained complete v1.3 authority.
2. `lib/MomGenix/src/messages.h`, `timing_config.h`: exact wording, severities, timing.
3. `lib/MomGenix/src/state_machine.*`: actual gesture and sequence implementation.
4. `tests/state_machine_tests.cpp`: tests against those exact sources.
5. `lib/MomGenix/src/layout.*`, `renderer.*`, `font_data.h`: wrapping, pixels, redraws.
6. `src/hardware.cpp`, `src/main.cpp`, `include/config.h`, `include/board_setup.h`:
   hardware boundaries, radio shutdown, backlight and touch-bus separation.
7. `platformio.ini`, requirements files, `docs/BOARD_PROVENANCE.md`: reproducibility.
8. `preview/index.html`, `preview/all_messages.png`: ten frames plus startup/PAUSED.
9. `README.md`, `HARDWARE_CHECKLIST.md`: Windows/arrival/handoff procedures.
10. `build-results.txt`, `test-results.txt`, milestone checksums and source archive.

## Commands and actual results

Run from project root on Windows, October 22, 2026:

```powershell
.\.venv\Scripts\python.exe -m platformio run
.\.venv\Scripts\python.exe tools/verify.py
```

Costume build results: `cyd_ili9341` SUCCESS; `cyd_st7789` SUCCESS.
Static RAM 48,264 bytes each; flash 892,765 and 892,665 bytes respectively.
Build durations: 19.574s and 16.610s. Full retained log: `build-results.txt`.
Both arrival diagnostics also compile successfully with
`-e diagnostic_ili9341 -e diagnostic_st7789`, in 24.134s and 22.200s;
see `diagnostic-build-results.txt`. They were not uploaded or hardware-tested.
No PSRAM flag, sprite or framebuffer is used by firmware. The host preview's raster
buffer is host-only. Static memory figures do not measure hardware heap/stack use.

Host: 9 groups passed, 6512 checks including rectangle bounds. C++11 with
`-Wall -Wextra -Werror -pedantic`, Zig 0.13.0 native Windows compiler.
`test-results.txt` retains the successful final run. Groups cover:

- Startup, 70-second uninterrupted list loop, wrap and reset defaults.
- AUTO and PAUSED taps; full AUTO interval reset; PAUSED beyond two intervals.
- 49ms rejected / 50ms accepted; strictly greater press threshold; hysteresis.
- Hold toggles while down at exactly 2000ms, long hold latch, no action on release.
- 1999/2000/2001ms release boundary; fresh AUTO interval after resume release.
- Touch at timer expiry, frozen progress, no double advance.
- Stable release, release bounce and no repeated latch/rearm.
- Startup, automatic timing, held gesture and release debounce across rollover.
- Every message layout fits within drawing bounds; repeat frames draw nothing;
  progress-only updates do not redraw messages; no full-screen message clears.

Previews: 10 native 320x240 message PNGs, startup, PAUSED, contact sheet, local HTML.
Inspected the contact sheet and startup; automated bounds checks cover every draw.
Headings wrap up to two lines; bodies for ERROR 1965, WARNING 022 and MEMORY ALERT
use four lines. Other bodies use two or three. No current fallback or clipping.
PNG RGB565-to-RGB888 expansion is deterministic; actual LCD optics, color order,
brightness, timing, physical orientation and readability are not simulated.

Compile-verified stack: Core 6.1.18; espressif32 6.10.0; Arduino ESP32 2.0.17;
GCC 8.4.0+2021r2-patch5; esptool 1.40501.0; SCons 4.40801.0; TFT_eSPI 2.5.43;
XPT2046 v1.4 commit `d57f64c8b5f2bc5b8d10d121550806eeff7b06d9`.
Hardware-tested versions: none. XPT archive has a synthetic PlatformIO display
version; `library.properties` says 1.4, source commit is pinned.
Expected warning: TFT_eSPI TOUCH_CS undefined, because XPT uses separate VSPI.
Initial compile mistakes were fixed before the retained successful runs. A sandbox
could not execute the downloaded compiler; authorized host execution succeeded.

## Specific reviewer questions

1. Check touch/deadline ordering and that release debounce never lengthens a tap
   into a hold. Is retaining a contact through the configured hysteresis band
   consistent with intended noisy-panel behavior?
2. Review pressure defaults 600/450 and 50ms stable release as provisional choices.
   The touch library internally clamps Z below 400 to zero. Can physical traces
   reliably separate release noise and comfortable pressure with that limitation?
3. Review the shared 5x7 raster layout at 32/24px; is it readable in the mounted
   costume? Any wording shortening requires user approval, not automatic truncation.
4. Verify selected upstream inversion/color order against actual panel; connector
   count does not establish controller. Is the retained 55MHz bus stable on arrival?
5. Check the power bank's low-load mode timeout and endurance using the actual cable,
   fixed brightness and intended wear duration before declaring the release ready.

## Physical checks and release status

All physical items in HARDWARE_CHECKLIST.md remain unchecked: board identification,
flash size, controller, display colors/orientation, electrical backlight polarity,
touch threshold/coordinate calibration, real response latency and noise, RGB-off
level, USB startup, low-load behavior, endurance, heat, mounted readability and
strain relief. Five-millisecond sampling is a software target; regional SPI drawing
and library calls take physical time that must be measured on-board.

The source/compile-verified Tier 1A milestone is preserved locally before optional
work. Do not call it a hardware-tested firmware backup or ship-ready costume.
The package contains 49 source/document/preview/log files, both profiles' flashing
inputs, an integrity-checked source ZIP and SHA256SUMS.txt. The packaging tool
refuses accidental overwrite by default; later hardware milestones should use a
separate name. No Git commit or remote update was needed to preserve this backup.
No reviewer pause was requested; implementation did not wait for Claude. No
messages were sent to another chat or external reviewer. No purchase or service
deployment occurred. Repository publication is recorded separately in Git history.
