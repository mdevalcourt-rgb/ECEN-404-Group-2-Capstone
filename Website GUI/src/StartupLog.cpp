#include "StartupLog.h"

#include <Arduino.h>
#include <vector>

static std::vector<String> startupLog;

void logln(const char *msg) {
  startupLog.push_back(String(msg));
  Serial.println(msg);
}

void logf(const char *fmt, ...) {
  char buf[128];
  va_list args;
  va_start(args, fmt);
  vsnprintf(buf, sizeof(buf), fmt, args);
  va_end(args);
  startupLog.push_back(String(buf));
  Serial.print(buf);
}

void printStartupLog() {
  Serial.println("--- startup log replay ---");
  for (const auto &line : startupLog) {
    Serial.println(line);
  }
  Serial.println("--- end of log ---");
}
