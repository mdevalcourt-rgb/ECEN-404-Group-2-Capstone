#include "Ad7356Sampler.h"
#include "StartupLog.h"

const std::array<const char *, 2> Ad7356Sampler::kLabels = {{"CH A", "CH B"}};

void Ad7356Sampler::begin() const {
  logf("[AD7356] begin(): SCLK=GPIO%d  CS=GPIO%d  DOUTA=GPIO%d  DOUTB=GPIO%d\n",
                kSclkPin, kCsPin, kSdataA, kSdataB);

  pinMode(kSclkPin, OUTPUT);
  digitalWrite(kSclkPin, LOW);
  logf("[AD7356]   SCLK GPIO%d → OUTPUT LOW  (reads back %d)\n",
                kSclkPin, digitalRead(kSclkPin));

  pinMode(kCsPin, OUTPUT);
  digitalWrite(kCsPin, HIGH);
  logf("[AD7356]   CS   GPIO%d → OUTPUT HIGH (reads back %d)\n",
                kCsPin, digitalRead(kCsPin));

  pinMode(kSdataA, INPUT);
  const int idleA = digitalRead(kSdataA);
  logf("[AD7356]   DOUTA GPIO%d → INPUT  idle=%d%s\n",
                kSdataA, idleA,
                idleA == 1 ? "  (HIGH — floating or chip unpowered?)" : "");

  pinMode(kSdataB, INPUT);
  const int idleB = digitalRead(kSdataB);
  logf("[AD7356]   DOUTB GPIO%d → INPUT  idle=%d%s\n",
                kSdataB, idleB,
                idleB == 1 ? "  (HIGH — floating or chip unpowered?)" : "");

  logln("[AD7356] begin() done.");
}

// ---------------------------------------------------------------------------
// Verbose diagnostic read — call once from setup() after begin().
// Prints the raw 16-bit SPI word, frame-structure check, 12-bit result,
// and scaled voltage for one conversion, then a 32-sample burst summary.
// ---------------------------------------------------------------------------
void Ad7356Sampler::diagnose() const {
  logln("[AD7356] === diagnose() start ===");

  // --- single-conversion bit trace ---
  uint16_t rawA = 0, rawB = 0;
  digitalWrite(kCsPin, LOW);
  for (int i = 0; i < 16; i++) {
    digitalWrite(kSclkPin, HIGH);
    delayMicroseconds(1);
    rawA = static_cast<uint16_t>((rawA << 1) | digitalRead(kSdataA));
    rawB = static_cast<uint16_t>((rawB << 1) | digitalRead(kSdataB));
    digitalWrite(kSclkPin, LOW);
    delayMicroseconds(1);
  }
  digitalWrite(kCsPin, HIGH);

  // Frame layout: [15:14] leading zeros | [13:2] 12-bit data | [1:0] trailing zeros
  const uint8_t leadA  = (rawA >> 14) & 0x03;
  const uint8_t trailA = rawA & 0x03;
  const uint8_t leadB  = (rawB >> 14) & 0x03;
  const uint8_t trailB = rawB & 0x03;
  const uint16_t adcA  = (rawA >> 2) & 0x0FFF;
  const uint16_t adcB  = (rawB >> 2) & 0x0FFF;
  const float voltA    = adcA * kVoltageScale - kVoltageBias;
  const float voltB    = adcB * kVoltageScale - kVoltageBias;

  logf("[AD7356]   CH A  raw16=0x%04X  lead=%d trail=%d  adc12=%4u  %.3f V\n",
                rawA, leadA, trailA, adcA, voltA);
  logf("[AD7356]   CH B  raw16=0x%04X  lead=%d trail=%d  adc12=%4u  %.3f V\n",
                rawB, leadB, trailB, adcB, voltB);

  if (leadA != 0 || trailA != 0)
    logln("[AD7356]   WARNING CH A: frame structure wrong — leading/trailing bits non-zero."
                   " Check SCLK/CS timing or wiring.");
  if (leadB != 0 || trailB != 0)
    logln("[AD7356]   WARNING CH B: frame structure wrong — leading/trailing bits non-zero."
                   " Check SCLK/CS timing or wiring.");
  if (adcA == 0x0FFF)
    logln("[AD7356]   WARNING CH A: saturated HIGH (4095) — input floating, open, or above +Vref.");
  if (adcA == 0x0000)
    logln("[AD7356]   WARNING CH A: saturated LOW  (0)    — input shorted to GND or below −Vref.");
  if (adcB == 0x0FFF)
    logln("[AD7356]   WARNING CH B: saturated HIGH (4095) — input floating, open, or above +Vref.");
  if (adcB == 0x0000)
    logln("[AD7356]   WARNING CH B: saturated LOW  (0)    — input shorted to GND or below −Vref.");

  // --- 32-sample burst stats ---
  constexpr size_t kDiagN = 32;
  uint16_t bA[kDiagN], bB[kDiagN];
  const uint32_t t0 = micros();
  readBurst(bA, bB, kDiagN);
  const uint32_t elapsed = micros() - t0;

  uint16_t minA = 0xFFFF, maxA = 0, minB = 0xFFFF, maxB = 0;
  uint32_t sumA = 0, sumB = 0;
  for (size_t i = 0; i < kDiagN; ++i) {
    if (bA[i] < minA) minA = bA[i];
    if (bA[i] > maxA) maxA = bA[i];
    if (bB[i] < minB) minB = bB[i];
    if (bB[i] > maxB) maxB = bB[i];
    sumA += bA[i];
    sumB += bB[i];
  }
  const float avgA = static_cast<float>(sumA) / kDiagN;
  const float avgB = static_cast<float>(sumB) / kDiagN;
  const uint32_t rateHz = elapsed > 0
      ? static_cast<uint32_t>((uint64_t)kDiagN * 1000000UL / elapsed)
      : 0;

  logf("[AD7356]   burst %u samples in %lu µs → sample rate ~%lu Hz\n",
                kDiagN, elapsed, rateHz);
  logf("[AD7356]   CH A  min=%4u  max=%4u  avg=%.1f  range=%.3f V\n",
                minA, maxA, avgA,
                (maxA - minA) * kVoltageScale);
  logf("[AD7356]   CH B  min=%4u  max=%4u  avg=%.1f  range=%.3f V\n",
                minB, maxB, avgB,
                (maxB - minB) * kVoltageScale);

  if (maxA - minA < 4)
    logln("[AD7356]   WARNING CH A: near-zero range across burst — signal missing or DC only?");
  if (maxB - minB < 4)
    logln("[AD7356]   WARNING CH B: near-zero range across burst — signal missing or DC only?");
  if (rateHz < 5000)
    logln("[AD7356]   WARNING: measured sample rate < 5 kHz — cannot capture 5 kHz signals.");

  logln("[AD7356] === diagnose() end ===");
}

// ---------------------------------------------------------------------------

// AD7356 SPI frame: CS low → 16 clock pulses → CS high.
// Frame layout: 2 leading zeros | 12-bit data | 2 trailing zeros.
// Both DOUTA and DOUTB are clocked simultaneously.
SampleSet Ad7356Sampler::read() const {
  uint16_t rawA = 0;
  uint16_t rawB = 0;

  digitalWrite(kCsPin, LOW);
  for (int i = 0; i < 16; i++) {
    digitalWrite(kSclkPin, HIGH);
    delayMicroseconds(1);
    rawA = static_cast<uint16_t>((rawA << 1) | digitalRead(kSdataA));
    rawB = static_cast<uint16_t>((rawB << 1) | digitalRead(kSdataB));
    digitalWrite(kSclkPin, LOW);
    delayMicroseconds(1);
  }
  digitalWrite(kCsPin, HIGH);

  SampleSet sample{};
  sample.values[0] = (rawA >> 2) & 0x0FFF;
  sample.values[1] = (rawB >> 2) & 0x0FFF;
  return sample;
}

// Burst capture: collect `count` samples back-to-back as fast as GPIO allows.
// CS must toggle HIGH between each conversion — the AD7356 only acquires a new
// sample on the falling edge of CS, so leaving CS low for the whole burst would
// result in DOUT going to 0 after the first 16 clocks.
void Ad7356Sampler::readBurst(uint16_t *bufA, uint16_t *bufB,
                               size_t count) const {
  // Deadline scheduling: sample interval = 17 µs → ~58,824 Hz ≈ 60 kHz.
  // The interval is measured from the START of each conversion, so bit-bang
  // overhead (~4 µs for 16 SCLK cycles) is absorbed into the idle gap.
  // int32_t cast on the difference handles micros() wraparound correctly.
  constexpr uint32_t kSamplePeriodUs = 17;
  uint32_t deadline = micros();
  for (size_t s = 0; s < count; ++s) {
    while (static_cast<int32_t>(micros() - deadline) < 0) {}
    deadline += kSamplePeriodUs;
    uint16_t rawA = 0;
    uint16_t rawB = 0;
    digitalWrite(kCsPin, LOW);
    for (int i = 0; i < 16; i++) {
      digitalWrite(kSclkPin, HIGH);
      rawA = static_cast<uint16_t>((rawA << 1) | digitalRead(kSdataA));
      rawB = static_cast<uint16_t>((rawB << 1) | digitalRead(kSdataB));
      digitalWrite(kSclkPin, LOW);
    }
    digitalWrite(kCsPin, HIGH);
    bufA[s] = (rawA >> 2) & 0x0FFF;
    bufB[s] = (rawB >> 2) & 0x0FFF;
  }
}
