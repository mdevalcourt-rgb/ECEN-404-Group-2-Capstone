# Oscilloscope

Use the oscilloscope to view and measure **AC signals**. For DC voltage measurements, use the DMM tab instead.

## Pins Used

| Pin | Location | Function |
|---|---|---|
| **Channel A** (BNC) | Top left | Scope input — channel A |
| **Channel B** (BNC) | Top right | Scope input — channel B |

Both channels are sampled simultaneously by the AD7356 dual 12-bit ADC at up to 60 kHz.

## Wiring

Use the **BNC probes** included with your kit.

1. Connect **Channel A** probe to the node you want to observe (typically V\_in).
2. Connect **Channel B** probe to the second node (typically V\_out).
3. Clip the probe ground leads to your breadboard ground rail.

## How to Take Common Measurements

**Amplitude** — Read peak or peak-to-peak voltage directly from the display.

**Time shift (Δt)** — Identify the time difference between matching peaks on Channel A and Channel B. Use this to calculate phase shift:

> Δφ = 360 × Δt × f

**Period (T)** — Measure the time between two consecutive peaks on the same channel. Frequency is:

> f = 1 / T

**Time constant (τ)** — For RC or RL transient circuits, τ is the time for the output to reach 63% of its final value (charging) or decay to 37% (discharging).

**AC RMS Voltage** — The oscilloscope displays the RMS voltage for each channel. Use this when the Lab Manual asks for "AC RMS measurement" or when using the "Logger tool" for AC signals.

## Tips

- Adjust the **timebase** if the waveform looks too compressed or too stretched.
- To save waveform data for your lab report, use the screenshot or export function.
