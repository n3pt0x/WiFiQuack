#pragma once

#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>
#include "config.h"
#include "keyboard_utils.h"
#include "utils.h"

namespace duckyparser {
    void begin(keyboard_utils::Layout initialLayout = DEFAULT_KEYBOARD_LAYOUT);
    bool execute(const String& script, String& errorMsg);
    void reset();
}