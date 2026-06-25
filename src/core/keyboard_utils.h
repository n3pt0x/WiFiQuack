#pragma once

#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>

namespace keyboard_utils {

    enum Layout : uint8_t {
        LAYOUT_US,
        LAYOUT_FR
    };

    void setLayout(Layout layout);
    void pressKey(uint8_t key);
    void pressCombination(uint8_t modifier, uint8_t key);
    void writeKey(const uint8_t key);
    void sendString(const String& str);
};