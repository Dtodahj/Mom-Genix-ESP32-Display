# Physical acceptance checklist

All physical items are pending. No physical hardware validation has occurred.
Tier 1A compiles and its actual state machine passes host tests against v1.3;
those software checks do not complete any item below. Record delivered board,
dependency versions, measured thresholds, fixed duty, bank model/mode/procedure,
mode timeout, wear duration target, measured runtime and mounting conditions.

## Identification and initial diagnostic

- [ ] Photograph board revision, USB bridge, panel/controller labels and connectors.
- [ ] Confirm classic ESP32 and actual flash size; adjust profile only with evidence.
- [ ] Record selected profile, firmware revision/hash, library versions and date.
- [ ] Upload via data cable; check boot and serial output at 115200 baud.
- [ ] Verify landscape 320 by 240, readable text, and red/green/blue diagnostic bars.
- [ ] Check black/white inversion, full panel coverage and stable SPI operation.
- [ ] Verify fixed PWM brightness, no unwanted flash at boot and no flicker on camera.
- [ ] Verify touch independently of display traffic; log corners, center and pressure.
- [ ] Measure touch release/noise behavior before choosing calibration/filter values.

## Final Tier 1A firmware acceptance

- [ ] Upload reviewed `cyd_ili9341` or `cyd_st7789` Tier 1A build after diagnostic bring-up.
- [ ] Confirm MOM-GENIX / DOMESTIC UNIT 1965 for three seconds, then first message in AUTO.
- [ ] Compare all ten messages and severities character for character with v1.3.
- [ ] Inspect every message at 320 by 240: font, wrap, margins, color and clipping.
- [ ] Verify seven seconds each and 70-second uninterrupted loop, including 10 to 1.
- [ ] Tap once in AUTO: exactly one advance; check timer behavior per v1.3.
- [ ] Tap once in PAUSED: exactly one advance while remaining paused.
- [ ] Reject noise and threshold-edge contacts according to v1.3.
- [ ] Hold for two seconds: toggle while still pressed, once per contact.
- [ ] Continue holding longer: no repeated toggles; release: no extra tap.
- [ ] Verify touch arrival at auto expiry never skips two entries.
- [ ] Reject 49ms noise, accept 50ms valid contact; pressure must exceed calibrated threshold.
- [ ] Verify 1999ms release taps; exactly 2000ms release holds despite release debounce.
- [ ] Confirm stable release is required before rearming; bounce causes no extra action.
- [ ] Confirm candidate touch freezes progress, including rejected noise.
- [ ] Verify full seven-second AUTO interval after stable release, noise and resumed hold.
- [ ] Remain PAUSED for more than 14 seconds without automatic advance.
- [ ] Compare physical touch traces against host tests at timing boundaries.
- [ ] Check repeated reset/power-cycle operation without a computer/network.
- [ ] Confirm application requires no PSRAM or full-screen sprite.

## Costume and USB power acceptance before October 31, 2026

- [ ] Verify USB-A cable's device-end connector fits actual board/power stick.
- [ ] Disconnect power stick before connecting board to PC for programming.
- [ ] Test the actual power stick, cable and mounting arrangement together.
- [ ] Run for expected wear duration plus margin; record runtime and any shutoff.
- [ ] Record low-current mode activation procedure and whether that mode times out.
- [ ] Check for resets, low-load power-bank cutoff, loose cable and thermal discomfort.
- [ ] Check readability indoors and outdoors at fixed brightness.
- [ ] Check at 3-6 feet and standing-height angles; request approval before shortening wording.
- [ ] Verify mounting does not press the resistive panel continuously.
- [ ] Save a known-good Tier 1A binary and documented build before optional testing.
- [ ] Leave ambient brightness and RGB independently disabled until each is tested.

Optional Tier 1B testing must use onboard parts only and preserve Tier 1A fallback.
Record accepted board profile and versions as hardware-tested only after completion.

## Calibration and acceptance record

| Item | Actual value / result |
| --- | --- |
| Board / PCB / display controller / USB bridge | Pending |
| Flash size / chosen profile / source and binary hash | Pending |
| Hardware-tested tool/library versions | None |
| Raw corner/center coordinates and axis direction | Pending |
| Pressure noise / comfortable press range / press / release thresholds | Pending (defaults 600 / 450 uncalibrated) |
| Stable release duration / measured response latency | Pending (default 50ms release) |
| Backlight polarity / accepted fixed duty | Pending (default 153 / 255) |
| RGB pins confirmed off at HIGH | Pending |
| Power-stick model / capacity / output rating | Pending |
| Mode activation steps / timeout / cable | Pending |
| Wear-duration target / actual endurance / lowest duty tested | Pending |
| Mount / viewing distance / angle / thermal observations | Pending |
| Tester / date / freeze / final handoff approval | Pending |
