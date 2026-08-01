#include <Arduino.h>
#include <LittleFS.h>
#include "core/config.h"
#include "core/debug.h"
#include "core/settings.h"
#include "core/utils.h"
#include "core/wifi_manager.h"
#include "core/webserver.h"
#include "core/duckyparser.h"
#include "core/mouse_utils.h"

void setup() {
  Serial.begin(SERIAL_BAUD);
  settings::begin();
  keyboard_utils::begin();
  mouse_utils::begin();
  delay(STARTUP_DELAY);

  #ifdef DEBUG
    printBanner();
    settings::flushLog();
  #endif

  if (!wifi::startAP()) {
    debugln("Creating AP failed !");
    while (true) { delay(100); }
  } else {
      debugln("AP successfully created !");
      wifi::printWiFiInfos();
  }
  webserver::init();
}

void loop() {
  webserver::processPendingRequests();
}