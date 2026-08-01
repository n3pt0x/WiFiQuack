#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>
#include "keyboard_utils.h"
#include "settings.h"
#include "core/ducky/duckyparser.h"

namespace {
    keyboard_utils::Layout _currentLayout = settings::getKeyboardLayout();
    bool _initialized = false;
}

namespace keyboard_utils {
    void begin() {
        Layout layout = settings::getKeyboardLayout();

        if (_currentLayout == layout && _initialized) return;
                
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
        _currentLayout = settings::getKeyboardLayout();
        _initialized = true;
    }

    void setLayout(Layout layout) {
        settings::setKeyboardLayout(layout);
        settings::save();
        begin();
    }

    void press(uint8_t key) {
        Keyboard.press(key);
    }

    void release(uint8_t key) {
        Keyboard.release(key);
    }

    void releaseAll() {
        Keyboard.releaseAll();
    }

    void write(const uint8_t key) {
        Keyboard.write(key);
    }

    void print(const String& str) {
        Keyboard.print(str);
    }

    void pressCombination(uint8_t modifier, uint8_t key) {
        Keyboard.press(modifier);
        Keyboard.write(key);
        Keyboard.releaseAll();
    }

    void pressPower() {
        Keyboard.write(KEY_POWER);
    }

    void pressReset() {
        Keyboard.write(KEY_RESET);
    }
    
    void pressSleep() {
        Keyboard.write(KEY_SLEEP);
    }
}