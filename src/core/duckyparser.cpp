#include <Arduino.h>
#include "keyboard_def.h"
#include "keyboard_utils.h"
#include "keymap.h"
#include "mouse_utils.h"
#include "duckyparser.h"
#include "utils.h"

namespace duckyparser {

    int defaultDelay = 0;

    void reset() {
        defaultDelay = 0;
    }

    bool handleModifierKey(const String& command, const String& param, const uint8_t HIDCode, String& errorMsg) {
        if (param.length() == 1) {
            keyboard_utils::pressCombination(HIDCode, param[0]);
        } else {
            uint8_t HIDCodeParam = getHIDCode(param);
            if (HIDCodeParam) {
                keyboard_utils::press(HIDCode);
                keyboard_utils::press(HIDCodeParam);
                keyboard_utils::releaseAll();
            } else {
                return setError(errorMsg, "Invalid parameter for " + command + ": " + param);
            }
        }
        return true;
    }

    bool press(String command, String& errorMsg) {
        uint8_t code = getHIDCode(command);
        if (code != 0) {
            keyboard_utils::write(code);
        }
        else if (command == "POWER") {
            keyboard_utils::pressPower();
        }
        else if (command == "RESET") {
            keyboard_utils::pressReset();
        }
        else if (command == "SLEEP"){
            keyboard_utils::pressSleep();
        }
        else {
            return setError(errorMsg, "Unknown command: " + command);
        }
        return true;
    }

    bool parser(String line, String& errorMsg) {
        line = cleanLine(line);
        if (line.isEmpty()) return true;

        if (line[0] == '#' || line.startsWith("REM ")) {
            return true;
        }

        int firstSpace = line.indexOf(' ');
        bool isDelay = false;

        if (firstSpace == -1) {
            if (!press(line, errorMsg)) {
                return false;
            }
        } else {
            String command = cleanLine(line.substring(0, firstSpace));
            String param = cleanLine(line.substring(firstSpace + 1));

            if (param.length() == 0) {
                return setError(errorMsg, "Empty parameter in command: " + command);
            }

            if (command == "STRING") {
                keyboard_utils::sendString(param);
            }
            else if (command == "DELAY") {
                int delayTime = param.toInt();
                if (delayTime < 0) {
                    return setError(errorMsg, "Invalid parameter for " + command + " : " + param);
                }
                if (delayTime > 0) delay(delayTime);
                isDelay = true;
            }
            else if (command == "DEFAULTDELAY" || command == "DEFAULT_DELAY") {
                defaultDelay = param.toInt();
                if (defaultDelay < 0) {
                    return setError(errorMsg, "Invalid parameter for " + command + " : " + param);
                }
                if (defaultDelay < 0) defaultDelay = 0;
            }
            else if (command == "GUI" || command == "WINDOWS") {
                if (!handleModifierKey(command, param, KEY_LEFT_GUI, errorMsg)) return false;
            }
            else if (command == "CTRL" || command == "CONTROL") {
                if (!handleModifierKey(command, param, KEY_LEFT_CTRL, errorMsg)) return false;
            }
            else if (command == "KEYCODE") {
                int secondSpace = line.indexOf(' ', firstSpace + 1);
                if (secondSpace != -1) {
                    String modifierStr = line.substring(firstSpace + 1, secondSpace);
                    String keyStr = line.substring(secondSpace + 1);
                    uint8_t modifier = (uint8_t)strtol(modifierStr.c_str(), NULL, 0);
                    uint8_t key = (uint8_t)strtol(keyStr.c_str(), NULL, 0);
                    keyboard_utils::write(modifier);
                    keyboard_utils::write(key);
                } else {
                    return setError(errorMsg, "KEYCODE: missing parameters (e.g. KEYCODE 0x02 0x04)");
                }
            }
            else if (command == "COMBO") {
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
            }
            else if (command == "SHIFT") {
                if (!handleModifierKey(command, param, KEY_LEFT_CTRL, errorMsg)) return false;
            }
            else if (command == "ALT") {
                if (!handleModifierKey(command, param, KEY_LEFT_ALT, errorMsg)) return false;
            }
            else if (command == "CLICK" || command == "MOUSE_CLICK") {
                if (param.isEmpty()) {
                    return setError(errorMsg, command + " requires 1 parameter (LEFT, RIGHT, MIDDLE, etc.)");
                }
                uint8_t button = mouse_utils::getMouseButton(param);
                if (button == 0) {
                    return setError(errorMsg, "Invalid button: " + param);
                }
                mouse_utils::click(button);
            }
            else if (command == "MOVE" || command == "MOUSE_MOVE") {
                std::vector<String> params = splitParams(line);
                if (params.size() != 2) {
                    return setError(errorMsg, "SCROLL requires 2 arguments: vertical horizontal");
                }

                int vertical = params[0].toInt();
                int horizontal = params[1].toInt();

                mouse_utils::move(vertical, horizontal);
            }
            else if (command == "SCROLL" || command == "MOUSE_SCROLL") {
                mouse_utils::scroll(param.toInt());
            }
            else if (command == "MOUSEPRESS" || command == "MOUSE_PRESS") {
                if (param.isEmpty()) {
                    return setError(errorMsg, command + " requires 1 parameter (LEFT, RIGHT, MIDDLE, etc.)");
                }
                uint8_t button = mouse_utils::getMouseButton(param);
                if (button == 0) {
                    return setError(errorMsg, "Invalid button: " + param);
                }
                mouse_utils::press(button);
            }
            else if (command == "MOUSERELEASE" || command == "MOUSE_RELEASE") {
                if (param.isEmpty()) {
                    return setError(errorMsg, command + " requires 1 parameter (LEFT, RIGHT, MIDDLE, etc.)");
                }
                uint8_t button = mouse_utils::getMouseButton(param);
                if (button == 0) {
                    return setError(errorMsg, "Invalid button: " + param);
                }
                mouse_utils::release(button);
            }
            else if (command == "LOCALE") {
                if      (param == "DE") keyboard_utils::setLayout(keyboard_utils::LAYOUT_DE);
                else if (param == "US") keyboard_utils::setLayout(keyboard_utils::LAYOUT_US);
                else if (param == "ES") keyboard_utils::setLayout(keyboard_utils::LAYOUT_ES);
                else if (param == "FR") keyboard_utils::setLayout(keyboard_utils::LAYOUT_FR);
                else if (param == "IT") keyboard_utils::setLayout(keyboard_utils::LAYOUT_IT);
                else if (param == "PT") keyboard_utils::setLayout(keyboard_utils::LAYOUT_PT);
                else if (param == "SE") keyboard_utils::setLayout(keyboard_utils::LAYOUT_SE);
                else if (param == "DK") keyboard_utils::setLayout(keyboard_utils::LAYOUT_DK);
                else {
                    return setError(errorMsg, "Unknown keyboard LOCALE: " + param);
                }
            }
            else {
                return setError(errorMsg, "Unknown command: " + command);
            }
        }

        if (!isDelay && defaultDelay > 0) delay(defaultDelay);
        return true;
    }

    bool execute(const String& script, String& errorMsg) {
        if (script.length() == 0) {
            setError(errorMsg, "Empty script");
            return false;
        }

        std::vector<String> lines;
        int start = 0;
        int end = script.indexOf('\n');

        while (end != -1) {
            lines.push_back(script.substring(start, end));
            start = end + 1;
            end = script.indexOf('\n', start);
        }
        lines.push_back(script.substring(start)); // last line

        for (int i = 0; i < lines.size(); i++) {
            String line = cleanLine(lines[i]);
            if (line.isEmpty()) continue;
            
            if (line.startsWith("REPEAT") || line.startsWith("REPLAY")) {
                String command = cleanLine(line.substring(0, line.indexOf(' ')));
                String param = cleanLine(line.substring(line.indexOf(' ') + 1));
                int replay = param.toInt();
                String endKeywork = command == "REPEAT" ? "ENDREPEAT" : "ENDREPLAY";

                int j = i + 1;
                String bufferCommands;
                while(j < lines.size() && !lines[j].startsWith(endKeywork)) {
                    bufferCommands += lines[j] + "\n";
                    j++;
                }

                if (j >= lines.size()) {
                    return setError(errorMsg, "Missing " + endKeywork);
                }
                
                for (int n = 0; n < replay; n++)  {
                    if (!execute(bufferCommands, errorMsg)) return false;
                }

                i = j;
                continue;
            }

            if (!parser(line, errorMsg)) return false;
        }
        return true;
    }
}