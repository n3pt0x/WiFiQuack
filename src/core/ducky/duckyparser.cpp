#include "duckyparser.h"
#include "helpers.h"
#include "../keyboard_utils.h"
#include "../mouse_utils.h"
#include "../keymap.h"
#include "../utils.h"

namespace {
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
                keyboard_utils::print(param);
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
                duckyparser::defaultDelay = param.toInt();
                if (duckyparser::defaultDelay < 0) {
                    return setError(errorMsg, "Invalid parameter for " + command + " : " + param);
                }
                if (duckyparser::defaultDelay < 0) duckyparser::defaultDelay = 0;
            }
            else if (helpers::handleModifierKey(command, param, errorMsg)) return true;
            else if (command == "COMBO") {
                return helpers::handleCombo(line, errorMsg);
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
            else if (helpers::handleMouseCommand(command, param, line, errorMsg)) return true;
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

        if (!isDelay && duckyparser::defaultDelay > 0) delay(duckyparser::defaultDelay);
        return true;
    }
}

namespace duckyparser {
    int defaultDelay = 0;

    void reset() {
        defaultDelay = 0;
    };

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