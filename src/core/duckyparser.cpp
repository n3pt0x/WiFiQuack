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

    std::vector<String> splitParams(const String& line) {
        std::vector<String> params;
        String rest = line.substring(line.indexOf(' ') + 1);

        int pos = 0;
        while (pos < rest.length()) {
            while (pos < rest.length() && rest[pos] == ' ') pos++;
            if (pos >= rest.length()) break;
            
            int end = rest.indexOf(' ', pos);
            if (end == -1) end = rest.length();

            params.push_back(rest.substring(pos, end));
            pos = end + 1;
        }
            
        return params;
    }

    bool press(String command, String& errorMsg) {
        uint8_t code = getHIDCode(command);
        if (code != 0) {
            keyboard_utils::writeKey(code);
            return true;
        }

        // Power Control
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

            if (command == "REM" || command == "#"){} // do nothing
            else if (command == "STRING") keyboard_utils::sendString(param);
            else if (command == "DELAY") {
                int delayTime = param.toInt();
                if (delayTime < 0) {
                    return setError(errorMsg, "Invalid parameter for the " + command + " : " + param + ". " + command + " expects an integer parameter");
                }

                if (delayTime > 0) delay(delayTime);
                isDelay = true;
            }
            else if (command == "DEFAULTDELAY" || command == "DEFAULT_DELAY") {
                defaultDelay = param.toInt();
                if (defaultDelay < 0) {
                    return setError(errorMsg, "Invalid parameter for the " + command + " : " + param + ". " + command + " expects an integer parameter");
                }

                if (defaultDelay < 0) defaultDelay = 0;
            }
            else if (command == "REPEAT" || command == "REPLAY") {
                int replay = param.toInt();
                if (replay < 0) {
                    return setError(errorMsg, "Invalid parameter for the " + command + " : " + param + ". " + command + " expects an integer parameter");
                }

                for (int i = 0; i < replay; i++) {
                    if (!parser(lastLine, errorMsg)) {
                        return false;
                    }
                }
            }
            else if (command == "GUI" || command == "WINDOWS") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_GUI, param[0]);
                } else {
                    return setError(errorMsg, "Invalid parameter for the the command: " + command);
                }
            }
            else if (command == "CTRL" || command == "CONTROL") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_CTRL, param[0]);
                } else {
                    return setError(errorMsg, "Invalid parameter for the the command: " + command);
                }
            }
            else if (command == "KEYCODE") {
                int secondSpace = line.indexOf(' ', firstSpace + 1);

                if (secondSpace != -1) {
                    String modifierStr = line.substring(firstSpace + 1, secondSpace);
                    String keyStr = line.substring(secondSpace + 1);

                    uint8_t modifier = (uint8_t)strtol(modifierStr.c_str(), NULL, 0);
                    uint8_t key = (uint8_t)strtol(keyStr.c_str(), NULL, 0);

                    keyboard_utils::writeKey(modifier);
                    keyboard_utils::writeKey(key);
                } else {
                    return setError(errorMsg, "KEYCODE: missing parameters (e.g. KEYCODE 0x02 0x04)");
                }
            } else if (command == "COMBO") {
                std::vector<String> params = splitParams(line);
                    for (const auto& param : params) {
                        uint8_t code = getHIDCode(param);
                        if (code != 0) {
                            keyboard_utils::press(code);
                        }
                    }
                delay(50);
                keyboard_utils::releaseAll();
            }
            else if (command == "SHIFT") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_SHIFT, param[0]);
                } else {
                    return setError(errorMsg, "Invalid parameter for the the command: " + command);
                }
            }
            else if (command == "ALT") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_ALT, param[0]);
                } else {
                    return setError(errorMsg, "Invalid parameter for the the command: " + command);
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
                    return setError(errorMsg, "Unknown keyboard locale: " + param + ", choose between (DE, US, ES, FR, IT, PT, SE, DK)");
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
            return returnError(errorMsg);
        }

        int start = 0;
        int end = script.indexOf('\n');

        while (end != -1) {
            String line = cleanLine(script.substring(start, end));
            lastLine = line;

            if (line != "") {
                if (!parser(line, errorMsg)) {
                    return returnError(errorMsg);
                }
            }

            start = end + 1;
            end = script.indexOf('\n', start);
        }

        String lastLine = cleanLine(script.substring(start));
        if (lastLine != "") {
            if (!parser(lastLine, errorMsg)) {
                return returnError(errorMsg);
            }
        }

        return true;
    }
}