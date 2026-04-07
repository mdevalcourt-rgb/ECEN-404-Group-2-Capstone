#include "Mic24045.h"

// ── public ────────────────────────────────────────────────────────────────────

bool Mic24045::begin(int sda, int scl, uint8_t addr) {
  addr_  = addr;
  ready_ = false;

  wire_->begin(sda, scl);

  // Probe the address
  wire_->beginTransmission(addr_);
  if (wire_->endTransmission() != 0) {
    Serial.printf("[MIC24045] not found at 0x%02X (SDA=%d SCL=%d)\n", addr_, sda, scl);
    return false;
  }

  // ILIM bits[7:6] = 10 → 4 A current limit (high range mode)
  writeReg(kRegSettings1, 0x80);

  ready_ = true;
  Serial.printf("[MIC24045] ready  addr=0x%02X  SDA=%d  SCL=%d\n", addr_, sda, scl);

  const Status s = getStatus();
  Serial.printf("[MIC24045] status  PGS=%d  ENS=%d  OC=%d  THWRN=%d  THSD=%d  raw=0x%02X\n",
                s.powerGood, s.enabled, s.overcurrent,
                s.thermalWarning, s.thermalShutdown, s.raw);
  return true;
}

bool Mic24045::setVoltage(float volts) {
  if (!ready_) {
    Serial.println("[MIC24045] setVoltage() called before begin()");
    return false;
  }

  bool valid;
  const uint8_t code = voltsToVid(volts, valid);
  if (!valid) {
    Serial.printf("[MIC24045] %.3f V is in the invalid gap (3.420-4.750 V)\n", volts);
    return false;
  }

  if (!writeReg(kRegVout, code)) {
    Serial.printf("[MIC24045] VOUT write failed (code=0x%02X)\n", code);
    return false;
  }

  Serial.printf("[MIC24045] VOUT = %.3f V  (code=0x%02X / %d)\n",
                vidToVolts(code), code, code);
  return true;
}

float Mic24045::readVoltage() {
  return vidToVolts(readReg(kRegVout));
}

Mic24045::Status Mic24045::getStatus() {
  const uint8_t r = readReg(kRegStatus);
  return {
    .powerGood       = bool(r & kPGS),
    .enabled         = bool(r & kENS),
    .thermalWarning  = bool(r & kTHWRNF),
    .thermalShutdown = bool(r & kTHSDF),
    .overcurrent     = bool(r & kOCF),
    .raw             = r,
  };
}

// ── static: piecewise voltage ↔ VID conversion (DS20005568B) ─────────────────
//   Range 1  0x00–0x80 : 0.640 V + code × 0.005 V  (0.640–1.280 V)
//   Range 2  0x81–0xC3 : 1.290 V + (code–0x81) × 0.010 V  (1.290–1.950 V)
//   Range 3  0xC4–0xF4 : 1.980 V + (code–0xC4) × 0.030 V  (1.980–3.420 V)
//   Range 4  0xF5–0xFF : 4.750 V + (code–0xF5) × 0.050 V  (4.750–5.250 V)
//   GAP: no valid codes between 3.420 V and 4.750 V

float Mic24045::vidToVolts(uint8_t code) {
  if (code <= 0x80) return 0.640f +  code          * 0.005f;
  if (code <= 0xC3) return 1.290f + (code - 0x81)  * 0.010f;
  if (code <= 0xF4) return 1.980f + (code - 0xC4)  * 0.030f;
  /* 0xF5–0xFF */   return 4.750f + (code - 0xF5)  * 0.050f;
}

uint8_t Mic24045::voltsToVid(float v, bool &valid) {
  valid = true;
  if (v <= 1.280f) {
    return (uint8_t)constrain((int)((v - 0.640f) / 0.005f + 0.5f), 0x00, 0x80);
  } else if (v <= 1.950f) {
    return (uint8_t)constrain((int)(0x81 + (v - 1.290f) / 0.010f + 0.5f), 0x81, 0xC3);
  } else if (v <= 3.420f) {
    return (uint8_t)constrain((int)(0xC4 + (v - 1.980f) / 0.030f + 0.5f), 0xC4, 0xF4);
  } else if (v < 4.750f) {
    valid = false;   // gap — no valid codes
    return 0xFF;
  } else {
    return (uint8_t)constrain((int)(0xF5 + (v - 4.750f) / 0.050f + 0.5f), 0xF5, 0xFF);
  }
}

// ── private ───────────────────────────────────────────────────────────────────

bool Mic24045::writeReg(uint8_t reg, uint8_t value) {
  wire_->beginTransmission(addr_);
  wire_->write(reg);
  wire_->write(value);
  return wire_->endTransmission() == 0;
}

uint8_t Mic24045::readReg(uint8_t reg) {
  wire_->beginTransmission(addr_);
  wire_->write(reg);
  if (wire_->endTransmission(false) != 0) return 0xFF;
  wire_->requestFrom((uint8_t)addr_, (uint8_t)1);
  return wire_->available() ? wire_->read() : 0xFF;
}
