# DMM — Resistance

Use this mode to measure the resistance of a component. The component **must be disconnected** from your circuit before measuring.

## Pins Used

| Pin | Location | Function |
|---|---|---|
| **R+** | Bottom header, pin 4 (labeled R in / +5V) | 5 V source — connect to one leg of the component |
| **R out** | Middle header, pin 6 | Sense input — connect to the other leg of the component |

## Wiring

1. **Remove** the component from your breadboard circuit.
2. Connect **R+** to one leg of the component.
3. Connect the other leg to **R out**.
4. Read the resistance from this screen.

## How It Works

The MADAM3 applies 5 V from R+ through the unknown component. The voltage at R out feeds into an internal resistor divider, and the ADS1115 ADC reads the resulting voltage. The software calculates resistance from the voltage ratio.

## What You Can Measure

- Fixed resistors (verify nominal values before building your circuit)
- Potentiometers (measure at minimum and maximum settings)
- Photoresistors (measure under different lighting conditions)
- Thermistors (measure at different temperatures)
- Inductor DC resistance (the winding resistance, not inductance)

## Important

- **Always disconnect** the component first. Measuring resistance in-circuit gives incorrect results because other components provide parallel current paths.
- This mode measures **resistance only**. It cannot measure inductance or capacitance.

## Using R in (+5V) as a Power Rail

The **R in (+5V)** pin also serves as a fixed **+5 V source** for your circuits. When you are not measuring resistance, you can use this pin to power op amp positive rails or logic gate ICs. This is the only +5 V source on the MADAM3 — the V+ and V− pins on the same header are measurement inputs only.
