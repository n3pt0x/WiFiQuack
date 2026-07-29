#include <Arduino.h>
#include "keyboard_def.h"
#include "keyboard_utils.h"
#include "keymap.h"
#include "duckyparser.h"
#include "utils.h"

namespace duckyparser {

    int defaultDelay = 0;
    String lastLine = "";

    void reset() {
        defaultDelay = 0;
        lastLine = "";
    }

    bool press(String command, String& errorMsg) {
        uint8_t code = getHIDCode(command);
        if (code != 0) {
            keyboard_utils::write(code);
            return true;
        }

        if (command == "POWER") {
            keyboard_utils::pressPower();
            return true;
        }
        if (command == "RESET") {
            keyboard_utils::pressReset();
            return true;
        }
        if (command == "SLEEP"){
            keyboard_utils::pressSleep();
            return true;
        }
        
        return setError(errorMsg, "Unknown command: " + command);
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
            else if (command == "REPEAT" || command == "REPLAY") {
                int replay = param.toInt();
                if (replay < 0) {
                    return setError(errorMsg, "Invalid parameter for " + command + " : " + param);
                }

                String lineToRepeat = lastLine;

                if (lineToRepeat.isEmpty()) {
                    return setError(errorMsg, "Nothing to repeat (last line is empty)");
                }

                if (lineToRepeat.startsWith("REPEAT") || lineToRepeat.startsWith("REPLAY")) {
                    return setError(errorMsg, "Cannot repeat a REPEAT command");
                }

                for (int i = 0; i < replay; i++) {
                    if (!parser(lineToRepeat, errorMsg)) {
                        return false;
                    }
                }
            }
            else if (command == "GUI" || command == "WINDOWS") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_GUI, param[0]);
                } else {
                    uint8_t HIDCode = getHIDCode(param);
                    if (HIDCode) {
                        keyboard_utils::press(KEY_LEFT_GUI);
                        keyboard_utils::press(HIDCode);
                        keyboard_utils::releaseAll();
                    } else {
                        return setError(errorMsg, "Invalid parameter for GUI: " + param);
                    }
                }
            }
            else if (command == "CTRL" || command == "CONTROL") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_CTRL, param[0]);
                } else {
                    uint8_t HIDCode = getHIDCode(param);
                    if (HIDCode) {
                        keyboard_utils::press(KEY_LEFT_CTRL);
                        keyboard_utils::press(HIDCode);
                        keyboard_utils::releaseAll();
                    } else {
                        return setError(errorMsg, "Invalid parameter for CTRL: " + param);
                    }
                }
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
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_SHIFT, param[0]);
                } else {
                    uint8_t HIDCode = getHIDCode(param);
                    if (HIDCode) {
                        keyboard_utils::press(KEY_LEFT_SHIFT);
                        keyboard_utils::press(HIDCode);
                        keyboard_utils::releaseAll();
                    } else {
                        return setError(errorMsg, "Invalid parameter for SHIFT: " + param);
                    }
                }
            }
            else if (command == "ALT") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_ALT, param[0]);
                } else {
                    uint8_t HIDCode = getHIDCode(param);
                    if (HIDCode) {
                        keyboard_utils::press(KEY_LEFT_ALT);
                        keyboard_utils::press(HIDCode);
                        keyboard_utils::releaseAll();
                    } else {
                        return setError(errorMsg, "Invalid parameter for ALT: " + param);
                    }
                }
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
                    return setError(errorMsg, "Unknown keyboard locale: " + param);
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

        int start = 0;
        int end = script.indexOf('\n');

        while (end != -1) {
            String line = cleanLine(script.substring(start, end));
            if (line != "") {
                if (!line.startsWith("REPEAT") && !line.startsWith("REPLAY")) {
                    lastLine = line;
                }
                if (!parser(line, errorMsg)) {
                    return false;
                }
            }
            start = end + 1;
            end = script.indexOf('\n', start);
        }

        String line = cleanLine(script.substring(start));
        if (line != "") {
            if (!line.startsWith("REPEAT") && !line.startsWith("REPLAY")) {
                lastLine = line;
            }
            if (!parser(line, errorMsg)) {
                return false;
            }
        }

        return true;
    }
}