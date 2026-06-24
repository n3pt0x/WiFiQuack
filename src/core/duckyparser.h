#pragma once

#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>

namespace duckyparser {
    enum Layout : uint8_t {
        LAYOUT_US,
        LAYOUT_FR
    };

    void begin(Layout initialLayout = LAYOUT_US);
    void setLayout(Layout layout);
    bool execute(const String& script);
    void reset();
}