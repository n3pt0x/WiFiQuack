#pragma once

#include "keyboard_utils.h"

namespace settings {
    extern String wifi_ssid;
    extern String wifi_passphrase;
    extern keyboard_utils::Layout keyboard_layout;
    extern const char* DEFAULT_LAYOUT_STR;

    bool begin();
    void load();
    void save();
    String getSettingsJson();
    bool setSettingsFromJson(const String& json);
    String layoutToString(keyboard_utils::Layout layout);
    keyboard_utils::Layout stringToLayout(String layout_str);
}