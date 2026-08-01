#pragma once
#include <Arduino.h>

namespace duckyparser {
    extern int defaultDelay;

    void reset();
    bool execute(const String& script, String& errorMsg);
}