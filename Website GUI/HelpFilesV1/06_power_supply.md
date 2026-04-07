# Power Supply

The MADAM3 provides three voltage rails for powering your circuits.

## Pins Used

| Pin | Location | Function |
|---|---|---|
| **V\_adj** | Upper header, pin 1 | Adjustable output: 0.64–5.25 V |
| **−5V** | Upper header, pin 4 | Fixed −5 V output |
| **R in (+5V)** | Bottom header, pin 4 | Fixed +5 V output (shared with resistance checker) |
| **GND** | Multiple locations | Common ground (use any GND pin) |

> **Note:** The **V+** and **V−** pins on the bottom header are DMM measurement inputs only — they do not output any voltage. For a fixed +5 V power source, use **R in (+5V)**.

## V\_adj — Adjustable Power Supply

Controlled by the MIC24045 programmable buck converter over I²C. Set the output voltage on this screen.

**Wiring:**

1. Connect **V\_adj** to the positive rail of your breadboard or directly to the circuit node that needs power.
2. Connect any **GND** pin to the breadboard ground rail.
3. Set the voltage on this screen and enable the output.

**Typical uses:**

- Main 5 V supply for most labs (set to 5.00 V)
- Low-voltage DC input for op amp circuits (e.g., set to 0.5 V)

> Always verify the actual V\_adj output with the DMM after setting it. The Lab Manual requires you to record the measured supply voltage.

## −5V — Fixed Negative Rail

A fixed −5 V output. No configuration needed.

**Wiring:**

1. Connect the **−5V** pin to the circuit node that needs a negative supply (typically the op amp's V− or VEE pin).
2. Ensure **GND** is connected to your breadboard ground rail.

**Used in:** Op amp labs (Labs 3 and 7) as the negative supply rail.

## R in (+5V) — Fixed +5 V Rail

A fixed +5 V output sourced from the resistance measurement circuit. When you are not measuring resistance, this pin provides a clean +5 V rail for your circuits. No configuration needed.

**Wiring:**

1. Connect the **R in (+5V)** pin to the circuit node that needs +5 V.
2. Ensure **GND** is connected to your breadboard ground rail.

**Typical uses:**

- Op amp positive supply when V\_adj is being used as a signal input
- Logic gate IC power (VCC, typically pin 14)

> Do not use this pin for +5 V power and resistance measurement at the same time.

## Op Amp Wiring Summary

Op amp circuits need three simultaneous connections:

| What | DC Input Labs | AC Input Labs |
|---|---|---|
| V\_in (signal) | V\_adj (set low, e.g. 0.5 V) | Waveform Generator Output |
| Op amp + rail | R in (+5V) | V\_adj (set to 5.0 V) |
| Op amp − rail | −5V | −5V |

> **WARNING:** Connecting +5 V and −5 V to the wrong op amp pins will destroy the chip. Double-check polarity before enabling the supply.
