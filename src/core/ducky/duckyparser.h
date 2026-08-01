#pragma once
#include <Arduino.h>

namespace duckyparser {
    bool execute(const String& script, String& errorMsg);
}