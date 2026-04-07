#pragma once

// Buffered startup logger.
// All messages are printed to Serial immediately AND stored so they can be
// replayed at any time with printStartupLog() (e.g. when the user sends "log"
// over the serial monitor after the board has already booted).

void logln(const char *msg);
void logf(const char *fmt, ...);
void printStartupLog();
