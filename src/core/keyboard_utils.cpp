#include "keyboard_utils.h"
#include "duckyparser.h"

namespace keyboard_utils {
    Layout currentLayout = LAYOUT_FR;

    void setLayout(Layout layout) {
        currentLayout = layout;
        Keyboard.end();
        switch (layout) {
            case LAYOUT_FR:
                Keyboard.begin(KeyboardLayout_fr_FR);
                break;
            case LAYOUT_US:
            default:
                Keyboard.begin(KeyboardLayout_en_US);
                break;
        }
    }

    void pressKey(uint8_t key) {
        Keyboard.press(key);
        Keyboard.release(key);
    }

    void pressCombination(uint8_t modifier, uint8_t key) {
        Keyboard.press(modifier);
        Keyboard.write(key);
        Keyboard.releaseAll();
    }

    void sendChar(const char* str) {
        uint8_t key = 0;
        uint8_t modifier = 0;

        for (size_t accent = 0; accent < sizeof(accentTable); accent++) {
            if (strcmp(str, accentTable[accent].utf8)) {
                Keyboard.write(accentTable[accent].keyCode);
            }
        }
        
    }
}