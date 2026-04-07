# MADAM3 Quick Start

Welcome to the MADAM3 bench instrument. This device provides four tools accessible from the tabs above:

- **Oscilloscope** — View and measure AC waveforms on two channels
- **DMM** — Measure DC voltage, current, and resistance
- **Waveform Generator** — Output sine, square, and triangle waves
- **Power Supply** — Adjustable and fixed voltage rails for your circuits

## Before You Begin

1. Connect a jumper wire from any **GND** pin on the MADAM3 to your breadboard's ground rail. Do this before making any other connections.
2. All measurements and outputs share this common ground. Skipping this step will produce incorrect readings or damage components.

## Pin Layout (Top to Bottom)

| Location | Pins | Purpose |
|---|---|---|
| Top (BNC) | Channel A, Channel B | Oscilloscope inputs |
| Top (header) | GND, GND, Output | Waveform generator |
| Upper header | V\_adj, GND, GND, −5V | Power supply |
| Middle header | NC, NC, Current+, Current−, GND, R out | DMM (current & resistance) |
| Bottom header | NC, NC, V+, R in (+5V), GND, V− | DMM (voltage) / +5V rail (R in) |

## Connecting

1. Power on the MADAM3 and connect to its Wi-Fi access point.
2. The web GUI will open automatically once connected.

Select a tab above to see detailed wiring instructions for each tool.
