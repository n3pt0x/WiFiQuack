#pragma once

#include "keyboard_utils.h"

namespace settings {
    extern String wifi_ssid;
    extern String wifi_passphrase;
    extern keyboard_utils::Layout keyboard_layout;

    bool begin();
    void load();
    void save();
    keyboard_utils::Layout stringToLayout(String layout_str);
}