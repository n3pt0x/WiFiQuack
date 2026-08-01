#include "helpers.h"
#include "duckyparser.h"
#include "../keyboard_utils.h"
#include "../mouse_utils.h"
#include "../keymap.h"
#include "../utils.h"

namespace {
    const KeyMap _modifierKey[] PROGMEM = {
        {"CTRL", KEY_LEFT_CTRL},
        {"CTRL_RIGHT", KEY_RIGHT_CTRL},
        {"CONTROL", KEY_LEFT_CTRL},
        {"CONTROL_RIGHT", KEY_RIGHT_CTRL},

        {"SHIFT", KEY_LEFT_SHIFT},
        {"SHIFT_RIGHT", KEY_RIGHT_SHIFT},

        {"ALT", KEY_LEFT_ALT},
        {"ALT_RIGHT", KEY_RIGHT_ALT},
        
        {"GUI", KEY_LEFT_GUI},
        {"GUI_RIGHT", KEY_RIGHT_GUI},
        {"WINDOWS", KEY_LEFT_GUI},
        {"WINDOWS_RIGHT", KEY_RIGHT_GUI},
    };
    constexpr size_t _modifierKeySize = sizeof(_modifierKey) / sizeof(_modifierKey[0]);
}

namespace helpers {
    bool handleMouseButton(const String& command, const String& param, String& errorMsg, void (*action)(uint8_t)) {
        if (param.isEmpty()) {
            return setError(errorMsg, command + " requires 1 parameter (LEFT, RIGHT, MIDDLE, etc.)");
        }

        uint8_t button = mouse_utils::getMouseButton(param);
        if (button == 0) {
            return setError(errorMsg, "Invalid button: " + param);
        }
        
        action(button);
        return true;
    }

    bool handleMouseCommand(const String& command, const String& param, const String& line, String& errorMsg) {
        if (command == "MOVE" || command == "MOUSE_MOVE") {
            std::vector<String> params = splitParams(line);
            if (params.size() != 2) {
                return setError(errorMsg, "MOVE requires 2 arguments: vertical horizontal");
            }
            
            int vertical = params[0].toInt();
            int horizontal = params[1].toInt();

            mouse_utils::move(vertical, horizontal);
        }
        else if (command == "SCROLL" || command == "MOUSE_SCROLL") {
            mouse_utils::scroll(param.toInt());
        }
        else if (command == "CLICK" || command == "MOUSE_CLICK") {
            helpers::handleMouseButton(command, param, errorMsg, mouse_utils::click);
        }
        else if (command == "MOUSEPRESS" || command == "MOUSE_PRESS") {
            helpers::handleMouseButton(command, param, errorMsg, mouse_utils::press);
        }
        else if (command == "MOUSERELEASE" || command == "MOUSE_RELEASE") {
            helpers::handleMouseButton(command, param, errorMsg, mouse_utils::release);
        }
        return false;
    }

    bool handleModifierKey(const String& command, const String& param, String& errorMsg) {
        uint8_t HIDCodeCommand = getHIDCode(command, _modifierKey, _modifierKeySize);
        if (HIDCodeCommand == 0) {
            return setError(errorMsg, "Unknown modifier: " + command);
        }

        if (param.length() == 1) {
            keyboard_utils::pressCombination(HIDCodeCommand, param[0]);
            return true;
        }

        uint8_t HIDCodeParam = getHIDCode(param);
        if (HIDCodeParam != 0) {
            keyboard_utils::press(HIDCodeCommand);
            keyboard_utils::press(HIDCodeParam);
            keyboard_utils::releaseAll();
            return true;
        }
        return setError(errorMsg, "Invalid parameter for " + command + ": " + param);
    }

    bool handleRepeat(const String& param, String& errorMsg) {
        return true;
    }

    bool handleCombo(const String& line, String& errorMsg) {
        std::vector<String> params = splitParams(line);
        
        for (const auto& param : params) {
            if (param.length() == 1) {
                keyboard_utils::press(param[0]);
            } else {
                uint8_t code = getHIDCode(param);
                if (code != 0) {
                    keyboard_utils::press(code);
                } else {
                    errorMsg = "Unknown key in COMBO: " + param;
                    return false;
                }
            }
        }
        delay(50);
        keyboard_utils::releaseAll();
        return true;
    }
}