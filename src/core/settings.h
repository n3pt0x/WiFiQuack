#pragma once

#include "keyboard_utils.h"

namespace settings {
    bool begin();
    void load();
    void save();
    String getSettingsJson();
    bool setSettingsFromJson(const String& json);
    String layoutToString(keyboard_utils::Layout layout);
    keyboard_utils::Layout stringToLayout(String layout_str);
    void logToBuffer(const char* log);
    void logToBuffer(const String& log);
    void flushLog();
    bool createDefaultConfig();
    void verifySave(const String& expectedJson);
    String getWiFiSSID();
    String getWiFiPassphrase();
    keyboard_utils::Layout getKeyboardLayout();
    void setKeyboardLayout(keyboard_utils::Layout layout);
}