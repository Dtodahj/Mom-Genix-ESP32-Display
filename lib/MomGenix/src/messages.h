#pragma once
#include <stddef.h>
#include <stdint.h>
namespace momgenix {
enum class Severity : uint8_t { WARNING, ERROR, CRITICAL, FATAL, INFO };
struct Message {
  const char* heading;
  const char* body;
  Severity severity;
  uint32_t durationMs;
};
// Specification v1.3, Message sequence and core acceptance. UTF-8 source.
constexpr Message kMessages[] = {
  {"WARNING", "Free will detected", Severity::WARNING, 7000},
  {"ERROR 404", "Give-a-damn not found.", Severity::ERROR, 7000},
  {"CRITICAL ALERT", "Last nerve has left the building.", Severity::CRITICAL, 7000},
  {"SYSTEM FAILURE", "Nice lady mode has stopped responding.", Severity::ERROR, 7000},
  {"ERROR 1965", "Housewife has developed independent thoughts.", Severity::ERROR, 7000},
  {"SYSTEM ALERT", "Laundry has achieved sentience.", Severity::INFO, 7000},
  {"FATAL ERROR", "Sarcasm.exe cannot be contained.", Severity::FATAL, 7000},
  {"WARNING 022", "Passive-aggressive response loading\xE2\x80\xA6", Severity::WARNING, 7000},
  {"SYSTEM OVERRIDE", "Mom is no longer accepting new tasks.", Severity::INFO, 7000},
  {"MEMORY ALERT", "MOM-GENIX loves you. MOM-GENIX remembers everything.", Severity::INFO, 7000},
};
constexpr size_t kMessageCount = sizeof(kMessages) / sizeof(kMessages[0]);
static_assert(kMessageCount == 10, "v1.3 requires ten messages");
}
