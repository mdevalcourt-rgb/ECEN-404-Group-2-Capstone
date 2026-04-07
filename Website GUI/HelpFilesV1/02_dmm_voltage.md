# DMM — DC Voltage

Use this mode to measure DC voltage between two points in your circuit.

## Pins Used

| Pin | Location | Function |
|---|---|---|
| **V+** | Bottom header, pin 3 | Differential voltage input (+) |
| **V−** | Bottom header, pin 6 | Differential voltage input (−) |

V+ and V− are **measurement inputs only** — they do not output any voltage. Do not use these pins to power your circuit. For +5 V power, use the **R in (+5V)** pin on the resistance checker header.

## Wiring

1. Connect **V+** to the higher-potential node in your circuit.
2. Connect **V−** to the lower-potential node (often your ground rail).
3. Read the DC voltage from this screen.

## Measuring Across a Specific Component

To measure the voltage drop across a single resistor, capacitor, or other component, place **V+** on one side and **V−** on the other side of that component — not to ground.

## Verifying Your Power Supply

After setting V\_adj in the Power Supply tab, you should always measure its actual output:

1. Connect **V+** to the V\_adj output wire (where it enters your circuit).
2. Connect **V−** to GND.
3. Record the reading — this is your true supply voltage.
