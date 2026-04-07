#include "Ads1115Sampler.h"

#include <Wire.h>

namespace {
// ---------------------------------------------------------------------------
// Piecewise correction tables  —  { estimate, error (overcounting) }
// corrected = estimate − error
// ---------------------------------------------------------------------------

// AIN0 resistance (Ω)  — inverse fit: A/x − B
static const float kAin0Est[] = {  -2.0357f,   89.2815f,   999.0496f,  9831.7448f, 102069.3018f};
static const float kAin0Err[] = {-11.857701f, -8.738523f, 15.549586f,   5.744792f,    -0.698154f};

// AIN1 current (A)  — linear fit: M·x + B
static const float kAin1Est[] = {0.012088f, 0.099139f, 0.198084f, 0.299252f, 0.400550f, 0.499887f};
static const float kAin1Err[] = {0.002088f,-0.000861f,-0.001916f,-0.000748f, 0.000550f, 0.000887f};

// AIN2-3 differential DC voltage (V)  — linear fit: M·x + B
static const float kDiffEst[] = {0.000599f, 0.999443f, 1.998328f, 2.997634f, 3.996982f, 4.996015f};
static const float kDiffErr[] = {-0.000401f, 0.000443f, 0.000328f,-0.000366f,-0.000018f, 0.000015f};
}  // namespace

// ---------------------------------------------------------------------------

float Ads1115Sampler::applyPiecewiseCorrection(float estimate,
                                                const float* estTable,
                                                const float* errTable,
                                                size_t n) {
  if (n == 0) return estimate;
  if (estimate <= estTable[0])     return estimate - errTable[0];
  if (estimate >= estTable[n - 1]) return estimate - errTable[n - 1];
  for (size_t i = 0; i < n - 1; ++i) {
    if (estimate < estTable[i + 1]) {
      const float t = (estimate - estTable[i]) / (estTable[i + 1] - estTable[i]);
      return estimate - (errTable[i] + t * (errTable[i + 1] - errTable[i]));
    }
  }
  return estimate - errTable[n - 1];
}

// ---------------------------------------------------------------------------

bool Ads1115Sampler::begin() {
  Serial.printf("[ADS1115] begin() called  SDA=%d SCL=%d addr=0x%02X\n",
                kSdaPin, kSclPin, kI2cAddress);

  Serial.println("[ADS1115] Wire.begin()");
  Wire.begin(kSdaPin, kSclPin);

  Serial.println("[ADS1115] _ads.begin()");
  if (!_ads.begin(kI2cAddress, &Wire)) {
    Serial.println("[ADS1115] _ads.begin() FAILED – device not found");
    return false;
  }

  Serial.println("[ADS1115] setGain GAIN_ONE");
  _ads.setGain(GAIN_ONE);
  _ads.setDataRate(RATE_ADS1115_860SPS);
  _ready = true;
  Serial.println("[ADS1115] begin() OK");
  return true;
}

// ---------------------------------------------------------------------------
// Blocking 500 ms measurement at 860 SPS.
// ch=0: AIN0 single-ended   → Resistance (inverse fit, OC detection)
// ch=1: AIN1 single-ended   → Current    (linear fit)
// ch=2: AIN2-3 differential → DC Voltage (linear fit)
// ---------------------------------------------------------------------------
AdsChannelResult Ads1115Sampler::measure(uint8_t ch) {
  AdsChannelResult result{};
  if (!_ready || ch > 2) return result;

  float    sum   = 0.0f;
  int32_t  count = 0;
  const uint32_t start = millis();
  while (millis() - start < 500) {
    if (ch == 2) {
      sum += static_cast<float>(_ads.readADC_Differential_2_3());
    } else {
      sum += static_cast<float>(_ads.readADC_SingleEnded(ch));
    }
    ++count;
  }
  result.rawAvg = (count > 0) ? sum / static_cast<float>(count) : 0.0f;

  switch (ch) {
    case 0: {
      // AIN0: ohmmeter — inverse fit then piecewise correction.
      if (result.rawAvg <= kAin0OcBits) {
        result.isOC  = true;
        result.value = 0.0f;
      } else {
        const float est = kAin0A / result.rawAvg - kAin0B;
        result.value = applyPiecewiseCorrection(est, kAin0Est, kAin0Err,
                                                sizeof(kAin0Est) / sizeof(kAin0Est[0]));
        if (result.value < 0.0f) result.value = 0.0f;
        result.isOC = false;
      }
      break;
    }
    case 1: {
      // AIN1: current sensor — linear fit then piecewise correction.
      const float est = kAin1M * result.rawAvg + kAin1B;
      result.value = applyPiecewiseCorrection(est, kAin1Est, kAin1Err,
                                              sizeof(kAin1Est) / sizeof(kAin1Est[0]));
      break;
    }
    case 2: {
      // AIN2-3 differential: DC voltage — linear fit then piecewise correction.
      const float est = kDiffM * result.rawAvg + kDiffB;
      result.value = applyPiecewiseCorrection(est, kDiffEst, kDiffErr,
                                              sizeof(kDiffEst) / sizeof(kDiffEst[0]));
      break;
    }
  }
  return result;
}
