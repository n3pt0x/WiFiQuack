#include <Arduino.h>
#include "core/config.h"
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
  
  bool wifiOk = startWiFiAP();
  if (!wifiOk) {
        Serial.println("Creating AP failed !");
        while (true) {
          delay(100);
        }
  } else {
        Serial.println("AP successfully created !");
        printWiFiInfos();
  }
  initWebServer();
  Serial.flush();
}

void loop() {
  server.handleClient();
}