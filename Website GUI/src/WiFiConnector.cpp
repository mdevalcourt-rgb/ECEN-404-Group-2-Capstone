#include "WiFiConnector.h"

#include <Arduino.h>
#include <WiFi.h>

void startAccessPoint(const char *ssid, const char *password) {
  WiFi.mode(WIFI_AP);
  bool started = WiFi.softAP(ssid, password);
  if (!started) {
    Serial.println("Failed to start access point!");
    return;
  }
  (void)password;  // not logged — avoid printing the AP password to Serial.
  Serial.println("Access point started.");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}
