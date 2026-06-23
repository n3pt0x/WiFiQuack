#include <Arduino.h>
#include "core/config.h"
#include "core/wifi_manager.h"

void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(1000);

  bool wifiOk = startWiFiAP();
  if (!wifiOk) {
        Serial.println("Creating AP failed !");
        while (true) {
          delay(1000);
        }
    } else {
        Serial.println("AP successfully created !");
        printWiFiInfos();
    }
  }

void loop() {
  handleWiFiClients();
  delay(5000);
}