#include <WiFi.h>
#include "wifi_manager.h"
#include "settings.h"
#include "debug.h"

bool startWiFiAP() {
    #ifndef DEBUG
        esp_log_level_set("wifi", ESP_LOG_NONE);
        esp_log_level_set("dhcps", ESP_LOG_NONE);
        esp_log_level_set("dhcpc", ESP_LOG_NONE);
    #endif
    
    IPAddress ip (192, 168, 4, 1);
    WiFi.mode(WIFI_AP);
    
    return WiFi.softAP(settings::wifi_ssid.c_str(), settings::wifi_passphrase.c_str()) && WiFi.softAPConfig(ip, ip, IPAddress(255, 255, 255, 0));
}

void printWiFiInfos() {
    debugf("SSID: %s\n", settings::wifi_ssid.c_str());
    debugf("Password: %s\n", settings::wifi_passphrase.c_str());
    debug("IP Address: ");
    debugln(WiFi.softAPIP());
}