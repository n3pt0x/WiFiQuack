#pragma once
#include <Arduino.h>

namespace mouse_utils {
    void begin();
    void click(uint8_t b);
    void press(uint8_t b);
    void release(uint8_t b);
    void move(uint8_t x, uint8_t y);
    void scroll(uint8_t amount);
    uint8_t getMouseButton(String param);
}