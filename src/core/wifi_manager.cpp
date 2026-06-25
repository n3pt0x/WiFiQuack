#include "wifi_manager.h"
#include "config.h"

bool startWiFiAP()
{
    WiFi.mode(WIFI_AP);
    
    if (!WiFi.softAP(WIFI_SSID, WIFI_PASSPHRASE)) {
        return false;
    } else {
        return true;
    }
}

void printWiFiInfos() {
    Serial.printf("SSID: %s\n", WIFI_SSID);
    Serial.printf("Password: %s\n", WIFI_PASSPHRASE);
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
}