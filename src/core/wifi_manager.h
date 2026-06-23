#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

static const char* WIFI_SSID = "WiFiQuack";
static const char* WIFI_PASSPHRASE = "WiFiQuack";

bool startWiFiAP();
void printWiFiInfos();

#endif