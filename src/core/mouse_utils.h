#pragma once
#include <Arduino.h>

namespace mouse_utils {
    void begin();
    void click();
    void press();
    void release();
    void move(uint8_t x, uint8_t y);
    void scroll(uint8_t amount);
}