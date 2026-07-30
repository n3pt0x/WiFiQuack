#include <Arduino.h>
#include <LittleFS.h>
#include "core/config.h"
#include "core/debug.h"
#include "core/settings.h"
#include "core/utils.h"
#include "core/wifi_manager.h"
#include "core/web_server.h"
#include "core/duckyparser.h"

void setup() {
  Serial.begin(SERIAL_BAUD);
  settings::begin();
  keyboard_utils::begin();
  delay(STARTUP_DELAY);

  printBanner();
  settings::flushLog();

  if (!startWiFiAP()) {
    debugln("Creating AP failed !");
    while (true) {
      delay(100);
    }
  } else {
      debugln("AP successfully created !");
      printWiFiInfos();
  }
  initWebServer();
}

void loop() {
  processPendingRequests();
}