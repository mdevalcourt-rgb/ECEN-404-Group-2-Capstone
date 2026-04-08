# MADAM3 — Complete Setup & User Guide

This guide walks you through everything needed to build, flash, and use the MADAM3 bench instrument firmware — from a clean machine to a running device. It also covers how to run the web GUI locally without hardware using the self-hosted mock server.

---

## Table of Contents

1. [Prerequisites Overview](#1-prerequisites-overview)
2. [Installing VS Code](#2-installing-vs-code)
3. [Installing PlatformIO](#3-installing-platformio)
4. [Installing Claude Code (optional)](#4-installing-claude-code-optional)
5. [Opening the Project](#5-opening-the-project)
6. [Building and Flashing the Firmware](#6-building-and-flashing-the-firmware)
7. [Over-the-Air (OTA) Updates](#7-over-the-air-ota-updates)
8. [Web GUI Self-Hosted Mock Server](#8-web-gui-self-hosted-mock-server)
9. [Web GUI User Guide](#9-web-gui-user-guide)

---

## 1. Prerequisites Overview

You will need the following installed before starting:

| Tool | Purpose | Required |
|------|---------|----------|
| **VS Code** | Code editor | Yes |
| **PlatformIO** | Firmware build system (VS Code extension) | Yes |
| **Python 3.8+** | Running the mock dev server | For mock server only |
| **Claude Code** | AI-assisted coding in the terminal | Optional |

---

## 2. Installing VS Code

1. Go to [https://code.visualstudio.com/](https://code.visualstudio.com/) and download the installer for your OS.
2. Run the installer and accept all defaults.
3. Launch VS Code once the installation completes.

---

## 3. Installing PlatformIO

PlatformIO is the build system used for this project. It runs as a VS Code extension.

1. In VS Code, click the **Extensions** icon in the left sidebar (or press `Ctrl+Shift+X`).
2. Search for **PlatformIO IDE**.
3. Click **Install** on the result published by *PlatformIO*.
4. Wait for the installation to finish — this may take a few minutes as it downloads the PlatformIO core tools.
5. When prompted, **reload VS Code** (or restart it manually).

After reloading, you will see the PlatformIO alien-head icon in the left sidebar. Click it to confirm PlatformIO is active.

> **Note:** PlatformIO automatically installs the ESP32 toolchain and all required libraries the first time you build the project. You do not need to install them manually.

---

## 4. Installing Claude Code (optional)

Claude Code is a command-line AI assistant for working with code. It is not required to build or flash the firmware, but it is useful for understanding and modifying the codebase.

### Prerequisites for Claude Code

- **Node.js 18 or later** — download from [https://nodejs.org/](https://nodejs.org/) and install using the LTS (Long Term Support) version.

### Installing Claude Code

Open a terminal (on Windows: press `Win+R`, type `cmd`, press Enter) and run:

```bash
npm install -g @anthropic-ai/claude-code
```

### First-time login

```bash
claude
```

On the first run, Claude Code will open a browser window and prompt you to log in with your Anthropic account. Follow the on-screen steps to authenticate.

### Using Claude Code in this project

1. Open a terminal inside VS Code (`Ctrl+`` ` ``).
2. Navigate to the project folder:
   ```bash
   cd "Website GUI"
   ```
3. Start Claude Code:
   ```bash
   claude
   ```
4. You can now ask questions about the firmware or request changes. For example:
   - *"Explain how the HTTP server routes are set up"*
   - *"What does the ADS1115 sampler do?"*
   - *"Add a new API endpoint that returns the chip temperature"*

---

## 5. Opening the Project

The firmware project lives in the `Website GUI/` subfolder.

1. In VS Code, go to **File → Open Folder**.
2. Navigate to and select the `Website GUI` folder inside this repository.
3. VS Code will detect the `platformio.ini` file and activate PlatformIO automatically.

You should see two environments listed in the PlatformIO sidebar:
- `esp32s3usbotg` — used for the first flash over USB
- `esp32s3usbotg-ota` — used for subsequent wireless updates

---

## 6. Building and Flashing the Firmware

### First flash (USB cable required)

1. Connect the ESP32-S3 to your computer with a USB cable.

2. In the VS Code PlatformIO sidebar, expand **esp32s3usbotg** and click **Upload**, or use the toolbar at the bottom of VS Code and click the right-arrow (→) upload button.

   Alternatively, open the PlatformIO terminal (`Ctrl+`` ` ```) and run:
   ```bash
   pio run -e esp32s3usbotg -t upload
   ```

3. PlatformIO will:
   - Download the ESP32 toolchain on first run (this takes a few minutes)
   - Compile all source files
   - Flash the binary to the board

4. Once flashing completes, open the serial monitor to confirm startup:
   - Click the plug icon at the bottom of VS Code, or run:
     ```bash
     pio device monitor
     ```
   - You should see output including `HTTP server started` and the AP IP address.

5. On your phone or laptop, connect to the Wi-Fi access point:
   - **SSID:** `ESP32-Oscilloscope`
   - **Password:** `scopeview123`

6. Open a browser and navigate to `http://192.168.4.1/`. The web GUI will load automatically (captive portal redirects handle this on most devices).

### Serial monitor commands

Once the serial monitor is open, you can type:

| Command | Effect |
|---------|--------|
| `log` | Replay all startup messages (useful if the monitor was not open during boot) |

---

## 7. Over-the-Air (OTA) Updates

After the first USB flash, you can update the firmware wirelessly without a cable.

1. Connect your computer to the `ESP32-Oscilloscope` Wi-Fi network (password: `scopeview123`).

2. In PlatformIO, select the **esp32s3usbotg-ota** environment and click **Upload**, or run:
   ```bash
   pio run -e esp32s3usbotg-ota -t upload
   ```

3. PlatformIO connects to `192.168.4.1` using the OTA password `scopeota123` and uploads the new firmware wirelessly.

> **Important:** Your computer must be connected to the ESP32's Wi-Fi network — not your normal internet router — when performing an OTA update.

---

## 8. Web GUI Self-Hosted Mock Server

The mock server lets you run and develop the web GUI on your computer without any hardware. It extracts the HTML/CSS/JavaScript directly from `src/WebContent.cpp` and serves it locally, while providing simulated API responses for all instrument endpoints.

### Requirements

- Python 3.8 or later. Check by running:
  ```bash
  python --version
  ```
  If Python is not installed, download it from [https://www.python.org/downloads/](https://www.python.org/downloads/). During installation, check **"Add Python to PATH"**.

- No additional Python packages are required — the server uses only the standard library.

### Starting the server

1. Open a terminal in the `Website GUI/` folder.

   In VS Code: open the integrated terminal (`Ctrl+`` ` ```) and run:
   ```bash
   cd "Website GUI"
   ```

2. Start the server:
   ```bash
   python mock_server.py
   ```

3. You will see:
   ```
     MADAM3 dev server
     Open → http://localhost:8080/
     HTML from: ...\Website GUI\src\WebContent.cpp
     Ctrl+C to stop.
   ```

4. Open `http://localhost:8080/` in your browser.

### What the mock server simulates

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/` | GET | Serves the full web GUI |
| `/adc/burst` | GET | Returns simulated dual-channel oscilloscope data |
| `/ads/measure` | GET | Returns simulated DMM readings (voltage, current, resistance) |
| `/control-state` | GET | Returns current waveform generator and power supply state |
| `/waveform` | POST | Updates waveform generator settings |
| `/power` | POST | Updates power supply voltage |
| `/dev/config` | GET/POST | Reads or updates the simulated signal configuration panel |

### Using the developer configuration panel

The mock server exposes a **Dev Config** panel inside the web GUI (visible only when running against the mock server). This panel lets you control the simulated signals in real time:

**Oscilloscope channels:**

| Setting | Description |
|---------|-------------|
| Shape | Waveform shape for each channel: `sine`, `square`, or `triangle` |
| Frequency | Signal frequency in Hz |
| Amplitude | Peak voltage (up to ±2 V) |
| Phase | Phase offset in degrees (useful for testing phase-shift displays) |
| Noise | Gaussian noise level in volts added to both channels |

**DMM:**

| Setting | Description |
|---------|-------------|
| Voltage | Simulated DC voltage in volts |
| Current | Simulated current in amps |
| Resistance | Simulated resistance in ohms |

**Power supply:**

| Setting | Description |
|---------|-------------|
| Voltage | Simulated V_adj output voltage |

Changes take effect immediately — the next data poll from the GUI picks up the new values.

### Stopping the server

Press `Ctrl+C` in the terminal where the server is running.

---

## 9. Web GUI User Guide

The web GUI is a single-page application accessible from any browser connected to the MADAM3's Wi-Fi. It provides four instrument tabs.

### Connecting to the device

1. Power on the MADAM3.
2. On your device (phone, laptop, tablet), open Wi-Fi settings and connect to:
   - **SSID:** `ESP32-Oscilloscope`
   - **Password:** `scopeview123`
3. A captive portal should open the GUI automatically. If it does not, open a browser and go to `http://192.168.4.1/`.

---

### Oscilloscope

The oscilloscope displays real-time AC waveforms on two channels, sampled simultaneously by the AD7356 at up to 60 kHz.

**Connections:**

| Pin | Location | Function |
|-----|----------|----------|
| Channel A (BNC) | Top left | Scope input — channel A |
| Channel B (BNC) | Top right | Scope input — channel B |

**How to use:**

1. Clip the BNC probe ground leads to your circuit's ground rail.
2. Touch the probe tip to the node you want to observe.
3. Adjust the **timebase** slider if the waveform is too compressed or stretched.
4. Read measurements directly from the display:
   - **Peak-to-peak voltage** — amplitude of the signal
   - **RMS voltage** — useful for AC power calculations and lab reports
   - **Period / Frequency** — time between consecutive peaks

**Common measurements:**

- **Phase shift (Δφ):** Measure the time difference (Δt) between matching peaks on channel A and B. `Δφ = 360 × Δt × f`
- **Time constant (τ):** For RC/RL circuits, τ is the time to reach 63% of final value when charging, or 37% when discharging.

> Use the oscilloscope for AC signals. For DC voltage, use the DMM tab.

---

### DMM — DC Voltage

Measures DC voltage between two points.

**Connections:**

| Pin | Location | Function |
|-----|----------|----------|
| V+ | Bottom header, pin 3 | Positive measurement input |
| V− | Bottom header, pin 6 | Negative measurement input |

**How to use:**

1. Connect **V+** to the higher-potential node.
2. Connect **V−** to the lower-potential node (often ground).
3. Read the DC voltage from the screen.

To measure voltage across a specific component, place V+ and V− on either side of it — not to ground.

> V+ and V− are measurement inputs only. Do not use them to power your circuit.

---

### DMM — Current

Measures DC current through a branch. The MADAM3 must be inserted **in series** into the current path.

**Connections:**

| Pin | Location | Function |
|-----|----------|----------|
| Current+ | Middle header, pin 3 | Current input (connect to positive side) |
| Current− | Middle header, pin 4 | Current output (connect to negative side) |

**How to use:**

1. Break the circuit at the point where you want to measure current.
2. Connect **Current+** to the side closer to the positive supply.
3. Connect **Current−** to the side closer to ground/load.
4. Read the current from the screen.

**Safety:** Maximum supported current is ±5 A. Never connect Current+ and Current− directly across a voltage source — this creates a near-short-circuit.

---

### DMM — Resistance

Measures component resistance. The component **must be removed from your circuit** before measuring.

**Connections:**

| Pin | Location | Function |
|-----|----------|----------|
| R+ (R in / +5V) | Bottom header, pin 4 | 5 V source — connect to one leg |
| R out | Middle header, pin 6 | Sense input — connect to other leg |

**How to use:**

1. Remove the component from your circuit.
2. Connect **R+** to one leg of the component.
3. Connect the other leg to **R out**.
4. Read the resistance from the screen.

**Measurable components:** fixed resistors, potentiometers, photoresistors, thermistors, inductor DC winding resistance.

> Measuring resistance in-circuit gives incorrect results because other components create parallel current paths.

The **R in (+5V)** pin doubles as a fixed +5 V power rail when you are not measuring resistance.

---

### Waveform Generator

Outputs AC signals from the AD9833 DDS chip.

**Connections:**

| Pin | Location | Function |
|-----|----------|----------|
| Output | Top header, pin 3 | Signal output |
| GND | Top header, pins 1–2 | Signal return / ground reference |

**How to use:**

1. Connect **Output** to the input node of your circuit (V_in).
2. Connect an adjacent **GND** pin to your circuit's ground rail.
3. Configure the signal parameters on the Waveform Generator tab:

| Parameter | Description | Range |
|-----------|-------------|-------|
| Waveform | Shape: Sine, Square, or Triangle | — |
| Frequency | Signal frequency | Up to 200 kHz |
| Amplitude | Peak voltage of the signal | — |
| Phase | Phase shift | 0–360° |

4. Enable the output.

> The waveform generator outputs signals centered at 0 V — it does not support a built-in DC offset. Add an external bias circuit if a lab requires a DC offset.

**Common lab settings:**

| Lab type | Waveform | Frequency | Amplitude |
|----------|---------|-----------|-----------|
| RC/RL Transients | Square | 20 Hz | 1 V |
| AC Signals | Sine | 30 Hz | 2 V |
| Op Amp | Sine | 30 Hz | 0.5 V |
| Frequency Response | Sine | 1–1200 Hz (sweep) | 2 V |

---

### Power Supply

Provides three voltage rails for your circuits, controlled by the MIC24045 programmable buck converter.

**Available rails:**

| Rail | Pin | Value | Notes |
|------|-----|-------|-------|
| V_adj | Upper header, pin 1 | 0.64–5.25 V adjustable | Set on this screen |
| −5V | Upper header, pin 4 | Fixed −5 V | Always on |
| +5V (R in) | Bottom header, pin 4 | Fixed +5 V | Shared with resistance checker |

**How to use V_adj:**

1. Set the desired voltage on the Power Supply tab.
2. Connect **V_adj** to the positive rail of your circuit.
3. Connect any **GND** pin to the ground rail.
4. Enable the output.
5. Verify the actual output voltage with the DMM tab — always record the measured value, not just the set value.

**Op amp wiring summary:**

| Signal | DC input labs | AC input labs |
|--------|--------------|--------------|
| V_in (signal) | V_adj (set low, e.g. 0.5 V) | Waveform Generator Output |
| Op amp + rail | R in (+5V) | V_adj (set to 5.0 V) |
| Op amp − rail | −5V | −5V |

> **WARNING:** Connecting +5 V and −5 V to the wrong op amp pins will destroy the chip. Double-check polarity before enabling any supply.

---

### General Tips

- Always connect a **GND** pin to your breadboard ground rail before making any other connections.
- All measurements and outputs share a common ground.
- When the Wi-Fi indicator in the GUI shows disconnected, the browser has lost contact with the ESP32. Reload the page or reconnect to the `ESP32-Oscilloscope` network.
- The serial monitor (`pio device monitor`, 115200 baud) shows live diagnostic output. Type `log` to replay startup messages.
