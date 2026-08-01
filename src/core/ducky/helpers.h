#pragma once
#include <Arduino.h>

namespace helpers {
    bool handleMouseButton(const String& command, const String& param, String& errorMsg, void (*action)(uint8_t));
    bool handleMouseCommand(const String& command, const String& param, const String& line, String& errorMsg);
    bool handleModifierKey(const String& command, const String& param, String& errorMsg);
    bool handleRepeat(const String& param, String& errorMsg);
    bool handleCombo(const String& line, String& errorMsg);
}