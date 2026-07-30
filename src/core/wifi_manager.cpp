#include <WiFi.h>
#include "wifi_manager.h"
#include "settings.h"
#include "debug.h"

bool startWiFiAP()
{
    WiFi.mode(WIFI_AP);
    return WiFi.softAP(settings::wifi_ssid, settings::wifi_passphrase);
}

void printWiFiInfos() {
    debugf("SSID: %s\n", settings::wifi_ssid.c_str());
    debugf("Password: %s\n", settings::wifi_passphrase.c_str());
    debug("IP Address: ");
    debugln(WiFi.softAPIP());
}