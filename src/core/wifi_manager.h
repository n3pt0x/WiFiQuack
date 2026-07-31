#pragma once
#include <WiFi.h>

extern IPAddress ipAddr;
extern IPAddress subnetmask;
extern IPAddress gateway;
extern String macAddr;
extern uint32_t channel;

bool startWiFiAP();
void printWiFiInfos();