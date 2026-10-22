#include "layout.h"
#include <string.h>
namespace momgenix {
Lines wrap(const char* text, size_t columns) {
  Lines result;
  char glyphs[256] = {};
  size_t n = 0;
  for (size_t i = 0; text[i] && n < sizeof(glyphs) - 1; ) {
    if (static_cast<unsigned char>(text[i]) == 0xe2 &&
        static_cast<unsigned char>(text[i+1]) == 0x80 &&
        static_cast<unsigned char>(text[i+2]) == 0xa6) { glyphs[n++] = 0x7f; i += 3; }
    else glyphs[n++] = text[i++];
  }
  if (columns == 0 || columns > 63) { result.fits = false; return result; }
  size_t start = 0;
  while (start < n) {
    while (start < n && glyphs[start] == ' ') ++start;
    if (start == n) break;
    if (result.count == 8) { result.fits = false; break; }
    size_t end = start + columns;
    if (end > n) end = n;
    if (end < n && glyphs[end] != ' ') {
      size_t split = end;
      while (split > start && glyphs[split-1] != ' ' && glyphs[split-1] != '-') --split;
      if (split > start) end = split;
    }
    size_t trimmed = end;
    while (trimmed > start && glyphs[trimmed-1] == ' ') --trimmed;
    memcpy(result.text[result.count++], glyphs + start, trimmed - start);
    start = end;
  }
  return result;
}
}
