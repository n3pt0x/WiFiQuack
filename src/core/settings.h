#pragma once

#include "keyboard_utils.h"

namespace settings {
    extern String wifi_ssid;
    extern String wifi_passphrase;
    extern keyboard_utils::Layout keyboard_layout;
    const char* DEFAULT_LAYOUT_STR = "FR";

    bool begin();
    void load();
    void save();
    String getSettingsJson();
    String layoutToString(keyboard_utils::Layout layout);
    keyboard_utils::Layout stringToLayout(String layout_str);
}