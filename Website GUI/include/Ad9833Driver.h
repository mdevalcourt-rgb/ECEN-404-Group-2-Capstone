#pragma once

#include <Arduino.h>
#include <SPI.h>

// AD9833 DDS waveform generator + MCP41010 digital potentiometer amplitude control.
//
// Pin assignments (ESP32-S3, avoids USB GPIO19-20, flash GPIO26-32,
// AD7356 GPIO10-13, ADS1115/MIC24045 I²C GPIO5-6):
//   SPI SCK    → GPIO 18
//   SPI MOSI   → GPIO 14
//   AD9833 /FSYNC (CS) → GPIO 15   (SPI_MODE2)
//   MCP41010   CS      → GPIO 16   (SPI_MODE0)
//
// Call begin() once in setup(). Then call apply() whenever any parameter
// changes — it updates frequency, waveform shape, and amplitude atomically.
class Ad9833Driver {
 public:
  Ad9833Driver() = default;
  void begin();

  // Apply all parameters in one call.
  // mode        : "sine" | "triangle" | "square"
  // hz          : 0.1 – 12.5 MHz (AD9833 limit with 25 MHz MCLK)
  // targetV     : desired output voltage (V); mapped to digipot via calibrated fit
  // phaseDeg    : phase offset in degrees (0–360), written to PHASE0 register
  void apply(const String& mode, float hz, float targetV, float phaseDeg = 0.0f);

  // Write a raw 8-bit wiper value (0–255) directly to the MCP41010,
  // bypassing the calibrated fit entirely.
  void setAmplitudeRaw(uint8_t potValue);

 private:
  // SPI pins
  static constexpr uint8_t kSckPin    = 18;
  static constexpr uint8_t kMosiPin   = 14;
  static constexpr uint8_t kFsyncPin  = 15;  // AD9833 chip-select (active LOW)
  static constexpr uint8_t kPotCsPin  = 16;  // MCP41010 chip-select (active LOW)

  // AD9833 master clock (25 MHz oscillator)
  static constexpr float kMclkHz = 25000000.0f;

  // AD9833 control words (B28 mode, RESET cleared)
  static constexpr uint16_t kCtrlSine     = 0x2000;
  static constexpr uint16_t kCtrlTriangle = 0x2002;
  static constexpr uint16_t kCtrlSquare   = 0x2028;

  // AD9833 phase register write prefix — DB15:DB14=11, DB13=0 → PHASE0
  static constexpr uint16_t kPhase0Write = 0xC000;

  // MCP41010 amplitude calibration — least-squares fit through three
  // confirmed hardware measurements: (0.5 V → 65), (1.0 V → 165), (1.7 V → 255).
  //   pot = kCalSlope * targetV + kCalOffset   (clamped to [0, 255])
  static constexpr float kCalSlope  = 156.422f;
  static constexpr float kCalOffset = -5.183f;

  void writeAD9833(uint16_t data);
  void setFrequency(float hz);
  void setPhase(float degrees);
  void setWaveformShape(const String& mode);
};
