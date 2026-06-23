#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>

const char *WIFI_SSID = "WiFiQuack";
const char *WIFI_PASSPHRASE = "WiFiQuack";

bool startWiFiAP();
void printWiFiInfos();
void handleWiFiClients();

#endif