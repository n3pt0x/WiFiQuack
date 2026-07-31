#include "wifi_manager.h"
#include "settings.h"
#include "debug.h"

namespace {
    IPAddress _ip(192, 168, 4, 1);
    IPAddress _subnet(255, 255, 255, 0);
    IPAddress _gateway = _ip;
    String _mac;
    uint32_t _channel;
}

namespace wifi {
    IPAddress getIP()           { return _ip; }
    IPAddress getSubnetMask()   { return _subnet; }
    IPAddress getGateway()      { return _gateway; }
    String getMAC()             { return _mac; }
    uint32_t getChannel()       { return _channel; }

    bool startAP() {
        #ifndef DEBUG
            esp_log_level_set("wifi", ESP_LOG_NONE);
            esp_log_level_set("dhcps", ESP_LOG_NONE);
            esp_log_level_set("dhcpc", ESP_LOG_NONE);
        #endif
        
        WiFi.mode(WIFI_AP);
        bool success = WiFi.softAP(settings::wifi_ssid.c_str(), settings::wifi_passphrase.c_str()) && WiFi.softAPConfig(_ip, _gateway, _subnet);
        if (success) {
            _channel = WiFi.channel();
            _mac = WiFi.softAPmacAddress();
        }
        return success;
    }

    void printWiFiInfos() {
        debugln("--- WiFi Info ---");
        debugf("SSID: %s\n", settings::wifi_ssid.c_str());
        debugf("Password: %s\n", settings::wifi_passphrase.c_str());
        debug("IP: ");
        debugln(_ip);
        debug("Gateway: ");
        debugln(_gateway);
        debug("Subnet: ");
        debugln(_subnet);
        debug("MAC Addr: ");
        debugln(_mac);
        debugln("-----------------");
    }
}