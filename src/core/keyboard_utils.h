#pragma once

#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>

namespace keyboard_utils {

    enum Layout : uint8_t {
        LAYOUT_DE,
        LAYOUT_US,
        LAYOUT_ES,
        LAYOUT_FR,
        LAYOUT_IT,
        LAYOUT_PT,
        LAYOUT_SE,
        LAYOUT_DK
    };

    void setLayout(Layout layout);
    void pressKey(uint8_t key);
    void pressCombination(uint8_t modifier, uint8_t key);
    void writeKey(const uint8_t key);
    void sendString(const String& str);
};