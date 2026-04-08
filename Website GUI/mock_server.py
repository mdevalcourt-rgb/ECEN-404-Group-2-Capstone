#!/usr/bin/env python3
"""
MADAM3 mock dev server.
Serves the web GUI directly from WebContent.cpp and provides fake API endpoints.

Usage:
    python mock_server.py
    Open: http://localhost:8080/
"""

import http.server
import json
import math
import random
import re
import urllib.parse
from pathlib import Path

PORT = 8080
WEBCONTENT = Path(__file__).parent / "src" / "WebContent.cpp"


# ---------------------------------------------------------------------------
# Extract HTML from the PROGMEM raw string in WebContent.cpp
# ---------------------------------------------------------------------------
def extract_html() -> str:
    src = WEBCONTENT.read_text(encoding="utf-8")
    match = re.search(r'R"HTML\((.*?)\)HTML"', src, re.DOTALL)
    if not match:
        raise RuntimeError(f'Could not find R"HTML(...)HTML" block in {WEBCONTENT}')
    return match.group(1)


HTML: str = extract_html()


# ---------------------------------------------------------------------------
# Shared mutable state
# ---------------------------------------------------------------------------

# Dev config — tweaked live via /dev/config POST from the in-page panel.
dev_config: dict = {
    "scope": {
        "chA": {"shape": "sine",     "frequency": 1000.0, "amplitude": 1.5, "phase":  0.0},
        "chB": {"shape": "sine",     "frequency": 1000.0, "amplitude": 1.0, "phase": 90.0},
        "noise": 0.02,
    },
    "dmm": {
        "voltage":    3.300,
        "current":    0.150,
        "resistance": 470.0,
    },
    "psu": {
        "voltage": 3.300,
    },
}

# Mirrors what the ESP32 tracks after /waveform POST commands.
waveform_state: dict = {
    "mode": "square", "frequency": 1000.0,
    "amplitude": 1.0, "offset": 0.0, "phase": 0.0,
}


# ---------------------------------------------------------------------------
# Waveform generation helpers
# ---------------------------------------------------------------------------

def generate_channel(cfg: dict, n: int, period_us: float, noise_v: float) -> list:
    """Generate N calibrated voltage samples (floats), matching the format that
    the ESP32 firmware sends — applyAdcCalibration() is applied server-side there,
    so the JS receives and plots voltages directly."""
    shape     = cfg.get("shape", "sine")
    freq      = max(0.01, float(cfg.get("frequency", 1000.0)))
    amp       = float(cfg.get("amplitude", 1.0))
    phase_rad = math.radians(float(cfg.get("phase", 0.0)))
    dt        = period_us * 1e-6  # seconds per sample

    out = []
    for i in range(n):
        theta = 2.0 * math.pi * freq * i * dt + phase_rad
        if shape == "square":
            v = amp * (1.0 if math.sin(theta) >= 0.0 else -1.0)
        elif shape == "triangle":
            v = amp * (2.0 / math.pi) * math.asin(math.sin(theta))
        else:  # sine (default)
            v = amp * math.sin(theta)

        if noise_v > 0.0:
            v += random.gauss(0.0, noise_v)

        out.append(round(max(-2.015, min(2.058, v)), 3))

    return out


# ---------------------------------------------------------------------------
# HTTP request handler
# ---------------------------------------------------------------------------

class Handler(http.server.BaseHTTPRequestHandler):

    # Silence the default per-request log line; print our own minimal version.
    def log_message(self, fmt, *args):
        print(f"  {self.command} {self.path.split('?')[0]}")

    # ── response helpers ────────────────────────────────────────────────────

    def _cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def send_json(self, data, status: int = 200):
        body = json.dumps(data).encode()
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self._cors()
        self.end_headers()
        self.wfile.write(body)

    def send_html(self, html: str):
        body = html.encode()
        self.send_response(200)
        self.send_header("Content-Type", "text/html; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def send_empty(self, status: int):
        self.send_response(status)
        self._cors()
        self.end_headers()

    # ── request parsing helpers ─────────────────────────────────────────────

    def _query(self) -> dict:
        return dict(urllib.parse.parse_qsl(urllib.parse.urlparse(self.path).query))

    def _path(self) -> str:
        return urllib.parse.urlparse(self.path).path

    def _body_json(self) -> dict:
        length = int(self.headers.get("Content-Length", 0))
        raw = self.rfile.read(length).decode() if length else ""
        try:
            return json.loads(raw)
        except Exception:
            return dict(urllib.parse.parse_qsl(raw))

    # ── GET routes ──────────────────────────────────────────────────────────

    def do_GET(self):
        p = self._path()

        if p in ("/", "/index.html"):
            self.send_html(HTML)

        elif p == "/adc/burst":
            q   = self._query()
            n   = max(1, min(512, int(q.get("n", 100))))
            pus = max(1.0, float(q.get("period_us", 17.0)))
            cfg = dev_config["scope"]
            noise = float(cfg.get("noise", 0.02))
            self.send_json({
                "chA": generate_channel(cfg["chA"], n, pus, noise),
                "chB": generate_channel(cfg["chB"], n, pus, noise),
            })

        elif p == "/ads/measure":
            ch  = int(self._query().get("ch", 0))
            dmm = dev_config["dmm"]
            if ch == 0:   # resistance
                self.send_json({"value": dmm["resistance"], "rawAvg": 1234.0, "isOC": False})
            elif ch == 1: # current
                self.send_json({"value": dmm["current"], "rawAvg": 2048.0})
            else:         # dc voltage (ch=2, AIN2-3 differential)
                self.send_json({"value": dmm["voltage"], "rawAvg": 1800.0})

        elif p == "/control-state":
            self.send_json({
                "supplies": [dev_config["psu"]["voltage"]],
                "waveform": waveform_state,
            })

        elif p == "/dev/config":
            self.send_json(dev_config)

        else:
            self.send_empty(404)

    # ── POST routes ─────────────────────────────────────────────────────────

    def do_POST(self):
        p = self._path()

        if p == "/waveform":
            # JS sends params as URL query string on a POST.
            q = self._query()
            if q.get("type"):
                waveform_state["mode"]      = q["type"]
            if q.get("frequency"):
                waveform_state["frequency"] = float(q["frequency"])
            if q.get("amplitude"):
                waveform_state["amplitude"] = float(q["amplitude"])
            if q.get("offset"):
                waveform_state["offset"]    = float(q["offset"])
            if q.get("phase"):
                waveform_state["phase"]     = float(q["phase"])
            self.send_json({"waveform": waveform_state})

        elif p == "/power":
            q = self._query()
            if q.get("voltage"):
                v = float(q["voltage"])
                dev_config["psu"]["voltage"] = v
            self.send_json({
                "voltage":         dev_config["psu"]["voltage"],
                "powerGood":       True,
                "enabled":         True,
                "overcurrent":     False,
                "thermalWarning":  False,
                "thermalShutdown": False,
            })

        elif p == "/dev/config":
            # Deep-merge the posted JSON into dev_config.
            updates = self._body_json()
            for section, values in updates.items():
                if section not in dev_config:
                    continue
                if not isinstance(values, dict):
                    continue
                for k, v in values.items():
                    if isinstance(v, dict) and isinstance(dev_config[section].get(k), dict):
                        dev_config[section][k].update(v)
                    else:
                        dev_config[section][k] = v
            self.send_json(dev_config)

        else:
            self.send_empty(404)

    # ── OPTIONS (CORS preflight) ─────────────────────────────────────────────

    def do_OPTIONS(self):
        self.send_response(204)
        self._cors()
        self.end_headers()


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

if __name__ == "__main__":
    server = http.server.HTTPServer(("localhost", PORT), Handler)
    print(f"\n  MADAM3 dev server")
    print(f"  Open → http://localhost:{PORT}/")
    print(f"  HTML from: {WEBCONTENT.resolve()}")
    print(f"  Ctrl+C to stop.\n")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n  Server stopped.")
