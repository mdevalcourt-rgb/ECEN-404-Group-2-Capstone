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

  // Apply all three parameters in one call.
  // mode      : "sine" | "triangle" | "square"
  // hz        : 0.1 – 12.5 MHz (AD9833 limit with 25 MHz MCLK)
  // amplitudeVpp : desired peak-to-peak output in volts;
  //               hardware range ≈ 2.0–4.0 Vpp (1.0–2.0 Vpk with ×4 gain stage)
  void apply(const String& mode, float hz, float amplitudeVpp);

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

  // MCP41010 amplitude stage:
  //   AD9833 max output ≈ 0.75 V peak → ×4 op-amp → 3.0 V peak max
  //   Vpot = (Vpk_desired / kGain) / kVinMax → ratio for digipot
  static constexpr float kGain   = 4.0f;
  static constexpr float kVinMax = 0.75f;

  void writeAD9833(uint16_t data);
  void setFrequency(float hz);
  void setWaveformShape(const String& mode);
  void setAmplitudeVpk(float vpk);
};
