#pragma once

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>

struct AdsChannelResult {
  float rawAvg;  // oversampled average raw ADC bits (fractional resolution)
  float value;   // calibrated physical quantity
  bool  isOC;    // open-circuit flag (AIN0 resistance only)
};

// Reads the ADS1115 external 16-bit ADC over I²C (Wire, SDA=6, SCL=5).
// Channel mapping:
//   ch=0  AIN0 single-ended   → Resistance (Ω),  inverse fit
//   ch=1  AIN1 single-ended   → Current (A),      linear fit
//   ch=2  AIN2-3 differential → DC Voltage (V),   linear fit
// Call begin() once in setup(). Call measure(ch) on demand to perform a
// blocking 500 ms oversampled measurement at 860 SPS on the given channel.
class Ads1115Sampler {
 public:
  Ads1115Sampler() = default;
  bool begin();
  AdsChannelResult measure(uint8_t ch);

 private:
  // I²C bus / device config
  static constexpr uint8_t  kSdaPin     = 6;
  static constexpr uint8_t  kSclPin     = 5;
  static constexpr uint8_t  kI2cAddress = 0x48;

  // AIN0: inverse fit  bits → Ω  (y = A/x − B)
  static constexpr float kAin0A =  40711273.7439f;
  static constexpr float kAin0B =      1521.7255f;
  // rawAvg <= this threshold → open circuit
  static constexpr float kAin0OcBits = 0.1f;

  // AIN1: linear fit  bits → A  (y = M·x + B)
  static constexpr float kAin1M = -0.00065354f;
  static constexpr float kAin1B =  13.37348381f;

  // AIN2-3 differential: linear fit  bits → V  (y = M·x + B)
  static constexpr float kDiffM =  0.00021034f;
  static constexpr float kDiffB =  0.00013654f;

  // Applies piecewise linear correction: returns estimate − interpolated_error.
  static float applyPiecewiseCorrection(float estimate,
                                         const float* estTable,
                                         const float* errTable,
                                         size_t n);

  Adafruit_ADS1115 _ads;
  bool             _ready = false;
};
