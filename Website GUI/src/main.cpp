#include <Arduino.h>

#include "Ad7356Sampler.h"
#include "Ad9833Driver.h"
#include "Ads1115Sampler.h"
#include "HttpServer.h"
#include "Mic24045.h"
#include "StartupLog.h"
#include "OtaUpdater.h"
#include "WiFiConnector.h"

// Credentials for the ESP32-hosted WiFi network.
constexpr const char *AP_SSID     = "ESP32-Oscilloscope";
constexpr const char *AP_PASSWORD = "scopeview123";

// OTA credentials.
constexpr const char *OTA_HOSTNAME = "esp32-oscilloscope";
constexpr const char *OTA_PASSWORD = "scopeota123";

Ad7356Sampler  adcSampler;
Ad9833Driver   wavegen;
Ads1115Sampler adsSampler;
Mic24045       mic;

// ---------------------------------------------------------------------------

void printAdcInfo() {
  String line = "ADC channels: ";
  for (size_t i = 0; i < adcSampler.channelCount(); ++i) {
    line += adcSampler.label(i);
    if (i + 1 < adcSampler.channelCount()) line += ", ";
  }
  logln(line.c_str());
}

void setup() {
  Serial.begin(115200);
  // Startup messages are buffered so they are never lost even if the serial
  // monitor is not open yet.  Type "log" in the serial monitor to replay them.

  logln("[SETUP] Serial ready");

  logln("[SETUP] adcSampler.begin()");
  adcSampler.begin();
  logln("[SETUP] adcSampler.begin() done");
  adcSampler.diagnose();

  logln("[SETUP] wavegen.begin()");
  wavegen.begin();
  logln("[SETUP] wavegen.begin() done");

  // Probe both I²C chips in parallel for up to 2 seconds.
  bool micOk = false;
  bool adsOk = false;
  const uint32_t initDeadline = millis() + 2000;
  logln("[SETUP] entering I2C init loop (2 s deadline)");
  while (millis() < initDeadline) {
    logf("[SETUP] tick t=%lu ms  micOk=%d adsOk=%d\n",
         millis(), micOk, adsOk);
    if (!micOk) micOk = mic.begin();
    if (!adsOk) adsOk = adsSampler.begin();
    if (micOk && adsOk) break;
    yield();
    delay(100);
  }
  logln("[SETUP] I2C init loop done");
  logln(micOk ? "MIC24045 initialized." : "MIC24045 init failed.");
  logln(adsOk ? "ADS1115 initialized."  : "ADS1115 init failed. Check wiring/address.");

  logln("[SETUP] startAccessPoint()");
  startAccessPoint(AP_SSID, AP_PASSWORD);
  logln("[SETUP] startAccessPoint() done");

  logln("[SETUP] setupOTA()");
  setupOTA(OTA_HOSTNAME, OTA_PASSWORD);
  logln("[SETUP] setupOTA() done");

  logln("[SETUP] setupServerRoutes()");
  setupServerRoutes(adcSampler, adsSampler, mic, wavegen);
  logln("[SETUP] httpServer().begin()");
  httpServer().begin();

  logln("HTTP server started. Open the IP above in a browser.");
  printAdcInfo();
  logln("[SETUP] setup() complete");
  Serial.println("Type \"log\" to replay startup messages.");
}

// ---------------------------------------------------------------------------
// Serial command handler
// ---------------------------------------------------------------------------
static String serialBuf;

static void handleSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      serialBuf.trim();
      if (serialBuf.equalsIgnoreCase("log")) {
        printStartupLog();
      } else if (serialBuf.length() > 0) {
        Serial.printf("Unknown command: \"%s\". Available: log\n",
                      serialBuf.c_str());
      }
      serialBuf = "";
    } else {
      serialBuf += c;
    }
  }
}

void loop() {
  handleOTA();
  handleSerial();
  httpServer().handleClient();
}
