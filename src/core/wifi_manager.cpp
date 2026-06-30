#include <WiFi.h>
#include "wifi_manager.h"
#include "settings.h"

bool startWiFiAP()
{
    WiFi.mode(WIFI_AP);
    return WiFi.softAP(settings::wifi_ssid, settings::wifi_passphrase);
}

void printWiFiInfos() {
    Serial.printf("SSID: %s\n", settings::wifi_ssid.c_str());
    Serial.printf("Password: %s\n", settings::wifi_passphrase.c_str());
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
}