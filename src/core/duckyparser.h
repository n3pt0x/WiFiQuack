#pragma once

#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>
#include "keyboard_utils.h"

namespace duckyparser {
    void begin(keyboard_utils::Layout initialLayout = keyboard_utils::LAYOUT_US);
    bool execute(const String& script);
}