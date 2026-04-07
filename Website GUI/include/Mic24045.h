#pragma once

#include <Arduino.h>
#include <Wire.h>

class Mic24045 {
 public:
  Mic24045() = default;

  // Initialise the shared I²C bus and configure the chip.
  // Uses the same bus as ADS1115 (Wire, SDA=6, SCL=5).
  bool begin(int sda = 6, int scl = 5, uint8_t addr = 0x50);

  // Set output voltage (V).
  // Valid ranges: 0.640–3.420 V and 4.750–5.250 V.
  // Returns false if the voltage is in the invalid gap, out of range,
  // or the I²C write fails.
  bool setVoltage(float volts);

  // Read back the VOUT register and convert to volts.
  float readVoltage();

  struct Status {
    bool    powerGood;
    bool    enabled;
    bool    thermalWarning;
    bool    thermalShutdown;
    bool    overcurrent;
    uint8_t raw;
  };
  Status getStatus();

  // Voltage ↔ VID code conversion (piecewise, per DS20005568B).
  static float   vidToVolts(uint8_t code);
  static uint8_t voltsToVid(float v, bool &valid);

  bool ready() const { return ready_; }

 private:
  static constexpr uint8_t kRegStatus    = 0x00;
  static constexpr uint8_t kRegSettings1 = 0x01;
  static constexpr uint8_t kRegVout      = 0x03;

  // STATUS register bit masks
  static constexpr uint8_t kPGS    = 0x01;  // bit0 Power Good
  static constexpr uint8_t kENS    = 0x08;  // bit3 EN pin HIGH
  static constexpr uint8_t kTHWRNF = 0x20;  // bit5 Thermal Warning
  static constexpr uint8_t kTHSDF  = 0x40;  // bit6 Thermal Shutdown
  static constexpr uint8_t kOCF    = 0x80;  // bit7 Overcurrent

  bool    writeReg(uint8_t reg, uint8_t value);
  uint8_t readReg(uint8_t reg);

  TwoWire *wire_  = &Wire;
  uint8_t  addr_  = 0x50;
  bool     ready_ = false;
};
