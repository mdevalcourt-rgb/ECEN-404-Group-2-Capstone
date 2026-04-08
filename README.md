# ESP32-S3 Web Bench Instrument

A self-contained, browser-accessible bench instrument built across 4 custom PCBs — providing a dual-channel oscilloscope, digital multimeter (resistance, current, DC voltage), programmable waveform generator, and adjustable power supply, all served over a self-hosted Wi-Fi access point with no router or installed software required.

## ICs and Their Roles

| IC | Role |
|----|------|
| **ESP32-S3** | Central node — runs the HTTP server, serves the web app from flash, handles all SPI/I²C communication |
| **AD7356** | Dual 12-bit SAR ADC — simultaneous two-channel oscilloscope sampling at 60 kHz (PCB 4) |
| **ADS1115** | 16-bit delta-sigma ADC over I²C — DMM front-end for resistance, current, and DC voltage (PCB 1) |
| **AD9833** | DDS waveform generator over SPI — sine, triangle, and square wave output up to 200 kHz (PCB 3) |
| **MIC24045** | Programmable buck converter over I²C — adjustable output 0.64–5.25 V with live status readback (PCB 2) |

## Built With

- **PlatformIO** on **ESP32-S3** (Arduino framework)
- Vanilla HTML/CSS/JavaScript single-page app embedded in firmware flash
- Python calibration scripts generating empirical piecewise linear fits for all ADC channels

## Documentation

See [README/SETUP_GUIDE.md](README/SETUP_GUIDE.md) for:

- Step-by-step setup instructions for VS Code, PlatformIO, and Claude Code
- Build and flash instructions (USB and OTA)
- Running the web GUI mock server without hardware
- Full web GUI user guide (oscilloscope, DMM, waveform generator, power supply)
