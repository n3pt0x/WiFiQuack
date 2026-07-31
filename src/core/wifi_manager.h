#pragma once
#include <WiFi.h>

namespace wifi {
    bool startAP();
    IPAddress getIP();
    IPAddress getSubnetMask();
    IPAddress getGateway();
    String getMAC();
    uint32_t getChannel();
    void printWiFiInfos();
}