#pragma once

#include "keyboard_utils.h"

namespace settings {
    extern String wifi_ssid;
    extern String wifi_passphrase;
    extern keyboard_utils::Layout keyboard_layout;
    extern String logBuffer;

    bool begin();
    void load();
    void save();
    void flushLog();
    String getSettingsJson();
    bool setSettingsFromJson(const String& json);
    String layoutToString(keyboard_utils::Layout layout);
    keyboard_utils::Layout stringToLayout(String layout_str);
    void logToBuffer(const char* log);
    void logToBuffer(const String& log);
    bool createDefaultConfig();
    void verifySave(const String& expectedJson);
}