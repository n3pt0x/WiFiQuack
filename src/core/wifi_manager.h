#pragma once
#include <WiFi.h>

namespace wifi {
    bool startAP();
    void printWiFiInfos();
    IPAddress getIP();
    IPAddress getSubnetMask();
    IPAddress getGateway();
    String getMAC();
    uint32_t getChannel();
}