#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include "keyboard_utils.h"
#include "settings.h"
#include "duckyparser.h"

namespace keyboard_utils {
    static Layout currentLayout = settings::keyboard_layout;
    static bool initialized = false;
    static USBHIDKeyboard Keyboard;

    
    static const uint8_t* getKeyboardLayout(Layout layout) {
        switch (layout) {
            case LAYOUT_DE: return KeyboardLayout_de_DE;
            case LAYOUT_US: return KeyboardLayout_en_US;
            case LAYOUT_ES: return KeyboardLayout_es_ES;
            case LAYOUT_FR: return KeyboardLayout_fr_FR;
            case LAYOUT_IT: return KeyboardLayout_it_IT;
            case LAYOUT_PT: return KeyboardLayout_pt_PT;
            case LAYOUT_SE: return KeyboardLayout_sv_SE;
            case LAYOUT_DK: return KeyboardLayout_da_DK;
            default:        return KeyboardLayout_fr_FR;
        }
    }

    void begin() {
        Layout layout = settings::keyboard_layout;

        if (currentLayout == layout && initialized) return;        
        USB.begin();
        Keyboard.begin(getKeyboardLayout(layout));
        
        currentLayout = layout;
        initialized = true;
        duckyparser::reset();
    }

    void setLayout(Layout layout) {
        settings::keyboard_layout = layout;
        settings::save();
        begin();
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
