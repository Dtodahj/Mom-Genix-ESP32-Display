# Specification v1.3 implementation map

Authority: retained MOM_GENIX_Display_Specification.docx, version 1.3,
October 22, 2026. Original file is preserved verbatim. Physical bring-up items
remain pending under the user's explicit direction to develop without the board.

| Requirement | Implementation / software evidence | Physical gate |
| --- | --- | --- |
| Three-second identity, reset to first AUTO entry | StateMachine startup; main shows identity before starting timer; startup_auto_wrap test | Cold boot timing and optics |
| Ten exact messages, explicit severity and duration | messages.h; body retains UTF-8 ellipsis; INFO override explicit | Compare on-screen text with source |
| Seven seconds each, loop in order | StateMachine::advance/tick; 70-second wrap test | Observe uninterrupted operation |
| Candidate freezes timer; pressure > threshold for 50ms | StateMachine qualification; noise_and_thresholds, expiry_and_freeze | Calibrate 600/450 provisional defaults |
| Tap on release in AUTO/PAUSED | stable release then advance; taps_and_paused | Repeat physical taps |
| 2.0s hold from initial candidate; immediate mode while held | downAt_, valid_, held_; hold_and_release | Measure response and repeatability |
| Exactly 2.0s release is hold; 1999ms tap unaffected by release debounce | releaseAt_; exact_release_boundaries | Repeat boundary/contact traces |
| Hold release no extra action; resume full interval | held_ latch; hold_and_release | Verify no bounce-triggered extra action |
| Expiry only AUTO without candidate; touch first | tick checks pressure before elapsed expiry | Finger at deadline |
| Stable release/hysteresis; invalid contact resets AUTO | release timer and thresholds; noise/bounce tests | Measure release noise and latency |
| Nonblocking and rollover-safe timing | uint32_t subtraction in machine/main; rollover test | Check real SPI draw latency |
| Landscape black/branding/heading/body/footer/progress | shared layout/renderer; bounds checks and twelve preview frames | Color order, inversion, mounting |
| No clipping, no wording shortening | wrap/fallback/LAYOUT ERROR; all current bodies at 24px | Viewing distance 3-6ft |
| No PSRAM / full-screen device sprite | direct Canvas::rect adapter; dirty-region test | Check actual heap/stack and flicker |
| Exclusive GPIO21 ownership | TFT_BL omitted plus compile guard; LEDC app-only | Polarity, PWM brightness/flicker |
| Independent display/touch buses | HSPI forced profile / dedicated SPIClass(VSPI) | Verify actual revision |
| Exact dependencies and variant profiles | platformio.ini and Python lockfiles; BOARD_PROVENANCE.md | Freeze passing physical combination |
| Unused radios disabled; no persistence | WiFi.mode(WIFI_OFF), btStop, no editor/settings storage | Network-independent cold boot |
| USB power stick | Boot needs only USB; no peripherals/network | Actual USB-A bank/cable/endurance |
| Tier 1B independently disabled | separate false flags + compile guards; RGB off, LDR unused | Optional work only after 1A acceptance |
| Phase 2 deferred | No AP, server, editing, settings save, or startup setup gesture | After Halloween only |

Implementation choices where v1.3 leaves calibration or exact presentation open:
50ms stable release; provisional Z=600 press, Z=450 release; 60% fixed backlight;
32px heading/24px body cells; severity color mapping on screen; progress freezes
with a candidate and remains frozen PAUSED. AUTO's new interval starts at stable
release confirmation. A startup-held finger begins a new candidate at AUTO entry.
No physical quantities are represented as calibrated or validated.
