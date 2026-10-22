#pragma once
// Verified against witnessmenow/ESP32-Cheap-Yellow-Display
// revision 0564a140c76de76ef34a8b7115044e7c6c852695. See docs/BOARD_PROVENANCE.md.
#if defined(MOM_GENIX_ILI9341) == defined(MOM_GENIX_ST7789)
#error "Select exactly one MOM-GENIX display profile"
#endif
#ifdef MOM_GENIX_ILI9341
#define ILI9341_2_DRIVER
#define USER_SETUP_INFO "MOM-GENIX ILI9341"
#else
#define ST7789_DRIVER
#define TFT_RGB_ORDER TFT_BGR
#define TFT_INVERSION_OFF
#define USER_SETUP_INFO "MOM-GENIX CYD2USB ST7789"
#endif
#define TFT_WIDTH 240
#define TFT_HEIGHT 320
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS 15
#define TFT_DC 2
#define TFT_RST -1
#define USE_HSPI_PORT
#define SPI_FREQUENCY 55000000
#define SPI_READ_FREQUENCY 20000000
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
// Deliberately no TFT_BL: the application exclusively owns GPIO21.
// Deliberately no TOUCH_CS: XPT2046 uses its own SPIClass(VSPI).
