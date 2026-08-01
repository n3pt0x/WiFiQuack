#include <Arduino.h>
#include <USB.h>
#ifdef PLATFORM_ESP32
    #include <USBHIDMouse.h>
#elifdef PLATFORM_PICO
    #include <Mouse.h>
#endif

namespace mouse_utils {
    #ifdef PLATFORM_ESP32
    static USBHIDMouse mouse;

    void begin() {
        mouse.begin();
    }

    void click(uint8_t b) {
        mouse.click(b);
    }

    void press(uint8_t b) {
        mouse.press(b);
    }

    void release(uint8_t b) {
        mouse.release(b);
    }

    void move(uint8_t x, uint8_t y) {
        mouse.move(x, y);
    }

    void scroll(uint8_t amount) {
        mouse.move(0, 0, amount, 0);
    }

    uint8_t getMouseButton(String param)  {
        if (param == "LEFT") return MOUSE_LEFT;
        if (param == "RIGHT") return MOUSE_RIGHT;
        if (param == "MIDDLE") return MOUSE_MIDDLE;
        if (param == "BACKWARD") return MOUSE_BACKWARD;
        if (param == "FORWARD") return MOUSE_FORWARD;
        if (param == "ALL") return MOUSE_ALL;
        return 0;
    }

    #elifdef PLATFORM_PICO
    
    void begin() {
        Mouse.begin();
    }

    void click(uint8_t b) {
        Mouse.click(b);
    }

    void press(uint8_t b) {
        Mouse.press(b);
    }

    void release(uint8_t b) {
        Mouse.release(b);
    }

    void move(uint8_t x, uint8_t y) {
        Mouse.move(x, y);
    }

    void scroll(uint8_t amount) {
        Mouse.move(0, 0, amount);
    }

    uint8_t getMouseButton(String param)  {
        if (param == "LEFT") return MOUSE_LEFT;
        if (param == "RIGHT") return MOUSE_RIGHT;
        if (param == "MIDDLE") return MOUSE_MIDDLE;
        if (param == "ALL") return MOUSE_ALL;
        return 0;
    }
    #endif
}