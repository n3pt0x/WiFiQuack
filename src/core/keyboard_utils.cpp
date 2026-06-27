#include "keyboard_utils.h"
#include "duckyparser.h"

namespace keyboard_utils {
    Layout currentLayout = LAYOUT_FR;
    bool keyboardStarted = false;

    void setLayout(Layout layout) {
        if (currentLayout == layout && keyboardStarted) return;
        
        currentLayout = layout;
        Keyboard.end();
        switch (layout) {
            case LAYOUT_DE:
                Keyboard.begin(KeyboardLayout_de_DE);
                break;
            case LAYOUT_US:
                Keyboard.begin(KeyboardLayout_en_US);
                break;
            case LAYOUT_ES:
                Keyboard.begin(KeyboardLayout_es_ES);
                break;
            case LAYOUT_FR:
                Keyboard.begin(KeyboardLayout_fr_FR);
                break;
            case LAYOUT_IT:
                Keyboard.begin(KeyboardLayout_it_IT);
                break;
            case LAYOUT_PT:
                Keyboard.begin(KeyboardLayout_pt_PT);
                break;
            case LAYOUT_SE:
                Keyboard.begin(KeyboardLayout_sv_SE);
                break;
            case LAYOUT_DK:
                Keyboard.begin(KeyboardLayout_da_DK);
                break;
            default:
                Keyboard.begin(KeyboardLayout_fr_FR);
                break;
            keyboardStarted = true;
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

    void writeKey(const uint8_t key) {
        Keyboard.write(key);
    }

    void sendString(const String& str) {
        Keyboard.print(str);
    }
}