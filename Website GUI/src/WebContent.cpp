#include "WebContent.h"

const char INDEX_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ESP32 ADC Monitor</title>
  <style>
    :root {
      font-family: "Segoe UI", Arial, sans-serif;
      background: #020617;
      color: #f8fafc;
    }
    * {
      box-sizing: border-box;
    }
    body {
      margin: 0;
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 2rem;
    }
    .card {
      width: min(1080px, 100%);
      background: rgba(15, 23, 42, 0.92);
      border-radius: 1.5rem;
      padding: 2rem;
      box-shadow: 0 30px 60px rgba(2, 6, 23, 0.6);
      border: 1px solid rgba(148, 163, 184, 0.2);
    }
    .eyebrow {
      font-size: 0.78rem;
      color: #94a3b8;
      letter-spacing: 0.35em;
      text-transform: uppercase;
      margin: 0;
    }
    h1 {
      margin: 0.25rem 0 0.5rem;
      font-size: clamp(1.5rem, 2vw, 2.4rem);
      letter-spacing: 0.08em;
      text-transform: uppercase;
    }
    .subtitle {
      margin-top: 0;
      color: #cbd5f5;
    }
    canvas {
      width: 100%;
      height: clamp(200px, 40vw, 420px);
      border-radius: 0.9rem;
      background: #010314;
      box-shadow: inset 0 0 0 1px rgba(148, 163, 184, 0.18);
      margin-top: 1rem;
      display: block;
    }
    .controls {
      display: grid;
      gap: 1rem;
      margin-top: 1.5rem;
    }
    .control-group {
      background: rgba(2, 6, 23, 0.65);
      border-radius: 1rem;
      padding: 1rem 1.25rem;
      border: 1px solid rgba(148, 163, 184, 0.15);
    }
    .control-title {
      margin: 0 0 0.5rem;
      font-size: 0.78rem;
      letter-spacing: 0.35em;
      text-transform: uppercase;
      color: #a5b4fc;
    }
    .button-row {
      display: flex;
      flex-wrap: wrap;
      gap: 0.5rem;
    }
    button {
      appearance: none;
      border: 1px solid rgba(148, 163, 184, 0.4);
      border-radius: 999px;
      background: rgba(15, 23, 42, 0.7);
      color: #f1f5f9;
      padding: 0.5rem 1.1rem;
      font-size: 0.9rem;
      cursor: pointer;
      transition: background 0.2s ease, border-color 0.2s ease;
    }
    button:hover {
      border-color: #f97316;
    }
    button.active {
      background: linear-gradient(120deg, #f97316, #fb7185);
      border-color: transparent;
      color: #0f172a;
      font-weight: 600;
    }
    .offset-input {
      margin-top: 0.75rem;
      display: flex;
      align-items: center;
      gap: 0.5rem;
      color: #cbd5f5;
      font-size: 0.9rem;
    }
    .offset-input input {
      width: 120px;
      border-radius: 0.75rem;
      border: 1px solid rgba(148, 163, 184, 0.4);
      padding: 0.35rem 0.75rem;
      background: rgba(15, 23, 42, 0.7);
      color: #f8fafc;
      font-size: 0.95rem;
    }
    .status-bar {
      margin-top: 1.25rem;
      display: flex;
      flex-wrap: wrap;
      gap: 1rem;
      font-size: 0.92rem;
      color: #94a3b8;
      justify-content: space-between;
    }
    .status-bar strong {
      color: #f8fafc;
    }
    .legend {
      display: flex;
      flex-wrap: wrap;
      gap: 0.85rem;
      margin-top: 1rem;
      font-size: 0.95rem;
    }
    .legend-item {
      display: flex;
      align-items: center;
      gap: 0.5rem;
      padding: 0.35rem 0.9rem;
      border-radius: 999px;
      background: rgba(148, 163, 184, 0.15);
    }
    .legend-color {
      width: 0.9rem;
      height: 0.9rem;
      border-radius: 50%;
    }
    .measurements {
      margin-top: 1.5rem;
      background: rgba(2, 6, 23, 0.65);
      border-radius: 1.25rem;
      border: 1px solid rgba(148, 163, 184, 0.18);
      padding: 1.25rem;
    }
    .measurements-header {
      display: flex;
      flex-wrap: wrap;
      justify-content: space-between;
      gap: 0.5rem;
    }
    .measurements h2 {
      margin: 0;
      font-size: 1rem;
      letter-spacing: 0.2em;
      text-transform: uppercase;
      color: #a5b4fc;
    }
    #cursor-status {
      margin: 0;
      color: #cbd5f5;
      font-size: 0.9rem;
    }
    .cursor-values {
      margin-top: 1rem;
      display: grid;
      gap: 0.75rem;
      grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
    }
    .cursor-card {
      background: rgba(148, 163, 184, 0.08);
      border-radius: 0.9rem;
      padding: 0.75rem;
      border: 1px solid rgba(148, 163, 184, 0.15);
    }
    .cursor-card h3 {
      margin: 0 0 0.35rem;
      font-size: 0.9rem;
      letter-spacing: 0.15em;
      text-transform: uppercase;
      color: #e0e7ff;
    }
    .cursor-card p {
      margin: 0.15rem 0;
      font-family: "Roboto Mono", monospace;
      font-size: 0.85rem;
      color: #f8fafc;
    }
    .delta-summary {
      margin-top: 1rem;
      font-size: 1rem;
      color: #fcd34d;
    }
    .sig-measurements {
      margin-top: 1.5rem;
      background: rgba(2, 6, 23, 0.65);
      border-radius: 1.25rem;
      border: 1px solid rgba(148, 163, 184, 0.18);
      padding: 1.25rem;
    }
    .sig-measurements h2 {
      margin: 0 0 1rem;
      font-size: 1rem;
      letter-spacing: 0.2em;
      text-transform: uppercase;
      color: #a5b4fc;
    }
    .sig-meas-channels {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
      gap: 0.75rem;
    }
    .sig-meas-card {
      background: rgba(148, 163, 184, 0.08);
      border-radius: 0.9rem;
      padding: 0.75rem 1rem;
      border: 1px solid rgba(148, 163, 184, 0.15);
    }
    .sig-meas-card h3 {
      margin: 0 0 0.5rem;
      font-size: 0.9rem;
      letter-spacing: 0.15em;
      text-transform: uppercase;
    }
    .sig-meas-row {
      display: flex;
      justify-content: space-between;
      align-items: baseline;
      font-size: 0.88rem;
      padding: 0.18rem 0;
      border-bottom: 1px solid rgba(148, 163, 184, 0.08);
    }
    .sig-meas-row:last-child { border-bottom: none; }
    .sig-meas-label { color: #94a3b8; }
    .sig-meas-value { font-family: "Roboto Mono", monospace; color: #f8fafc; }
    .section-title {
      margin: 0.1rem 0;
      font-size: clamp(1.1rem, 2vw, 1.35rem);
      letter-spacing: 0.12em;
      text-transform: uppercase;
    }
    .panel-header {
      display: flex;
      flex-wrap: wrap;
      justify-content: space-between;
      align-items: center;
      gap: 0.5rem;
    }
    .panel-chip {
      padding: 0.35rem 0.8rem;
      border-radius: 999px;
      border: 1px solid rgba(148, 163, 184, 0.35);
      font-size: 0.8rem;
      color: #cbd5f5;
      background: rgba(148, 163, 184, 0.08);
    }
    .panel-copy {
      margin: 0.3rem 0 0;
      color: #cbd5f5;
      font-size: 0.95rem;
    }
    .lab-grid {
      margin-top: 2rem;
      display: grid;
      gap: 1.25rem;
    }
    .meter-grid {
      display: grid;
      gap: 1rem;
      grid-template-columns: repeat(auto-fit, minmax(260px, 1fr));
    }
    .meter-card {
      background: rgba(2, 6, 23, 0.7);
      border-radius: 1rem;
      padding: 1rem 1.25rem;
      border: 1px solid rgba(148, 163, 184, 0.16);
      box-shadow: inset 0 0 0 1px rgba(255, 255, 255, 0.02);
    }
    .meter-label {
      margin: 0;
      letter-spacing: 0.15em;
      text-transform: uppercase;
      font-size: 0.85rem;
      color: #a5b4fc;
    }
    .meter-value {
      margin: 0.35rem 0;
      font-size: clamp(1.6rem, 3vw, 2.1rem);
      font-family: "Roboto Mono", monospace;
    }
    .meter-meta {
      margin: 0.15rem 0 0;
      color: #94a3b8;
      font-size: 0.92rem;
    }
    .meter-bar {
      position: relative;
      height: 14px;
      background: rgba(148, 163, 184, 0.2);
      border-radius: 999px;
      overflow: hidden;
      margin-top: 0.5rem;
    }
    .meter-bar span {
      position: absolute;
      left: 0;
      top: 0;
      height: 100%;
      width: 0;
      background: linear-gradient(120deg, #22d3ee, #6366f1);
      transition: width 0.2s ease;
    }
    .control-grid {
      display: grid;
      grid-template-columns: 2fr 1fr;
      gap: 1rem;
    }
    @media (max-width: 900px) {
      .control-grid {
        grid-template-columns: 1fr;
      }
    }
    .control-panel {
      background: rgba(2, 6, 23, 0.72);
      border-radius: 1rem;
      padding: 1rem 1.25rem 1.35rem;
      border: 1px solid rgba(148, 163, 184, 0.15);
    }
    .panel-title-row {
      display: flex;
      align-items: center;
      justify-content: space-between;
      gap: 0.5rem;
    }
    .control-panel h3 {
      margin: 0;
      font-size: 1rem;
      letter-spacing: 0.14em;
      text-transform: uppercase;
    }
    .supply-grid {
      margin-top: 1rem;
      display: grid;
      gap: 0.75rem;
      grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
    }
    .supply-card {
      background: rgba(15, 23, 42, 0.55);
      border-radius: 0.9rem;
      padding: 0.85rem;
      border: 1px solid rgba(148, 163, 184, 0.14);
    }
    .supply-heading {
      letter-spacing: 0.15em;
      text-transform: uppercase;
      font-size: 0.88rem;
      margin: 0 0 0.5rem;
    }
    .supply-slider {
      width: 100%;
    }
    .supply-input-row {
      display: flex;
      align-items: center;
      gap: 0.5rem;
      margin-top: 0.65rem;
    }
    .supply-input-row input {
      flex: 1;
      border-radius: 0.75rem;
      border: 1px solid rgba(148, 163, 184, 0.4);
      padding: 0.4rem 0.65rem;
      background: rgba(15, 23, 42, 0.7);
      color: #f8fafc;
      font-size: 0.95rem;
    }
    .supply-unit {
      color: #cbd5f5;
      font-weight: 600;
    }
    .supply-status {
      margin: 0.35rem 0 0;
      font-size: 0.9rem;
      color: #94a3b8;
    }
    .wave-buttons button {
      flex: 1 1 120px;
    }
    .waveform-status {
      margin: 0.65rem 0 0;
      color: #cbd5f5;
      font-size: 0.93rem;
    }
    .button-row.wave-buttons {
      width: 100%;
    }
    .waveform-grid {
      display: grid;
      gap: 0.75rem;
      margin-top: 0.85rem;
      grid-template-columns: repeat(auto-fit, minmax(180px, 1fr));
    }
    .waveform-field {
      display: flex;
      flex-direction: column;
      gap: 0.35rem;
    }
    .waveform-field label {
      font-size: 0.86rem;
      color: #cbd5f5;
      letter-spacing: 0.08em;
      text-transform: uppercase;
    }
    .waveform-field input {
      border-radius: 0.75rem;
      border: 1px solid rgba(148, 163, 184, 0.4);
      padding: 0.45rem 0.65rem;
      background: rgba(15, 23, 42, 0.75);
      color: #f8fafc;
      font-size: 0.95rem;
    }
    @media (max-width: 480px) {
      body { padding: 0.75rem; }
      .card { padding: 1.25rem 1rem; border-radius: 1rem; }
      .status-bar { font-size: 0.82rem; gap: 0.5rem; }
    }
  </style>
</head>
<body>
  <div class="card">
    <p class="eyebrow">WEB OSCILLOSCOPE</p>
    <h1>ADC STREAM</h1>
    <p class="subtitle">AD7356 CH A / CH B — calibrated ±2 V range (piecewise linear, cal_20260327).</p>

    <canvas id="graph" width="960" height="420"></canvas>

    <div class="controls">
      <div class="control-group">
        <p class="control-title">Window — X axis</p>
        <div class="button-row">
          <button class="window-btn" data-window-ms="1">1 ms</button>
          <button class="window-btn" data-window-ms="2">2 ms</button>
          <button class="window-btn active" data-window-ms="4">4 ms</button>
          <button class="window-btn" data-window-ms="8">8 ms</button>
        </div>
        <label class="offset-input">
          <span>Custom (ms)</span>
          <input type="number" id="window-custom" min="0.1" max="8.5" step="0.1" value="4.00" />
          <button id="window-custom-set">Set</button>
        </label>
      </div>

      <div class="control-group">
        <p class="control-title">V Range — Y axis</p>
        <div class="button-row">
          <button class="range-btn" data-range="0.5">0.5 V</button>
          <button class="range-btn" data-range="1">1 V</button>
          <button class="range-btn" data-range="2">2 V</button>
          <button class="range-btn active" data-range="4">4 V</button>
        </div>
        <label class="offset-input">
          <span>Custom (V)</span>
          <input type="number" id="range-input" step="0.1" min="0.1" value="4" />
        </label>
      </div>

      <div class="control-group">
        <p class="control-title">Center — Y position</p>
        <div class="button-row">
          <button class="center-btn" data-center="-1">-1 V</button>
          <button class="center-btn" data-center="-0.5">-0.5 V</button>
          <button class="center-btn active" data-center="0">0 V</button>
          <button class="center-btn" data-center="0.5">+0.5 V</button>
          <button class="center-btn" data-center="1">+1 V</button>
        </div>
        <label class="offset-input">
          <span>Manual (V)</span>
          <input type="number" id="center-input" step="0.1" value="0" />
        </label>
      </div>

      <div class="control-group">
        <p class="control-title">Acquisition</p>
        <div class="button-row">
          <button id="toggle-stream">Pause</button>
          <button class="cursor-btn" data-cursor="A">Set Cursor A</button>
          <button class="cursor-btn" data-cursor="B">Set Cursor B</button>
          <button id="clear-cursors">Clear Cursors</button>
        </div>
      </div>

      <div class="control-group">
        <p class="control-title">Trigger</p>
        <div class="button-row">
          <button id="trigger-toggle">Trigger: Off</button>
          <button class="trig-edge-btn active" data-edge="rising">Rising &#x2191;</button>
          <button class="trig-edge-btn" data-edge="falling">Falling &#x2193;</button>
          <button class="trig-ch-btn active" data-ch="0">CH A</button>
          <button class="trig-ch-btn" data-ch="1">CH B</button>
        </div>
        <label class="offset-input">
          <span>Level (V)</span>
          <input type="number" id="trigger-level" step="0.05" value="0.00" />
        </label>
      </div>
    </div>

    <div class="status-bar">
      <span>Last update: <strong id="last-update">--</strong></span>
      <span>Sample window: <strong id="window-label">--</strong></span>
      <span>Range: <strong id="range-display">4.00 V</strong></span>
      <span>Center: <strong id="center-display">0.00 V</strong></span>
      <span>Trigger: <strong id="trigger-status">Off</strong></span>
      <span>Stream: <strong id="stream-status">Running</strong></span>
    </div>

    <div id="legend" class="legend"></div>

    <section class="measurements">
      <div class="measurements-header">
        <h2>Cursor Measurements</h2>
        <p id="cursor-status">Select Cursor A or B, then click the plot to place it.</p>
      </div>
      <div id="cursor-values" class="cursor-values"></div>
      <div id="delta-summary" class="delta-summary">Δt: -- s</div>
    </section>

    <section class="sig-measurements">
      <h2>Signal Measurements</h2>
      <div class="sig-meas-channels" id="sig-meas-channels"></div>
    </section>

    <section class="lab-grid" style="margin-top:1.5rem">
      <div class="panel-header">
        <div>
          <p class="eyebrow">ADS1115 SENSORS</p>
          <h2 class="section-title">External ADC Readings</h2>
        </div>
        <span class="panel-chip">I&#xB2;C 0x48</span>
      </div>
      <div class="meter-grid" style="margin-top:1rem">
        <div class="meter-card">
          <p class="meter-label">Resistance</p>
          <p class="meter-value" id="ads-ohms">--</p>
          <p class="meter-meta" id="ads-raw0">AIN0: --</p>
          <button class="ads-measure-btn" data-ch="0" style="margin-top:0.5rem">Measure</button>
        </div>
        <div class="meter-card">
          <p class="meter-label">Current</p>
          <p class="meter-value" id="ads-amps">--</p>
          <p class="meter-meta" id="ads-raw1">AIN1: --</p>
          <button class="ads-measure-btn" data-ch="1" style="margin-top:0.5rem">Measure</button>
        </div>
        <div class="meter-card">
          <p class="meter-label">DC Voltage</p>
          <p class="meter-value" id="ads-dc">--</p>
          <p class="meter-meta" id="ads-raw2">AIN2&#x2012;3: --</p>
          <button class="ads-measure-btn" data-ch="2" style="margin-top:0.5rem">Measure</button>
        </div>
      </div>
    </section>

    <section class="lab-grid">
      <div class="panel-header">
        <div>
          <p class="eyebrow">INSTRUMENT PANEL</p>
          <h2 class="section-title">Meters & Generators</h2>
          <p class="panel-copy">SPI-ready power supply and waveform generator controls.</p>
        </div>
        <span class="panel-chip">AD9833 + MCP41010</span>
      </div>

      <div class="control-grid">
        <div class="control-panel">
          <div class="panel-title-row">
            <h3>Power Supply</h3>
            <span class="panel-chip">MIC24045</span>
          </div>
          <p class="panel-copy">Set output voltage over I&sup2;C. Valid: 0.640&ndash;3.420&thinsp;V and 4.750&ndash;5.250&thinsp;V.</p>
          <div class="supply-grid">
            <div class="supply-card">
              <div class="supply-heading">Supply A</div>
              <div class="supply-input-row">
                <input id="supply-a-input" type="number" step="0.010" min="0.640" max="5.250" value="1.000" />
                <span class="supply-unit">V</span>
                <button id="supply-a-set">Set</button>
              </div>
              <p class="supply-status" id="supply-a-status">Waiting for input&hellip;</p>
            </div>
          </div>
        </div>
        <div class="control-panel">
          <div class="panel-title-row">
            <h3>Waveform Generator</h3>
            <span class="panel-chip">SPI Controlled</span>
          </div>
          <p class="panel-copy">Select the shape to push to the generator.</p>
          <div class="button-row wave-buttons">
            <button class="waveform-btn active" data-wave="square">Square</button>
            <button class="waveform-btn" data-wave="sine">Sine</button>
            <button class="waveform-btn" data-wave="triangle">Triangle</button>
          </div>
          <div class="waveform-grid">
            <div class="waveform-field">
              <label for="waveform-frequency">Frequency (Hz)</label>
              <input id="waveform-frequency" type="number" min="0.1" step="0.1" value="1000" />
            </div>
            <div class="waveform-field">
              <label for="waveform-amplitude">Amplitude (Vpp)</label>
              <input id="waveform-amplitude" type="number" min="0" max="5" step="0.1" value="1.0" />
            </div>
            <div class="waveform-field">
              <label for="waveform-offset">Offset (V)</label>
              <input id="waveform-offset" type="number" min="-5" max="5" step="0.1" value="0.0" />
            </div>
          </div>
          <p class="waveform-status" id="waveform-status">Waiting for selection…</p>
        </div>
      </div>
    </section>

  </div>

  <script>
    const colors = ["#ff006e", "#00b4d8"];
    const labels = ["CH A", "CH B"];
    const pollIntervalMs  = 50;       // 20 fps display refresh
    const kSampleRateHz   = 60000;    // fixed hardware sample rate
    let maxWindowMs = 4.0;            // capture window in milliseconds
    let maxPoints   = Math.round(maxWindowMs * kSampleRateHz / 1000); // samples
    const series = labels.map(() => []);
    let timestamps = [];
    let displayRange  = 4.0;   // total voltage span shown on Y axis (V)
    let displayCenter = 0.0;   // voltage at vertical centre of plot (V)
    let lastSample = [];
    let isPaused = false;
    let cursorMode = null;
    let triggerEnabled = false;
    let triggerChannel = 0;       // 0 = CH A, 1 = CH B
    let triggerEdge    = "rising"; // "rising" | "falling"
    let triggerLevel   = 0.0;      // voltage threshold (V)
    const cursorPositions = { A: null, B: null }; // { ratio: number } in [0,1]
    let cursorPreview = null; // { key: 'A'|'B', ratio }
    const cursorColors = { A: "#facc15", B: "#38bdf8" };
    let pollHandle = null;
    let dpr = 1;
    const geometry = { left: 60, right: 20, top: 20, bottom: 70, plotWidth: 0, plotHeight: 0, stepX: 0 };

    const canvas = document.getElementById("graph");
    const ctx = canvas.getContext("2d");
    const legendEl = document.getElementById("legend");
    const lastUpdateEl = document.getElementById("last-update");
    const windowLabelEl = document.getElementById("window-label");
    const rangeDisplayEl = document.getElementById("range-display");
    const centerDisplayEl = document.getElementById("center-display");
    const streamStatusEl = document.getElementById("stream-status");
    const windowButtons = document.querySelectorAll(".window-btn");
    const rangeButtons = document.querySelectorAll(".range-btn");
    const rangeInput = document.getElementById("range-input");
    const centerButtons = document.querySelectorAll(".center-btn");
    const centerInput = document.getElementById("center-input");
    const windowCustomInput = document.getElementById("window-custom");
    const windowCustomSetBtn = document.getElementById("window-custom-set");
    const streamBtn = document.getElementById("toggle-stream");
    const cursorButtons = document.querySelectorAll(".cursor-btn");
    const clearCursorsBtn = document.getElementById("clear-cursors");
    const cursorValuesEl = document.getElementById("cursor-values");
    const cursorStatusEl = document.getElementById("cursor-status");
    const deltaSummaryEl = document.getElementById("delta-summary");
    const sigMeasChannelsEl  = document.getElementById("sig-meas-channels");
    const triggerToggleBtn   = document.getElementById("trigger-toggle");
    const triggerLevelInput  = document.getElementById("trigger-level");
    const triggerEdgeButtons = document.querySelectorAll(".trig-edge-btn");
    const triggerChButtons   = document.querySelectorAll(".trig-ch-btn");
    const triggerStatusEl    = document.getElementById("trigger-status");
    const supplyChannels = [
      {
        input:  document.getElementById("supply-a-input"),
        setBtn: document.getElementById("supply-a-set"),
        status: document.getElementById("supply-a-status")
      }
    ];
    const waveformButtons = document.querySelectorAll(".waveform-btn");
    const waveformStatusEl = document.getElementById("waveform-status");
    const waveformFrequencyEl = document.getElementById("waveform-frequency");
    const waveformAmplitudeEl = document.getElementById("waveform-amplitude");
    const waveformOffsetEl = document.getElementById("waveform-offset");
    let activeWaveform = "square";

    function clampRatio(value) {
      if (typeof value !== "number" || Number.isNaN(value)) {
        return null;
      }
      return Math.min(1, Math.max(0, value));
    }

    function sampleCount() {
      return series[0]?.length || 0;
    }

    function pointerRatioFromEvent(event) {
      if (!geometry || geometry.plotWidth <= 0) {
        return null;
      }
      const rect = canvas.getBoundingClientRect();
      // Support both mouse and touch events.
      const src = event.touches        ? event.touches[0]
                : event.changedTouches ? event.changedTouches[0]
                : event;
      // rect and clientX/Y are in CSS pixels; geometry is also in CSS pixels
      // (drawGraph scales the context by dpr so all coordinates are CSS-space).
      const x = src.clientX - rect.left;
      const ratio = (x - geometry.left) / geometry.plotWidth;
      if (ratio < 0 || ratio > 1) {
        return null;
      }
      return clampRatio(ratio);
    }

    function dataPositionFromRatio(ratio, length) {
      if (length === 0 || ratio === null || typeof ratio !== "number") {
        return null;
      }
      if (length === 1) {
        return { lower: 0, upper: 0, fraction: 0 };
      }
      const position = ratio * (length - 1);
      const lower = Math.floor(position);
      const upper = Math.min(length - 1, Math.ceil(position));
      const fraction = position - lower;
      return { lower, upper, fraction };
    }

    function interpolateSeries(seriesValues, ratio) {
      const count = seriesValues.length;
      const pos = dataPositionFromRatio(ratio, count);
      if (!pos) {
        return null;
      }
      if (pos.upper === pos.lower) {
        return seriesValues[pos.lower];
      }
      const lowerValue = seriesValues[pos.lower];
      const upperValue = seriesValues[pos.upper];
      if (typeof lowerValue !== "number" || typeof upperValue !== "number") {
        return null;
      }
      return lowerValue + pos.fraction * (upperValue - lowerValue);
    }

    function valueAtRatio(channelIdx, ratio) {
      if (ratio === null) {
        return null;
      }
      const baseValue = interpolateSeries(series[channelIdx], ratio);
      if (typeof baseValue !== "number") {
        return null;
      }
      return baseValue;
    }

    function timestampAtRatio(ratio) {
      const pos = dataPositionFromRatio(ratio, timestamps.length);
      if (!pos) {
        return null;
      }
      if (pos.upper === pos.lower) {
        return timestamps[pos.lower];
      }
      const lowerTime = timestamps[pos.lower];
      const upperTime = timestamps[pos.upper];
      if (typeof lowerTime !== "number" || typeof upperTime !== "number") {
        return null;
      }
      return lowerTime + pos.fraction * (upperTime - lowerTime);
    }

    function formatCursorTime(ms) {
      if (typeof ms !== "number") {
        return "--";
      }
      const date = new Date(ms);
      const time = date.toLocaleTimeString([], { hour12: false });
      const msPart = String(date.getMilliseconds()).padStart(3, "0");
      return `${time}.${msPart}`;
    }

    function schedulePoll() {
      if (pollHandle) {
        clearTimeout(pollHandle);
      }
      pollHandle = setTimeout(poll, pollIntervalMs);
    }

    function enforceLengthLimit() {
      while (timestamps.length > maxPoints) {
        timestamps.shift();
        series.forEach(channel => channel.shift());
      }
    }

    // Returns the index of the first sample where the signal crosses `level`
    // in the specified direction. Returns -1 if no crossing is found.
    function findTriggerIndex(data, level, edge) {
      for (let i = 1; i < data.length; i++) {
        if (edge === "rising"  && data[i - 1] < level && data[i] >= level) return i - 1;
        if (edge === "falling" && data[i - 1] > level && data[i] <= level) return i - 1;
      }
      return -1;
    }

    // Replace all series data with a fresh burst snapshot.
    // When trigger is active, the data is sliced so the trigger crossing appears
    // at the left edge of the display. If no crossing is found the frame is held.
    // timestamps are real-time offsets so cursor Δt measurements stay accurate.
    function loadBurst(chA, chB) {

      let startIdx = 0;
      if (triggerEnabled) {
        const trigData = triggerChannel === 0 ? chA : chB;
        const idx = findTriggerIndex(trigData, triggerLevel, triggerEdge);
        if (idx < 0) return; // no edge found — hold current frame
        startIdx = idx;
      }

      const sliceA = chA.slice(startIdx, startIdx + maxPoints);
      const sliceB = chB.slice(startIdx, startIdx + maxPoints);
      const n = sliceA.length;
      const intervalMs = 1000 / kSampleRateHz;
      const burstStartMs = Date.now() - (n - 1) * intervalMs;
      timestamps = sliceA.map((_, i) => burstStartMs + i * intervalMs);
      series[0] = sliceA;
      series[1] = sliceB;
      lastSample = [sliceA[sliceA.length - 1], sliceB[sliceB.length - 1]];
      enforceLengthLimit();
      ensureCursorValidity();
    }

    function computeChannelStats(data) {
      if (!data || data.length < 2) return null;
      const n = data.length;
      let max = -Infinity, min = Infinity, sumSq = 0;
      for (let i = 0; i < n; i++) {
        if (data[i] > max) max = data[i];
        if (data[i] < min) min = data[i];
        sumSq += data[i] * data[i];
      }
      const rms = Math.sqrt(sumSq / n);
      const vpp = max - min;
      // Count positive-going crossings of the signal midpoint to estimate frequency.
      const mid = (max + min) / 2;
      let crossings = 0;
      for (let i = 1; i < n; i++) {
        if (data[i - 1] < mid && data[i] >= mid) crossings++;
      }
      const durationSec = (n - 1) / kSampleRateHz;
      const freq = crossings > 0 && durationSec > 0 ? crossings / durationSec : null;
      return { max, min, vpp, rms, freq };
    }

    function renderSignalMeasurements() {
      if (!sigMeasChannelsEl) return;
      const fmtV = v => typeof v === "number" ? v.toFixed(3) + " V" : "--";
      const fmtHz = v => {
        if (v === null || v === undefined) return "--";
        if (v >= 1000) return (v / 1000).toFixed(2) + " kHz";
        return v.toFixed(1) + " Hz";
      };
      sigMeasChannelsEl.innerHTML = labels.map((label, idx) => {
        const stats = computeChannelStats(series[idx]);
        const color = colors[idx];
        const rows = [
          ["Vpp",  stats ? fmtV(stats.vpp)   : "--"],
          ["RMS",  stats ? fmtV(stats.rms)   : "--"],
          ["Max",  stats ? fmtV(stats.max)   : "--"],
          ["Min",  stats ? fmtV(stats.min)   : "--"],
          ["Freq", stats ? fmtHz(stats.freq) : "--"],
        ];
        return `<div class="sig-meas-card">
          <h3 style="color:${color}">${label}</h3>
          ${rows.map(([l, v]) =>
            `<div class="sig-meas-row"><span class="sig-meas-label">${l}</span><span class="sig-meas-value">${v}</span></div>`
          ).join("")}
        </div>`;
      }).join("");
    }

    function updateLegend() {
      const values = lastSample.length ? lastSample : labels.map(() => null);
      legendEl.innerHTML = values
        .map(
          (value, index) => `
            <span class="legend-item">
              <span class="legend-color" style="background:${colors[index]}"></span>
              ${labels[index]}: ${
                typeof value === "number" ? value.toFixed(3) + " V" : "--"
              }
            </span>`
        )
        .join("");
      if (timestamps.length) {
        lastUpdateEl.textContent = new Date(timestamps[timestamps.length - 1]).toLocaleTimeString();
      } else {
        lastUpdateEl.textContent = "--";
      }
    }

    function drawGraph() {
      // All drawing is done in CSS pixel space; ctx is pre-scaled by dpr.
      const cssW = canvas.width / dpr;
      const cssH = canvas.height / dpr;
      const isNarrow = cssW < 420;

      // Update adaptive margins so pointerRatioFromEvent stays in sync.
      geometry.left   = isNarrow ? 44 : 58;
      geometry.right  = 12;
      geometry.top    = 16;
      geometry.bottom = isNarrow ? 40 : 56;

      const left   = geometry.left;
      const right  = geometry.right;
      const top    = geometry.top;
      const bottom = geometry.bottom;
      geometry.plotWidth  = cssW - left - right;
      geometry.plotHeight = cssH - top - bottom;

      ctx.save();
      ctx.scale(dpr, dpr);
      ctx.clearRect(0, 0, cssW, cssH);

      const minVoltage    = displayCenter - displayRange / 2;
      const maxVoltage    = displayCenter + displayRange / 2;
      const verticalRange = displayRange;
      const axisFont      = (isNarrow ? "11px" : "13px") + " 'Segoe UI', sans-serif";

      ctx.strokeStyle = "rgba(226, 232, 240, 0.2)";
      ctx.fillStyle = "#cbd5f5";
      ctx.font = axisFont;
      ctx.textAlign = "right";
      ctx.textBaseline = "middle";
      const divisions = 4;
      for (let i = 0; i <= divisions; i++) {
        const ratio = i / divisions;
        const y = top + ratio * geometry.plotHeight;
        ctx.beginPath();
        ctx.moveTo(left, y);
        ctx.lineTo(left + geometry.plotWidth, y);
        ctx.stroke();
        const valueLabel = (maxVoltage - ratio * verticalRange).toFixed(1);
        ctx.fillText(valueLabel + " V", left - 4, y);
      }

      // X-axis time labels.
      {
        const windowMs     = (maxPoints - 1) * 1000 / kSampleRateHz;
        const tickInterval = calcNiceInterval(windowMs, isNarrow ? 3 : 5);
        const tickY        = top + geometry.plotHeight;
        ctx.strokeStyle    = "rgba(226, 232, 240, 0.35)";
        ctx.fillStyle      = "#cbd5f5";
        ctx.font           = axisFont;
        ctx.textAlign      = "center";
        ctx.textBaseline   = "top";
        ctx.lineWidth      = 1;
        // Guard against infinite loop when windowMs ≈ 0.
        const maxTicks = 20;
        let tMs = 0;
        for (let tick = 0; tick <= maxTicks && tMs <= windowMs * 1.001; tick++) {
          const x   = left + (windowMs > 0 ? tMs / windowMs : 0) * geometry.plotWidth;
          ctx.beginPath();
          ctx.moveTo(x, tickY);
          ctx.lineTo(x, tickY + 4);
          ctx.stroke();
          const lbl = tMs === 0 ? "0"
                    : tMs < 1   ? (tMs * 1000).toFixed(0) + "\u00b5s"
                    :             tMs.toFixed(tMs < 10 ? 2 : 1) + " ms";
          ctx.fillText(lbl, x, tickY + 6);
          tMs = +(tMs + tickInterval).toFixed(12);
        }
      }

      const pointCount = sampleCount();
      geometry.stepX = pointCount > 1 ? geometry.plotWidth / (pointCount - 1) : geometry.plotWidth;

      if (pointCount >= 2) {
        series.forEach((data, idx) => {
          if (data.length < 2) return;
          ctx.beginPath();
          ctx.lineWidth = 1.5;
          ctx.strokeStyle = colors[idx];
          data.forEach((value, pointIdx) => {
            const normalized = (value - minVoltage) / verticalRange;
            const clamped    = Math.min(1, Math.max(0, normalized));
            const x = left + pointIdx * geometry.stepX;
            const y = top + (1 - clamped) * geometry.plotHeight;
            if (pointIdx === 0) ctx.moveTo(x, y); else ctx.lineTo(x, y);
          });
          ctx.stroke();
        });
      } else if (pointCount === 1) {
        // Render single-point indicators when only one sample is present.
        series.forEach((data, idx) => {
          if (!data.length) return;
          const normalized = (data[0] - minVoltage) / verticalRange;
          const clamped    = Math.min(1, Math.max(0, normalized));
          const x = left;
          const y = top + (1 - clamped) * geometry.plotHeight;
          ctx.fillStyle = colors[idx];
          ctx.beginPath();
          ctx.arc(x, y, 3, 0, Math.PI * 2);
          ctx.fill();
        });
      }

      drawCursorOverlay("A", cursorPositions.A?.ratio, false, isNarrow);
      drawCursorOverlay("B", cursorPositions.B?.ratio, false, isNarrow);
      if (cursorPreview && cursorMode === cursorPreview.key) {
        drawCursorOverlay(cursorPreview.key, cursorPreview.ratio, true, isNarrow);
      }

      // Trigger level line — amber dashed, drawn over signals and cursors.
      if (triggerEnabled) {
        const tMin  = displayCenter - displayRange / 2;
        const tNorm = (triggerLevel - tMin) / displayRange;
        const tY    = top + (1 - Math.min(1, Math.max(0, tNorm))) * geometry.plotHeight;
        ctx.save();
        ctx.strokeStyle = "#fbbf24";
        ctx.lineWidth   = 1;
        ctx.setLineDash([5, 4]);
        ctx.globalAlpha = 0.9;
        ctx.beginPath();
        ctx.moveTo(left, tY);
        ctx.lineTo(left + geometry.plotWidth, tY);
        ctx.stroke();
        ctx.setLineDash([]);
        ctx.globalAlpha = 1;
        const arrow = triggerEdge === "rising" ? "\u2191" : "\u2193";
        ctx.fillStyle = "#fbbf24";
        ctx.font = (isNarrow ? "10px" : "11px") + " 'Segoe UI', sans-serif";
        ctx.textAlign = "right";
        ctx.textBaseline = "bottom";
        ctx.fillText(arrow + " " + triggerLevel.toFixed(2) + " V", left + geometry.plotWidth - 4, tY - 2);
        ctx.restore();
      }

      ctx.restore();
    }

    function drawCursorOverlay(key, ratio, isPreview, isNarrow) {
      const safeRatio = clampRatio(ratio);
      if (safeRatio === null || geometry.plotWidth <= 0) {
        return;
      }
      const color   = cursorColors[key] || "#fff";
      const x       = geometry.left + safeRatio * geometry.plotWidth;
      const yBottom = geometry.top + geometry.plotHeight;
      if (isPreview) {
        ctx.setLineDash([6, 4]);
        ctx.globalAlpha = 0.5;
      } else {
        ctx.setLineDash([8, 6]);
        ctx.globalAlpha = 1;
      }
      ctx.strokeStyle = color;
      ctx.lineWidth = 1.5;
      ctx.beginPath();
      ctx.moveTo(x, geometry.top);
      ctx.lineTo(x, yBottom);
      ctx.stroke();
      ctx.setLineDash([]);
      ctx.fillStyle = color;
      ctx.globalAlpha = isPreview ? 0.7 : 1;
      ctx.font = (isNarrow ? "10px" : "12px") + " 'Segoe UI', sans-serif";
      const label = isPreview ? `${key}?` : key;
      ctx.fillText(label, x + 5, geometry.top + 8);
    }

    function resizeCanvas() {
      dpr = window.devicePixelRatio || 1;
      canvas.width  = Math.round(canvas.offsetWidth  * dpr);
      canvas.height = Math.round(canvas.offsetHeight * dpr);
      drawGraph();
    }

    function ensureCursorValidity() {
      ["A", "B"].forEach(key => {
        const entry = cursorPositions[key];
        if (!entry || typeof entry.ratio !== "number") {
          cursorPositions[key] = null;
          return;
        }
        const clamped = clampRatio(entry.ratio);
        if (clamped === null) {
          cursorPositions[key] = null;
        } else {
          entry.ratio = clamped;
        }
      });
    }

    function formatValue(value) {
      return typeof value === "number" ? value.toFixed(2) + " V" : "--";
    }

    function getValueForCursor(channelIdx, key) {
      const ratio = cursorPositions[key]?.ratio ?? null;
      return valueAtRatio(channelIdx, ratio);
    }

    function renderMeasurements() {
      const ratioA = cursorPositions.A?.ratio ?? null;
      const ratioB = cursorPositions.B?.ratio ?? null;

      if (
        cursorMode &&
        cursorPreview &&
        cursorPreview.key === cursorMode &&
        typeof cursorPreview.ratio === "number"
      ) {
        const previewTime = timestampAtRatio(cursorPreview.ratio);
        cursorStatusEl.textContent = `Cursor ${cursorMode} preview @ ${formatCursorTime(
          previewTime
        )}. Click to confirm.`;
      } else if (cursorMode) {
        cursorStatusEl.textContent = `Move the pointer over the plot, then click to place Cursor ${cursorMode}.`;
      } else if (!cursorPositions.A && !cursorPositions.B) {
        cursorStatusEl.textContent = "Select Cursor A or B, then click the plot to place it.";
      } else {
        const parts = [];
        ["A", "B"].forEach(key => {
          const ratio = cursorPositions[key]?.ratio;
          if (ratio !== null && ratio !== undefined) {
            const ts = timestampAtRatio(ratio);
            parts.push(`Cursor ${key} @ ${formatCursorTime(ts)}`);
          }
        });
        cursorStatusEl.textContent = parts.join(" • ");
      }

      cursorValuesEl.innerHTML = labels
        .map((label, idx) => {
          const valueA = getValueForCursor(idx, "A");
          const valueB = getValueForCursor(idx, "B");
          const delta = valueA !== null && valueB !== null ? valueB - valueA : null;
          return `
            <div class="cursor-card">
              <h3>${label}</h3>
              <p>A: ${formatValue(valueA)}</p>
              <p>B: ${formatValue(valueB)}</p>
              <p>ΔV: ${formatValue(delta)}</p>
            </div>
          `;
        })
        .join("");

      if (ratioA !== null && ratioA !== undefined && ratioB !== null && ratioB !== undefined) {
        const timeA = timestampAtRatio(ratioA);
        const timeB = timestampAtRatio(ratioB);
        if (typeof timeA === "number" && typeof timeB === "number") {
          const deltaMs = Math.abs(timeB - timeA);
          const deltaSeconds = deltaMs / 1000;
          const frequency = deltaSeconds > 0 ? 1 / deltaSeconds : null;
          deltaSummaryEl.innerHTML = `<strong>Δt:</strong> ${deltaSeconds.toFixed(3)} s ${
            frequency ? `(<span>${frequency.toFixed(2)} Hz</span>)` : ""
          }`;
        } else {
          deltaSummaryEl.textContent = "Δt: -- s";
        }
      } else {
        deltaSummaryEl.textContent = "Δt: -- s";
      }
    }

    // Returns a "nice" round tick interval for a given data range and max tick count.
    function calcNiceInterval(range, maxDivisions) {
      if (range <= 0 || maxDivisions <= 0) return 1;
      const raw  = range / maxDivisions;
      const mag  = Math.pow(10, Math.floor(Math.log10(raw)));
      const norm = raw / mag;
      const nice = norm < 1.5 ? 1 : norm < 3.5 ? 2 : norm < 7.5 ? 5 : 10;
      return nice * mag;
    }

    function updateWindowLabel() {
      const actualMs = (maxPoints - 1) * 1000 / kSampleRateHz;
      windowLabelEl.textContent =
        `${actualMs.toFixed(2)} ms @ ${(kSampleRateHz / 1000).toFixed(0)} kHz (${maxPoints} samples)`;
    }

    function setSampleWindowMs(ms) {
      const clamped = Math.max(0.1, Math.min(8.5, ms));
      maxWindowMs = clamped;
      maxPoints   = Math.max(2, Math.min(512, Math.round(clamped * kSampleRateHz / 1000)));
      if (windowCustomInput) windowCustomInput.value = clamped.toFixed(2);
      windowButtons.forEach(btn =>
        btn.classList.toggle("active", Math.abs(Number(btn.dataset.windowMs) - clamped) < 0.01)
      );
      updateWindowLabel();
      enforceLengthLimit();
      ensureCursorValidity();
      drawGraph();
      renderMeasurements();
    }

    function setDisplayRange(value) {
      const v = Math.max(0.1, value);
      displayRange = v;
      if (rangeInput) rangeInput.value = v.toFixed(2);
      rangeButtons.forEach(btn => btn.classList.toggle("active", Number(btn.dataset.range) === v));
      if (rangeDisplayEl) rangeDisplayEl.textContent = v.toFixed(2) + " V";
      updateLegend();
      drawGraph();
      renderMeasurements();
    }

    function setDisplayCenter(value) {
      displayCenter = value;
      if (centerInput) centerInput.value = value.toFixed(2);
      centerButtons.forEach(btn => btn.classList.toggle("active", Number(btn.dataset.center) === value));
      if (centerDisplayEl) centerDisplayEl.textContent = value.toFixed(2) + " V";
      updateLegend();
      drawGraph();
      renderMeasurements();
    }

    function updateStreamState() {
      streamBtn.textContent = isPaused ? "Resume" : "Pause";
      streamStatusEl.textContent = isPaused ? "Paused" : "Running";
    }

    function updateTriggerUI() {
      triggerToggleBtn.textContent = triggerEnabled ? "Trigger: On" : "Trigger: Off";
      triggerToggleBtn.classList.toggle("active", triggerEnabled);
      if (triggerStatusEl) {
        if (triggerEnabled) {
          const arrow = triggerEdge === "rising" ? "\u2191" : "\u2193";
          triggerStatusEl.textContent = `${labels[triggerChannel]} ${arrow} ${triggerLevel.toFixed(2)} V`;
        } else {
          triggerStatusEl.textContent = "Off";
        }
      }
      drawGraph();
    }

    function setCursorMode(mode) {
      cursorMode = mode;
      cursorPreview = null;
      // Disable browser scroll/pan on canvas while a cursor is being placed
      // so that touch drags register as cursor moves rather than page scrolls.
      canvas.style.touchAction = mode ? "none" : "";
      cursorButtons.forEach(btn => btn.classList.toggle("active", btn.dataset.cursor === mode));
      renderMeasurements();
      drawGraph();
    }

    function supplyVoltageInGap(v) {
      return v > 3.420 && v < 4.750;
    }

    async function sendSupplyVoltage(channel, volts) {
      if (channel.status) channel.status.textContent = "Setting\u2026";
      try {
        const response = await fetch(`/power?voltage=${volts.toFixed(3)}`, { method: "POST" });
        const payload = await response.json().catch(() => ({}));
        if (!response.ok) {
          if (channel.status) channel.status.textContent = payload.error || "Error from device.";
          return;
        }
        if (channel.input && typeof payload.voltage === "number") {
          channel.input.value = payload.voltage.toFixed(3);
        }
        const v = typeof payload.voltage === "number" ? payload.voltage.toFixed(3) : "?";
        let msg = `Set to ${v} V`;
        if (payload.powerGood === false) msg += " \u26A0 Not power good";
        if (payload.enabled === false)   msg += " \u26A0 EN pin low";
        if (payload.overcurrent)         msg += " \u26A0 Overcurrent";
        if (payload.thermalWarning)      msg += " \u26A0 Thermal warning";
        if (payload.thermalShutdown)     msg += " \u26A0 Thermal shutdown";
        if (channel.status) channel.status.textContent = msg;
      } catch (err) {
        console.error("Power update failed", err);
        if (channel.status) channel.status.textContent = "Update failed; retry.";
      }
    }

    function bindSupplyChannel(channel) {
      if (!channel.input || !channel.setBtn) return;
      const apply = () => {
        const v = parseFloat(channel.input.value);
        if (Number.isNaN(v)) {
          if (channel.status) channel.status.textContent = "Enter a valid voltage.";
          return;
        }
        if (v < 0.640 || v > 5.250) {
          if (channel.status) channel.status.textContent = "Out of range. Valid: 0.640\u20133.420 V or 4.750\u20135.250 V.";
          return;
        }
        if (supplyVoltageInGap(v)) {
          if (channel.status) channel.status.textContent = "Invalid gap \u2014 no codes between 3.420 and 4.750 V.";
          return;
        }
        sendSupplyVoltage(channel, v);
      };
      channel.setBtn.addEventListener("click", apply);
      channel.input.addEventListener("keydown", e => { if (e.key === "Enter") apply(); });
    }

    function updateWaveformButtons(target) {
      waveformButtons.forEach(btn => btn.classList.toggle("active", btn.dataset.wave === target));
    }

    function clampWaveformFrequency(value) {
      if (typeof value !== "number" || Number.isNaN(value)) return 1000;
      return Math.min(200000, Math.max(0.1, value));
    }

    function clampWaveformAmplitude(value) {
      if (typeof value !== "number" || Number.isNaN(value)) return 1;
      return Math.min(5, Math.max(0, value));
    }

    function clampWaveformOffset(value) {
      if (typeof value !== "number" || Number.isNaN(value)) return 0;
      return Math.min(5, Math.max(-5, value));
    }

    function getWaveformPayload() {
      const frequency = clampWaveformFrequency(parseFloat(waveformFrequencyEl?.value));
      const amplitude = clampWaveformAmplitude(parseFloat(waveformAmplitudeEl?.value));
      const offset = clampWaveformOffset(parseFloat(waveformOffsetEl?.value));
      if (waveformFrequencyEl) waveformFrequencyEl.value = frequency;
      if (waveformAmplitudeEl) waveformAmplitudeEl.value = amplitude;
      if (waveformOffsetEl) waveformOffsetEl.value = offset;
      return { frequency, amplitude, offset };
    }

    async function setWaveform(type) {
      activeWaveform = type;
      updateWaveformButtons(type);
      if (waveformStatusEl) {
        waveformStatusEl.textContent = "Sending waveform command...";
      }
      try {
        const payload = getWaveformPayload();
        const params = new URLSearchParams({
          type,
          frequency: payload.frequency.toString(),
          amplitude: payload.amplitude.toString(),
          offset: payload.offset.toString()
        });
        const res = await fetch(`/waveform?${params.toString()}`, { method: "POST" });
        const data = await res.json().catch(() => ({}));
        const confirmedCfg = data.waveform || {};
        const confirmedMode = confirmedCfg.mode || type;
        updateWaveformButtons(confirmedMode);
        if (waveformStatusEl) {
          waveformStatusEl.textContent = `Waveform set to ${confirmedMode}`;
        }
        if (typeof confirmedCfg.frequency === "number" && waveformFrequencyEl) {
          waveformFrequencyEl.value = clampWaveformFrequency(confirmedCfg.frequency);
        }
        if (typeof confirmedCfg.amplitude === "number" && waveformAmplitudeEl) {
          waveformAmplitudeEl.value = clampWaveformAmplitude(confirmedCfg.amplitude);
        }
        if (typeof confirmedCfg.offset === "number" && waveformOffsetEl) {
          waveformOffsetEl.value = clampWaveformOffset(confirmedCfg.offset);
        }
      } catch (error) {
        console.error("Waveform request failed", error);
        if (waveformStatusEl) {
          waveformStatusEl.textContent = "Waveform update failed.";
        }
      }
    }

    function applyControlState(state) {
      if (state && Array.isArray(state.supplies)) {
        const v = parseFloat(state.supplies[0]);
        supplyChannels.forEach(channel => {
          if (channel.input && !Number.isNaN(v) && v > 0) {
            channel.input.value = v.toFixed(3);
            if (channel.status) channel.status.textContent = `Last set: ${v.toFixed(3)} V`;
          }
        });
      }
      if (state && state.waveform) {
        activeWaveform = state.waveform.mode || activeWaveform;
        updateWaveformButtons(activeWaveform);
        if (waveformStatusEl) {
          waveformStatusEl.textContent = `Waveform set to ${activeWaveform}`;
        }
        if (waveformFrequencyEl && typeof state.waveform.frequency === "number") {
          waveformFrequencyEl.value = clampWaveformFrequency(state.waveform.frequency);
        }
        if (waveformAmplitudeEl && typeof state.waveform.amplitude === "number") {
          waveformAmplitudeEl.value = clampWaveformAmplitude(state.waveform.amplitude);
        }
        if (waveformOffsetEl && typeof state.waveform.offset === "number") {
          waveformOffsetEl.value = clampWaveformOffset(state.waveform.offset);
        }
      }
    }

    async function fetchControlState() {
      try {
        const response = await fetch("/control-state");
        if (!response.ok) {
          throw new Error("Bad status");
        }
        const state = await response.json();
        applyControlState(state);
      } catch (error) {
        console.warn("Unable to load control state", error);
      }
    }

    function initBenchControls() {
      supplyChannels.forEach(bindSupplyChannel);
      waveformButtons.forEach(btn =>
        btn.addEventListener("click", () => setWaveform(btn.dataset.wave))
      );
      [waveformFrequencyEl, waveformAmplitudeEl, waveformOffsetEl].forEach(input => {
        if (!input) return;
        input.addEventListener("change", () => setWaveform(activeWaveform));
      });
      fetchControlState();
    }

    canvas.addEventListener("mousemove", event => {
      if (!cursorMode) {
        if (cursorPreview) {
          cursorPreview = null;
          drawGraph();
          renderMeasurements();
        }
        return;
      }
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) {
        if (cursorPreview) {
          cursorPreview = null;
          drawGraph();
          renderMeasurements();
        }
        return;
      }
      cursorPreview = { key: cursorMode, ratio };
      drawGraph();
      renderMeasurements();
    });

    canvas.addEventListener("mouseleave", () => {
      if (cursorPreview) {
        cursorPreview = null;
        drawGraph();
        renderMeasurements();
      }
    });

    canvas.addEventListener("click", event => {
      if (!cursorMode) {
        return;
      }
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) {
        return;
      }
      cursorPositions[cursorMode] = { ratio };
      cursorPreview = null;
      setCursorMode(null);
    });

    // Touch support for cursor placement on mobile.
    canvas.addEventListener("touchstart", event => {
      if (!cursorMode) return;
      event.preventDefault();
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) return;
      cursorPreview = { key: cursorMode, ratio };
      drawGraph();
      renderMeasurements();
    }, { passive: false });

    canvas.addEventListener("touchmove", event => {
      if (!cursorMode) return;
      event.preventDefault();
      const ratio = pointerRatioFromEvent(event);
      if (ratio === null) {
        if (cursorPreview) { cursorPreview = null; drawGraph(); renderMeasurements(); }
        return;
      }
      cursorPreview = { key: cursorMode, ratio };
      drawGraph();
      renderMeasurements();
    }, { passive: false });

    canvas.addEventListener("touchend", event => {
      if (!cursorMode) return;
      event.preventDefault();
      // event.touches is empty on touchend so pointerRatioFromEvent returns null.
      // Use the last preview position set by touchstart/touchmove instead.
      const ratio = cursorPreview?.ratio ?? null;
      if (ratio === null) return;
      cursorPositions[cursorMode] = { ratio };
      cursorPreview = null;
      setCursorMode(null);
    }, { passive: false });

    windowButtons.forEach(btn =>
      btn.addEventListener("click", () => setSampleWindowMs(Number(btn.dataset.windowMs)))
    );

    if (windowCustomSetBtn) {
      windowCustomSetBtn.addEventListener("click", () => {
        const ms = parseFloat(windowCustomInput?.value);
        if (!Number.isNaN(ms)) setSampleWindowMs(ms);
      });
    }
    if (windowCustomInput) {
      windowCustomInput.addEventListener("keydown", e => {
        if (e.key === "Enter") {
          const ms = parseFloat(windowCustomInput.value);
          if (!Number.isNaN(ms)) setSampleWindowMs(ms);
        }
      });
    }

    rangeButtons.forEach(btn =>
      btn.addEventListener("click", () => setDisplayRange(Number(btn.dataset.range)))
    );
    if (rangeInput) {
      rangeInput.addEventListener("input", () => {
        const v = parseFloat(rangeInput.value);
        if (!Number.isNaN(v) && v > 0) setDisplayRange(v);
      });
    }

    centerButtons.forEach(btn =>
      btn.addEventListener("click", () => setDisplayCenter(Number(btn.dataset.center)))
    );
    if (centerInput) {
      centerInput.addEventListener("input", () => {
        const v = parseFloat(centerInput.value);
        if (!Number.isNaN(v)) setDisplayCenter(v);
      });
    }

    streamBtn.addEventListener("click", () => {
      isPaused = !isPaused;
      updateStreamState();
      if (!isPaused) {
        schedulePoll();
      }
    });

    cursorButtons.forEach(btn =>
      btn.addEventListener("click", () => {
        const target = btn.dataset.cursor;
        setCursorMode(cursorMode === target ? null : target);
      })
    );

    clearCursorsBtn.addEventListener("click", () => {
      cursorPositions.A = null;
      cursorPositions.B = null;
      cursorPreview = null;
      setCursorMode(null);
    });

    triggerToggleBtn.addEventListener("click", () => {
      triggerEnabled = !triggerEnabled;
      updateTriggerUI();
    });

    triggerEdgeButtons.forEach(btn =>
      btn.addEventListener("click", () => {
        triggerEdge = btn.dataset.edge;
        triggerEdgeButtons.forEach(b => b.classList.toggle("active", b.dataset.edge === triggerEdge));
        updateTriggerUI();
      })
    );

    triggerChButtons.forEach(btn =>
      btn.addEventListener("click", () => {
        triggerChannel = Number(btn.dataset.ch);
        triggerChButtons.forEach(b => b.classList.toggle("active", Number(b.dataset.ch) === triggerChannel));
        updateTriggerUI();
      })
    );

    if (triggerLevelInput) {
      triggerLevelInput.addEventListener("input", () => {
        const v = parseFloat(triggerLevelInput.value);
        if (!Number.isNaN(v)) {
          triggerLevel = v;
          updateTriggerUI();
        }
      });
    }

    async function poll() {
      if (isPaused) {
        schedulePoll();
        return;
      }
      try {
        // When trigger is active, request up to 2× maxPoints so there is enough
        // pre-trigger data to reliably find the crossing edge.
        const burstN = triggerEnabled ? Math.min(512, maxPoints * 2) : maxPoints;
        const response = await fetch(`/adc/burst?n=${burstN}`);
        const payload = await response.json();
        if (Array.isArray(payload.chA) && Array.isArray(payload.chB)) {
          loadBurst(payload.chA, payload.chB);
          updateWindowLabel();
          updateLegend();
          renderMeasurements();
          renderSignalMeasurements();
          drawGraph();
        }
      } catch (error) {
        console.error("ADC fetch failed", error);
      } finally {
        schedulePoll();
      }
    }

    // ADS1115 manual measurement — one channel at a time, on button press.
    // ch=0: AIN0 Resistance, ch=1: AIN1 Current, ch=2: AIN2-3 Diff DC Voltage
    const adsValueEls = [
      document.getElementById("ads-ohms"),
      document.getElementById("ads-amps"),
      document.getElementById("ads-dc"),
    ];
    const adsRawEls = [
      document.getElementById("ads-raw0"),
      document.getElementById("ads-raw1"),
      document.getElementById("ads-raw2"),
    ];
    const adsUnits  = ["\u03A9", "A", "V"];
    const adsLabels = ["AIN0", "AIN1", "AIN2\u20123"];

    document.querySelectorAll(".ads-measure-btn").forEach(btn => {
      btn.addEventListener("click", async () => {
        const ch = parseInt(btn.dataset.ch, 10);
        const valueEl = adsValueEls[ch];
        const rawEl   = adsRawEls[ch];
        btn.disabled = true;
        btn.textContent = "Measuring\u2026";
        if (valueEl) valueEl.textContent = "\u2026";
        try {
          const response = await fetch(`/ads/measure?ch=${ch}`);
          if (!response.ok) throw new Error("Bad status");
          const data = await response.json();
          if (valueEl) {
            if (ch === 0) {
              if (data.isOC) {
                valueEl.textContent = "O.C.";
              } else {
                valueEl.textContent = data.value >= 1000
                  ? (data.value / 1000).toFixed(2) + " k\u03A9"
                  : data.value.toFixed(1) + " \u03A9";
              }
            } else {
              valueEl.textContent = data.value.toFixed(3) + " " + adsUnits[ch];
            }
          }
          if (rawEl) rawEl.textContent = adsLabels[ch] + ": " + data.rawAvg.toFixed(1);
        } catch (e) {
          console.warn("ADS measure failed", e);
          if (valueEl) valueEl.textContent = "Error";
        }
        btn.disabled = false;
        btn.textContent = "Measure";
      });
    });

    function init() {
      if (rangeDisplayEl)  rangeDisplayEl.textContent  = displayRange.toFixed(2) + " V";
      if (centerDisplayEl) centerDisplayEl.textContent = displayCenter.toFixed(2) + " V";
      resizeCanvas();   // sets dpr, sizes canvas buffer, draws first frame
      updateLegend();
      updateWindowLabel();
      updateStreamState();
      updateTriggerUI();
      renderMeasurements();
      renderSignalMeasurements();
      initBenchControls();
      poll();
    }

    window.addEventListener("resize", resizeCanvas);
    init();
  </script>
</body>
</html>
)HTML";
