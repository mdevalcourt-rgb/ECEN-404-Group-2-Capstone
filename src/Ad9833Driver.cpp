#include "Ad9833Driver.h"

void Ad9833Driver::begin() {
  pinMode(kFsyncPin, OUTPUT);
  pinMode(kPotCsPin, OUTPUT);
  digitalWrite(kFsyncPin, HIGH);
  digitalWrite(kPotCsPin, HIGH);

  SPI.begin(kSckPin, /*miso=*/-1, kMosiPin, /*ss=*/-1);

  // Initialise AD9833 to 1 kHz sine, MCP41010 to mid-scale.
  apply("sine", 1000.0f, 2.0f);
}

// ---------------------------------------------------------------------------

void Ad9833Driver::writeAD9833(uint16_t data) {
  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE2));
  digitalWrite(kFsyncPin, LOW);
  delayMicroseconds(1);
  SPI.transfer16(data);
  delayMicroseconds(1);
  digitalWrite(kFsyncPin, HIGH);
  SPI.endTransaction();
}

void Ad9833Driver::setFrequency(float hz) {
  if (hz < 0.0f) hz = 0.0f;
  const uint32_t freqWord =
      static_cast<uint32_t>((hz * 268435456.0f) / kMclkHz);
  const uint16_t lsb = static_cast<uint16_t>((freqWord & 0x3FFF) | 0x4000);
  const uint16_t msb = static_cast<uint16_t>(((freqWord >> 14) & 0x3FFF) | 0x4000);

  writeAD9833(0x2100);  // B28 + RESET
  writeAD9833(lsb);
  writeAD9833(msb);
  // Note: do NOT send the final control word here — setWaveformShape() follows.
}

void Ad9833Driver::setWaveformShape(const String& mode) {
  uint16_t ctrl = kCtrlSine;
  if (mode == "triangle") ctrl = kCtrlTriangle;
  else if (mode == "square") ctrl = kCtrlSquare;
  writeAD9833(ctrl);
}

void Ad9833Driver::setAmplitudeVpk(float vpk) {
  if (vpk < 0.0f) vpk = 0.0f;
  const float vin   = vpk / kGain;
  const float ratio = vin / kVinMax;
  const uint8_t potValue =
      static_cast<uint8_t>(constrain(ratio * 255.0f, 0.0f, 255.0f));

  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
  digitalWrite(kPotCsPin, LOW);
  delayMicroseconds(1);
  SPI.transfer(0x11);       // write wiper 0
  SPI.transfer(potValue);
  delayMicroseconds(1);
  digitalWrite(kPotCsPin, HIGH);
  SPI.endTransaction();
}

// ---------------------------------------------------------------------------

void Ad9833Driver::apply(const String& mode, float hz, float amplitudeVpp) {
  setFrequency(hz);
  setWaveformShape(mode);
  setAmplitudeVpk(amplitudeVpp / 2.0f);  // Vpp → Vpk
}
