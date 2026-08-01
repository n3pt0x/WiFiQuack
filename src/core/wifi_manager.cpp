#include "wifi_manager.h"
#include "settings.h"
#include "debug.h"

namespace {
    IPAddress _ip(192, 168, 4, 1);
    IPAddress _subnet(255, 255, 255, 0);
    IPAddress _gateway = _ip;
    String _mac;
    #ifdef PLATFORM_ESP32
    uint32_t _channel;
    #elifdef PLATFORM_PICO
    int _channel;
    #endif
}

namespace wifi {
    bool startAP() {
        #ifndef DEBUG
            #ifdef PLATFORM_ESP32
                esp_log_level_set("wifi", ESP_LOG_NONE);
                esp_log_level_set("dhcps", ESP_LOG_NONE);
                esp_log_level_set("dhcpc", ESP_LOG_NONE);
                esp_log_level_set("esp_netif", ESP_LOG_NONE);
            #endif
        #endif
        
        WiFi.mode(WIFI_AP);
        bool success = WiFi.softAP(settings::getWiFiSSID().c_str(), settings::getWiFiPassphrase().c_str()) && WiFi.softAPConfig(_ip, _gateway, _subnet);
        if (success) {
            _channel = WiFi.channel();
            _mac = WiFi.softAPmacAddress();
        }
        return success;
    }

    void printWiFiInfos() {
        debugln(F("--- WiFi Info ---"));
        debug(F("SSID: "));
        debugln(settings::getWiFiSSID());
        debug(F("Password: "));
        debugln(settings::getWiFiPassphrase());
        debug(F("IP: "));
        debugln(_ip);
        debug(F("Gateway: "));
        debugln(_gateway);
        debug(F("Subnet: "));
        debugln(_subnet);
        debug(F("MAC Addr: "));
        debugln(_mac);
        debugln(F("-----------------"));
    }

    IPAddress getIP()           { return _ip; }
    IPAddress getSubnetMask()   { return _subnet; }
    IPAddress getGateway()      { return _gateway; }
    String getMAC()             { return _mac; }

    #ifdef PLATFORM_ESP32
    uint32_t getChannel()       { return _channel; }
    #elifdef PLATFORM_PICO
    int getChannel()       { return _channel; }
    #endif
}