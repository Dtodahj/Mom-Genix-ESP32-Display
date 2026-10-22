#pragma once
#include <TFT_eSPI.h>
#include "hardware.h"
namespace momgenix {
void drawDiagnostic(TFT_eSPI& display);
void drawTouchDiagnostic(TFT_eSPI& display, TouchSample sample);
}
