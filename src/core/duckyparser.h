#pragma once

#include "keyboard_utils.h"

namespace duckyparser {
    extern int defaultDelay;
    extern String lastLine;

    bool execute(const String& script, String& errorMsg);
    void reset();
}