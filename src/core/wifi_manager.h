#pragma once
#include <WiFi.h>

namespace wifi {
    bool startAP();
    void printWiFiInfos();
    IPAddress getIP();
    IPAddress getSubnetMask();
    IPAddress getGateway();
    String getMAC();
    #ifdef PLATFORM_ESP32
    uint32_t getChannel();
    #elifdef PLATFORM_PICO
    int getChannel();
    #endif
}