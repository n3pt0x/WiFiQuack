#include <Arduino.h>
#include <USB.h>
#include <USBHIDMouse.h>

namespace mouse_utils {
    static USBHIDMouse mouse;

    void begin() {
        mouse.begin();
    }

    void click() {
        mouse.click();
    }

    void press() {
        mouse.press();
    }

    void release() {
        mouse.release();
    }

    void move(uint8_t x, uint8_t y) {
        mouse.move(x, y);
    }

    void scroll(uint8_t amount) {
        mouse.move(0, 0, amount, 0);
    }
}