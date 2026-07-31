#include "wifi_manager.h"
#include "settings.h"
#include "debug.h"

IPAddress ipAddr(192, 168, 4, 1);
IPAddress subnetmask(255, 255, 255, 0);
IPAddress gateway = ipAddr;
String macAddr;
uint32_t channel;

bool startWiFiAP() {
    #ifndef DEBUG
        esp_log_level_set("wifi", ESP_LOG_NONE);
        esp_log_level_set("dhcps", ESP_LOG_NONE);
        esp_log_level_set("dhcpc", ESP_LOG_NONE);
    #endif
    
    WiFi.mode(WIFI_AP);

    bool success = WiFi.softAP(settings::wifi_ssid.c_str(), settings::wifi_passphrase.c_str()) && WiFi.softAPConfig(ipAddr, gateway, subnetmask);

    if (success) {
        channel = WiFi.channel();
        macAddr = WiFi.softAPmacAddress();
    }
    
    return success;
}

void printWiFiInfos() {
    debugf("SSID: %s\n", settings::wifi_ssid.c_str());
    debugf("Password: %s\n", settings::wifi_passphrase.c_str());
    debug("IP Address: ");
    debugln(WiFi.softAPIP());
}