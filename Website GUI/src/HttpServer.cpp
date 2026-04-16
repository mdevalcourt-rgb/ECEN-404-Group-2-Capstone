#include "HttpServer.h"

#include <Arduino.h>
#include <array>
#include <cstdlib>
#include <vector>

#include "Ad7356Sampler.h"
#include "Ad9833Driver.h"
#include "Ads1115Sampler.h"
#include "Mic24045.h"
#include "WebContent.h"

namespace {
WebServer server(80);

// AD7356 amplitude calibration — cal_20260327_195422.txt, channel B.
// Channel A uses the same coefficients as an approximation (same hardware path).
// Piecewise linear: find segment, apply V = slope * raw + intercept.
// Extrapolate outside the measured range [2566, 3322] with the nearest segment.
//   seg_0 (raw < 2817 or extrapolate below): slope=0.00099685, intercept=-2.0581263208
//   seg_1 (raw >= 2817 or extrapolate above): slope=0.0009897887, intercept=-2.0382347119
// Full ADC range 0–4095 maps to approximately +2.058 V to −2.015 V (signal path is inverting).
static float applyAdcCalibration(uint16_t raw) {
  const float adc = static_cast<float>(raw);
  if (adc < 2817.0f) {
    return -(0.00099685f * adc - 2.0581263208f);
  }
  return -(0.0009897887f * adc - 2.0382347119f);
}

constexpr uint16_t kAdcMaxValue  = 4095;
// Calibrated full-scale window: +2.058 V (raw=0) to −2.015 V (raw=4095) ≈ 4.073 V span.
constexpr float    kMaxVoltage   = 4.073f;
constexpr float kMinWaveOffset = -5.0f;
constexpr float kMaxWaveOffset = 5.0f;
constexpr float kMinWavePhase  = 0.0f;
constexpr float kMaxWavePhase  = 360.0f;
constexpr float kMinWaveAmplitude = 0.0f;
constexpr float kMaxWaveAmplitude = 5.0f;
constexpr float kMinWaveFrequency = 0.1f;
constexpr float kMaxWaveFrequency = 200000.0f;  // 200 kHz placeholder

float supplyVoltage = 0.0f;
struct WaveformConfig {
  String mode = "square";
  float frequency = 1000.0f;  // Hz
  float amplitude = 1.0f;     // Vpp
  float offset = 0.0f;        // V
  float phase = 0.0f;         // degrees (0–360)
} waveformCfg;

float clampWaveAmplitude(float value) {
  return constrain(value, kMinWaveAmplitude, kMaxWaveAmplitude);
}

float clampWaveOffset(float value) {
  return constrain(value, kMinWaveOffset, kMaxWaveOffset);
}

float clampWavePhase(float value) {
  return constrain(value, kMinWavePhase, kMaxWavePhase);
}

float clampWaveFrequency(float value) {
  return constrain(value, kMinWaveFrequency, kMaxWaveFrequency);
}

String serializeControlState() {
  String json = "{\"supplies\":[";
  json += String(supplyVoltage, 3);
  json += "],\"waveform\":{\"mode\":\"";
  json += waveformCfg.mode;
  json += "\",\"frequency\":";
  json += String(waveformCfg.frequency, 2);
  json += ",\"amplitude\":";
  json += String(waveformCfg.amplitude, 2);
  json += ",\"offset\":";
  json += String(waveformCfg.offset, 2);
  json += ",\"phase\":";
  json += String(waveformCfg.phase, 2);
  json += "}}";
  return json;
}
}  // namespace

WebServer &httpServer() { return server; }

void setupServerRoutes(Ad7356Sampler &sampler, Ads1115Sampler &ads,
                       Mic24045 &mic, Ad9833Driver &wave) {
  // ---------------------------------------------------------------------------
  // Captive portal — redirect OS connectivity probes to the main page.
  // The DNS server (CaptivePortal.cpp) routes every hostname to 192.168.4.1,
  // so these handlers receive the OS probes and return a redirect that causes
  // the OS to detect a captive portal and auto-open the browser.
  // ---------------------------------------------------------------------------
  auto redirectToRoot = []() {
    server.sendHeader("Location", "http://192.168.4.1/");
    server.send(302, "text/plain", "");
  };
  server.on("/connecttest.txt",           redirectToRoot);  // Windows NCSI
  server.on("/redirect",                  redirectToRoot);  // Windows fallback
  server.on("/generate_204",              redirectToRoot);  // Android / Chrome
  server.on("/hotspot-detect.html",       redirectToRoot);  // iOS / macOS
  server.on("/library/test/success.html", redirectToRoot);  // older iOS
  server.on("/success.txt",               redirectToRoot);  // Firefox
  server.on("/canonical.html",            redirectToRoot);  // some Android
  server.onNotFound(                      redirectToRoot);  // any other probe

  server.on("/", []() { server.send_P(200, "text/html", INDEX_HTML); });

  server.on("/adc", [&sampler]() {
    const SampleSet sample = sampler.read();
    String json = "{\"values\":[";
    for (size_t i = 0; i < sample.values.size(); ++i) {
      const float voltage = applyAdcCalibration(sample.values[i]);
      json += String(voltage, 3);
      if (i + 1 < sample.values.size()) {
        json += ',';
      }
    }
    json += "],\"maxVoltage\":";
    json += String(kMaxVoltage, 2);
    json += ",\"rawMax\":";
    json += kAdcMaxValue;
    json += '}';
    server.send(200, "application/json", json);
  });

  // Burst capture endpoint for oscilloscope display.
  // GET /adc/burst?n=256&period_us=17  (n capped at 512, period_us clamped to 17-100000)
  // Returns pre-scaled voltages for both channels plus the actual sample period used.
  server.on("/adc/burst", [&sampler]() {
    constexpr size_t    kMaxBurst      = 512;
    constexpr uint32_t  kMinPeriodUs   = 17;
    constexpr uint32_t  kMaxPeriodUs   = 100000;

    const String nArg = server.arg("n");
    size_t n = nArg.length() ? static_cast<size_t>(nArg.toInt()) : 256;
    if (n < 2)        n = 2;
    if (n > kMaxBurst) n = kMaxBurst;

    const String periodArg = server.arg("period_us");
    uint32_t periodUs = periodArg.length() ? static_cast<uint32_t>(periodArg.toInt()) : kMinPeriodUs;
    if (periodUs < kMinPeriodUs) periodUs = kMinPeriodUs;
    if (periodUs > kMaxPeriodUs) periodUs = kMaxPeriodUs;

    std::vector<uint16_t> bufA(n), bufB(n);

    sampler.readBurst(bufA.data(), bufB.data(), n, periodUs);
    const uint32_t sampleRateHz = static_cast<uint32_t>(1000000UL / periodUs);

    // Build JSON: {"chA":[...],"chB":[...],"count":N,"sampleRateHz":R}
    String json;
    json.reserve(n * 14 + 64);
    json = "{\"chA\":[";
    for (size_t i = 0; i < n; ++i) {
      json += String(applyAdcCalibration(bufA[i]), 3);
      if (i + 1 < n) json += ',';
    }
    json += "],\"chB\":[";
    for (size_t i = 0; i < n; ++i) {
      json += String(applyAdcCalibration(bufB[i]), 3);
      if (i + 1 < n) json += ',';
    }
    json += "],\"count\":";
    json += n;
    json += ",\"sampleRateHz\":";
    json += sampleRateHz;
    json += ",\"samplePeriodUs\":";
    json += periodUs;
    json += ",\"maxVoltage\":";
    json += String(kMaxVoltage, 2);
    json += '}';
    server.send(200, "application/json", json);
  });

  // On-demand 500 ms oversampled measurement for one ADS1115 channel.
  // GET /ads/measure?ch=N  (N = 0..3)
  server.on("/ads/measure", [&ads]() {
    const String chArg = server.arg("ch");
    const int chInt = chArg.toInt();
    if (chArg.length() == 0 || chInt < 0 || chInt > 2) {
      server.send(400, "application/json", "{\"error\":\"ch must be 0-2\"}");
      return;
    }
    const uint8_t ch = static_cast<uint8_t>(chInt);
    const AdsChannelResult r = ads.measure(ch);
    String json = "{\"ch\":";
    json += ch;
    json += ",\"rawAvg\":";
    json += String(r.rawAvg, 1);
    json += ",\"value\":";
    json += String(r.value, 3);
    json += ",\"isOC\":";
    json += r.isOC ? "true" : "false";
    json += "}";
    server.send(200, "application/json", json);
  });

  server.on("/power", HTTP_POST, [&mic]() {
    const String voltArg = server.arg("voltage");
    if (voltArg.length() == 0) {
      server.send(400, "application/json", "{\"error\":\"Missing voltage parameter\"}");
      return;
    }

    const float targetV = voltArg.toFloat();

    if (targetV < 0.640f || targetV > 5.250f) {
      server.send(400, "application/json", "{\"error\":\"Voltage out of range (0.640-3.420 or 4.750-5.250 V)\"}");
      return;
    }
    if (targetV > 3.420f && targetV < 4.750f) {
      server.send(400, "application/json", "{\"error\":\"Voltage in invalid gap (3.420-4.750 V)\"}");
      return;
    }

    if (!mic.setVoltage(targetV)) {
      server.send(503, "application/json", "{\"error\":\"I2C write failed\"}");
      return;
    }

    const float actualV = mic.readVoltage();
    const Mic24045::Status s = mic.getStatus();
    supplyVoltage = actualV;

    String json = "{\"voltage\":";
    json += String(actualV, 3);
    json += ",\"powerGood\":";
    json += s.powerGood ? "true" : "false";
    json += ",\"enabled\":";
    json += s.enabled ? "true" : "false";
    json += ",\"overcurrent\":";
    json += s.overcurrent ? "true" : "false";
    json += ",\"thermalWarning\":";
    json += s.thermalWarning ? "true" : "false";
    json += ",\"thermalShutdown\":";
    json += s.thermalShutdown ? "true" : "false";
    json += "}";
    server.send(200, "application/json", json);
  });

  server.on("/waveform", HTTP_POST, [&wave]() {
    const String typeArg = server.arg("type");
    if (typeArg.length() > 0) {
      waveformCfg.mode = typeArg;
    }
    waveformCfg.frequency = clampWaveFrequency(server.arg("frequency").toFloat());
    waveformCfg.amplitude = clampWaveAmplitude(server.arg("amplitude").toFloat());
    waveformCfg.offset = clampWaveOffset(server.arg("offset").toFloat());
    waveformCfg.phase = clampWavePhase(server.arg("phase").toFloat());

    wave.apply(waveformCfg.mode, waveformCfg.frequency, waveformCfg.amplitude, waveformCfg.phase);

    String json = "{\"waveform\":{\"mode\":\"";
    json += waveformCfg.mode;
    json += "\",\"frequency\":";
    json += String(waveformCfg.frequency, 2);
    json += ",\"amplitude\":";
    json += String(waveformCfg.amplitude, 2);
    json += ",\"offset\":";
    json += String(waveformCfg.offset, 2);
    json += ",\"phase\":";
    json += String(waveformCfg.phase, 2);
    json += "}}";
    server.send(200, "application/json", json);
  });

  server.on("/digipot", HTTP_POST, [&wave]() {
    const int raw = constrain(server.arg("value").toInt(), 0, 255);
    wave.setAmplitudeRaw(static_cast<uint8_t>(raw));
    String json = "{\"digipot\":";
    json += String(raw);
    json += "}";
    server.send(200, "application/json", json);
  });

  // ---------------------------------------------------------------------------
  // Bode plot sweep — SSE streaming endpoint.
  // GET /bode/start?dwell_ms=50
  //
  // Sweeps 50 log-spaced frequencies from 10 Hz to 6 kHz.
  // For each point it:
  //   1. Sets a sine wave at the target frequency (confirmed 1 V: digipot 165).
  //   2. Waits dwell_ms for the DUT to settle.
  //   3. Captures 256 samples at an adaptive rate (target 4 cycles per window).
  //   4. Runs the Goertzel single-bin DFT on CH A (input) and CH B (output).
  //   5. Streams {"i":N,"f":F,"gain":G,"phase":P} as an SSE event.
  //
  // Chunked transfer keeps the connection open for the duration of the sweep.
  // After the sweep the waveform is restored to its previous state.
  server.on("/bode/start", HTTP_GET, [&sampler, &wave]() {
    const int dwellMs = constrain(server.arg("dwell_ms").toInt(), 10, 2000);

    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/event-stream", "");
    server.sendContent("retry: 3000\n\n");

    // Sine wave, confirmed 1 V output (digipot raw 165).
    wave.apply("sine", 10.0f, 1.0f, 0.0f);
    wave.setAmplitudeRaw(165);
    delay(200);  // initial settle after enabling output

    constexpr int    kNumPoints = 50;
    constexpr float  kFstart    = 10.0f;
    constexpr float  kFend      = 6000.0f;
    constexpr size_t kN         = 256;   // samples per capture
    constexpr int    kCycles    = 4;     // target complete cycles per window

    std::vector<uint16_t> bufA(kN), bufB(kN);

    for (int i = 0; i < kNumPoints; i++) {
      const float freq = kFstart *
          powf(kFend / kFstart, static_cast<float>(i) / (kNumPoints - 1));

      wave.apply("sine", freq, 1.0f, 0.0f);
      wave.setAmplitudeRaw(165);
      delay(dwellMs);

      // Adaptive sample rate: target kCycles full cycles in kN samples.
      const float    targetFs  = freq * kN / static_cast<float>(kCycles);
      const uint32_t periodUs  = static_cast<uint32_t>(
          constrain(1000000.0f / targetFs, 17.0f, 100000.0f));
      const float    actualFs  = 1000000.0f / static_cast<float>(periodUs);

      sampler.readBurst(bufA.data(), bufB.data(), kN, periodUs);

      float magA = 0.0f, phaseA = 0.0f, magB = 0.0f, phaseB = 0.0f;
      Ad7356Sampler::binDFT(bufA.data(), kN, freq, actualFs, &magA, &phaseA);
      Ad7356Sampler::binDFT(bufB.data(), kN, freq, actualFs, &magB, &phaseB);

      const float gainDb = (magA > 1e-4f) ? 20.0f * log10f(magB / magA) : 0.0f;
      float phaseDeg = (phaseB - phaseA) * (180.0f / static_cast<float>(M_PI));
      while (phaseDeg >  180.0f) phaseDeg -= 360.0f;
      while (phaseDeg < -180.0f) phaseDeg += 360.0f;

      String event = "data: {\"i\":";
      event += i;
      event += ",\"f\":";    event += String(freq, 2);
      event += ",\"gain\":"; event += String(gainDb, 2);
      event += ",\"phase\":";event += String(phaseDeg, 1);
      event += "}\n\n";
      server.sendContent(event);
    }

    server.sendContent("data: {\"done\":true}\n\n");
    server.sendContent("");  // chunked EOF

    // Restore previous waveform state.
    wave.apply(waveformCfg.mode, waveformCfg.frequency,
               waveformCfg.amplitude, waveformCfg.phase);
  });

  server.on("/control-state", []() { server.send(200, "application/json", serializeControlState()); });
}
