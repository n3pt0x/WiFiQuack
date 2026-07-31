#pragma once
#include "keyboard_utils.h"

namespace duckyparser {
    extern int defaultDelay;

    bool execute(const String& script, String& errorMsg);
    void reset();
}