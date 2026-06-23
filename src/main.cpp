#include <Arduino.h>
#include "core/config.h"
#include "core/wifi_manager.h"
#include "core/web_server.h"
#include "core/utils.h"

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  printBanner();

  bool wifiOk = startWiFiAP();
  if (!wifiOk) {
        unsigned long lastWiFiCheck = millis();
        Serial.println("Creating AP failed !");
        while (true) {
          delay(1000);
        }
  } else {
        Serial.println("AP successfully created !");
        printWiFiInfos();
  }
  initWebServer();
}

void loop() {
  server.handleClient();
}