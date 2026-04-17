#pragma once

// -----------------------------------------------------------------------------
// Local credentials template.
//
// Setup (once per clone):
//   1. Copy this file to  include/secrets.h
//   2. Replace the placeholder values below with your real credentials.
//   3. secrets.h is git-ignored; it will never be committed.
//
// Rotate these from the defaults shown in older commits — those passwords are
// in the public git history and must be considered compromised.
// -----------------------------------------------------------------------------

#define AP_SSID        "ESP32-Oscilloscope"
#define AP_PASSWORD    "CHANGE_ME_AP_PASSWORD"

#define OTA_HOSTNAME   "esp32-oscilloscope"
#define OTA_PASSWORD   "CHANGE_ME_OTA_PASSWORD"
