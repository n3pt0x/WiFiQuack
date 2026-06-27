#pragma once

#include "keyboard_utils.h"

const long SERIAL_BAUD = 115200;

#ifdef DEBUG
    #define STARTUP_DELAY 4000
#else
    #define STARTUP_DELAY 1000
#endif

static const char* WIFI_SSID = "WiFiQuack";
static const char* WIFI_PASSPHRASE = "WiFiQuack";

#define DEFAULT_LAYOUT keyboard_utils::LAYOUT_FR