#pragma once

#include <WiFi.h>

static const char* WIFI_SSID = "WiFiQuack";
static const char* WIFI_PASSPHRASE = "WiFiQuack";

bool startWiFiAP();
void printWiFiInfos();