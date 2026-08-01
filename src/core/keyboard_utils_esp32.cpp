#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>
#include <USBHIDConsumerControl.h>
#include "keyboard_utils.h"
#include "settings.h"
#include "core/ducky/duckyparser.h"

namespace {
    using Layout = keyboard_utils::Layout;
    
    Layout _currentLayout = settings::getKeyboardLayout();
    bool _initialized = false;
    USBHIDKeyboard _Keyboard;
    USBHIDConsumerControl _ConsumerControl;

    const uint8_t* getKeyboardLayout(Layout layout) {
        switch (layout) {
            case keyboard_utils::LAYOUT_DE: return KeyboardLayout_de_DE;
            case keyboard_utils::LAYOUT_US: return KeyboardLayout_en_US;
            case keyboard_utils::LAYOUT_ES: return KeyboardLayout_es_ES;
            case keyboard_utils::LAYOUT_FR: return KeyboardLayout_fr_FR;
            case keyboard_utils::LAYOUT_IT: return KeyboardLayout_it_IT;
            case keyboard_utils::LAYOUT_PT: return KeyboardLayout_pt_PT;
            case keyboard_utils::LAYOUT_SE: return KeyboardLayout_sv_SE;
            case keyboard_utils::LAYOUT_DK: return KeyboardLayout_da_DK;
            default:        return KeyboardLayout_fr_FR;
        }
    }
}

namespace keyboard_utils {    
    void begin() {
        Layout layout = settings::getKeyboardLayout();

        if (_currentLayout == layout && _initialized) return;        
        USB.begin();
        _Keyboard.begin(getKeyboardLayout(layout));
        
        _currentLayout = layout;
        _initialized = true;
    }

    void setLayout(Layout layout) {
        settings::setKeyboardLayout(layout);
        settings::save();
        begin();
    }

    void press(uint8_t key) {
        _Keyboard.press(key);
    }

    void release(uint8_t key) {
        _Keyboard.release(key);
    }

    void releaseAll() {
        _Keyboard.releaseAll();
    }

    void write(const uint8_t key) {
        _Keyboard.write(key);
    }

    void print(const String& str) {
        _Keyboard.print(str);
    }

    void pressCombination(uint8_t modifier, uint8_t key) {
        _Keyboard.press(modifier);
        _Keyboard.write(key);
        _Keyboard.releaseAll();
    }

    void pressPower() {
        _ConsumerControl.press(CONSUMER_CONTROL_POWER);
        delay(50);
        _ConsumerControl.release();
    }

    void pressReset() {
        _ConsumerControl.press(CONSUMER_CONTROL_RESET);
        delay(50);
        _ConsumerControl.release();
    }

    void pressSleep() {
        _ConsumerControl.press(CONSUMER_CONTROL_SLEEP);
        delay(50);
        _ConsumerControl.release();
    }
}
