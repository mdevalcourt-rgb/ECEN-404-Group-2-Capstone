#pragma once

#include <Arduino.h>
#include <array>

struct SampleSet {
  std::array<uint16_t, 2> values{};
};

// Bit-bang SPI driver for the AD7356 dual 12-bit ADC.
// ESP32-S3 pin assignments (avoids flash GPIO 26-32 and USB GPIO 19-20):
//   SCLK  → GPIO 12
//   CS    → GPIO 10
//   DOUTA → GPIO 11  (Channel A)
//   DOUTB → GPIO 13  (Channel B)
class Ad7356Sampler {
 public:
  Ad7356Sampler() = default;
  // Nominal sample rate enforced by deadline scheduling in readBurst().
  // 17 µs inter-sample period → 1,000,000 / 17 = 58,824 Hz, reported as 60 kHz.
  static constexpr uint32_t kSampleRateHz = 60000;
  void begin() const;
  // Verbose self-test: pin states, single-conversion bit trace, 32-sample
  // burst stats, and warnings for stuck-rail / framing / rate issues.
  // Call once from setup() after begin().
  void diagnose() const;
  SampleSet read() const;
  // Collect `count` back-to-back conversions with the given inter-sample period.
  // bufA and bufB must each hold at least `count` uint16_t values (12-bit).
  // periodUs: inter-sample period in microseconds (min 17 → ~59 kHz, default fast mode).
  void readBurst(uint16_t *bufA, uint16_t *bufB, size_t count, uint32_t periodUs = 17) const;

  // Single-frequency DFT via the Goertzel algorithm.
  // Evaluates the DFT of `buf` at exactly `freq` Hz (non-integer bin is fine).
  // DC is removed before processing to suppress ADC bias leakage.
  // mag:   peak amplitude in ADC counts (use ratio magB/magA for gain).
  // phase: phase in radians [-π, π].
  static void binDFT(const uint16_t *buf, size_t n, float freq, float fs,
                     float *mag, float *phase);
  size_t channelCount() const { return kLabels.size(); }
  const char *label(size_t index) const { return kLabels[index]; }

 private:
  static const std::array<const char *, 2> kLabels;
  static constexpr uint8_t kSclkPin = 12;
  static constexpr uint8_t kCsPin   = 10;
  static constexpr uint8_t kSdataA  = 11;
  static constexpr uint8_t kSdataB  = 13;
  // 12-bit, 2.5 V Vref, 10× attenuator, 1.25 V bias
  // input_V = (raw × 2.5/4095 − 1.25) × 10
  static constexpr float kVoltageScale = 25.0f / 4095.0f;
  static constexpr float kVoltageBias  = 12.5f;
};
