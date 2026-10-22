# Board configuration evidence

Checked 2026-10-22 against witnessmenow/ESP32-Cheap-Yellow-Display revision
`0564a140c76de76ef34a8b7115044e7c6c852695` (GitHub API main HEAD at retrieval).

Primary sources:

- [ILI9341 setup](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/0564a140c76de76ef34a8b7115044e7c6c852695/DisplayConfig/User_Setup.h)
- [CYD2USB setup](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/0564a140c76de76ef34a8b7115044e7c6c852695/DisplayConfig/CYD2USB/User_Setup.h)
- [Dedicated touch SPI example](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/0564a140c76de76ef34a8b7115044e7c6c852695/Examples/Basics/2-TouchTest/2-TouchTest.ino)

| Setting | ILI9341 | CYD2USB ST7789 |
| --- | --- | --- |
| TFT_eSPI driver | ILI9341_2_DRIVER | ST7789_DRIVER |
| Color order | Driver default | TFT_BGR |
| Inversion | Driver default | TFT_INVERSION_OFF |
| MISO / MOSI / clock | 12 / 13 / 14 | 12 / 13 / 14 |
| CS / DC / reset | 15 / 2 / -1 | 15 / 2 / -1 |
| Display bus | HSPI | HSPI |
| Write / read frequency | 55 / 20 MHz | 55 / 20 MHz |
| Backlight | GPIO21 active HIGH | GPIO21 active HIGH |

Touch example: SPIClass(VSPI), clock 25, MISO 39, MOSI 32, CS 33, IRQ 36,
rotation 1. Both touch and display use rotation 1 in this project.

Intentional changes from upstream:

- `TFT_BL` and `TFT_BACKLIGHT_ON` are omitted so TFT_eSPI cannot alter the
  application-owned backlight. Application uses GPIO21 through LEDC.
- Explicit portrait dimensions 240 by 320 for both configurations. Upstream
  CYD2USB leaves dimensions commented; TFT_eSPI's ST7789 defaults must match.
- Fonts 1, 2, and 4 enabled for the diagnostic. Costume rendering uses a shared
  copy of the pinned library's Adafruit 5x7 bitmap at integer scales 4 and 3,
  with scale 2 fallback. License retained in THIRD_PARTY_NOTICES.txt.
- No TFT_eSPI touch configuration; separate XPT2046 library and VSPI object.
- Upstream's blocking 100 ms touch delay is replaced by unsigned elapsed timing.
- Full-screen clears only at initialization/startup exit; message, footer and
  progress regions redraw on relevant changes. No full-screen device buffer.

These examples establish candidate configurations, not the controller fitted to
the ordered unit. USB connector count alone does not identify the controller.
Verify panel markings, visual colors, inversion, orientation, and SPI reliability
on arrival. The upstream 55 MHz setting is retained, not physically verified.

No board has been tested. Do not call any dependency version hardware-tested.

Also checked the pinned [PINS.md](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/blob/0564a140c76de76ef34a8b7115044e7c6c852695/PINS.md):
RGB GPIO4/16/17 are active-low and are driven HIGH for off. GPIO34 light sensing
is unused. All mappings still require confirmation on the delivered revision.
The pinned core 2.0.17 deliberately uses ledcSetup/ledcAttachPin, consistent with
the [Espressif migration guide](https://docs.espressif.com/projects/arduino-esp32/en/latest/migration_guides/2.x_to_3.0.html),
which removes those APIs in core 3.x. This is not a claim that all 3.x builds fail.
