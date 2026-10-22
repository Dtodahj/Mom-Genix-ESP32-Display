#include "diagnostic_renderer.h"
#include <stdio.h>
namespace momgenix {
void drawDiagnostic(TFT_eSPI& display) {
  display.setTextColor(TFT_WHITE, TFT_BLACK);
  display.drawCentreString("MOM-GENIX", 160, 14, 4);
  display.drawCentreString("BOARD DIAGNOSTIC ONLY", 160, 54, 2);
  display.drawCentreString("Calibrate pressure before use", 160, 78, 2);
  display.drawCentreString("Touch: raw coordinates below", 160, 106, 2);
  display.fillRect(16, 204, 88, 20, TFT_RED);
  display.fillRect(116, 204, 88, 20, TFT_GREEN);
  display.fillRect(216, 204, 88, 20, TFT_BLUE);
}
void drawTouchDiagnostic(TFT_eSPI& display, TouchSample sample) {
  char line[64];
  snprintf(line, sizeof(line), "Touch %s x=%d y=%d z=%d",
           sample.down ? "DOWN" : "UP", sample.x, sample.y, sample.pressure);
  display.fillRect(0, 144, 320, 32, TFT_BLACK);
  display.drawCentreString(line, 160, 150, 2);
}
}
