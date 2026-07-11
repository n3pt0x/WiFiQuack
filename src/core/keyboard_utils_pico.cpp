#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>
#include "keyboard_utils.h"
#include "settings.h"
#include "duckyparser.h"

namespace keyboard_utils {
    Layout currentLayout = settings::keyboard_layout;
    bool initialized = false;

    void begin() {
        Layout layout = settings::keyboard_layout;

        if (currentLayout == layout && initialized) return;
                
        Keyboard.end();
        switch (layout) {
            case LAYOUT_DE: Keyboard.begin(KeyboardLayout_de_DE); break;
            case LAYOUT_US: Keyboard.begin(KeyboardLayout_en_US); break;
            case LAYOUT_ES: Keyboard.begin(KeyboardLayout_es_ES); break;
            case LAYOUT_FR: Keyboard.begin(KeyboardLayout_fr_FR); break;
            case LAYOUT_IT: Keyboard.begin(KeyboardLayout_it_IT); break;
            case LAYOUT_PT: Keyboard.begin(KeyboardLayout_pt_PT); break;
            case LAYOUT_SE: Keyboard.begin(KeyboardLayout_sv_SE); break;
            case LAYOUT_DK: Keyboard.begin(KeyboardLayout_da_DK); break;
            default:        Keyboard.begin(KeyboardLayout_fr_FR); break;
        }
        currentLayout = settings::keyboard_layout;
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