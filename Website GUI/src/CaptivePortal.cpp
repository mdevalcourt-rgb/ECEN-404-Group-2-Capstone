#include "CaptivePortal.h"

#include <DNSServer.h>

// Responds to every DNS query with the AP's IP (192.168.4.1).
// This is what makes OS connectivity probes reach us instead of timing out,
// and also makes any hostname the user types resolve to the device.
static DNSServer dnsServer;
static constexpr uint8_t kDnsPort = 53;

void setupCaptivePortal() {
  // "*" matches every hostname — wildcard DNS.
  dnsServer.start(kDnsPort, "*", IPAddress(192, 168, 4, 1));
}

void handleCaptivePortal() {
  dnsServer.processNextRequest();
}
