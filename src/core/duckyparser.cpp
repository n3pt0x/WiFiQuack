#include "duckyparser.h"
#include "keyboard_utils.h"

namespace duckyparser {

    int defaultDelay = 0;
    String lastLine = "";

    void begin(keyboard_utils::Layout initialLayout) {
        keyboard_utils::setLayout(initialLayout);
        reset();
    }

    void reset() {
        defaultDelay = 0;
        lastLine = "";
    }

    void press(String command) {
        if (command == "ENTER") {
            Keyboard.write(KEY_RETURN);
        }
        else if (command == "GUI" || command == "WINDOWS") {
            keyboard_utils::pressKey(KEY_LEFT_GUI);
        }
        else if (command == "CTRL" || command == "CONTROL") {
            keyboard_utils::pressKey(KEY_LEFT_CTRL);
        }
        else if (command == "SHIFT") {
            keyboard_utils::pressKey(KEY_LEFT_SHIFT);
        }
        else if (command == "ALT") {
            keyboard_utils::pressKey(KEY_LEFT_ALT);
        }
        else if (command == "TAB") {
            Keyboard.write(KEY_TAB);
        }
        else if (command == "ESC" || command == "ESCAPE") {
            Keyboard.write(KEY_ESC);
        }
        else if (command == "BACKSPACE" || command == "DELETE") {
            Keyboard.write(KEY_BACKSPACE);
        }
        else if (command == "UP" || command == "UPARROW") {
            Keyboard.write(KEY_UP_ARROW);
        }
        else if (command == "DOWN" || command == "DOWNARROW") {
            Keyboard.write(KEY_DOWN_ARROW);
        }
        else if (command == "LEFT" || command == "LEFTARROW") {
            Keyboard.write(KEY_LEFT_ARROW);
        }
        else if (command == "RIGHT" || command == "RIGHTARROW") {
            Keyboard.write(KEY_RIGHT_ARROW);
        }
        else {
            Serial.print("Unknown command: ");
            Serial.println(command);
        }
    }

    void parser(String line) {
        int firstSpace = line.indexOf(' ');

        if (firstSpace == -1) {
            press(line);
        } else {
            String command = line.substring(0, firstSpace);
            String param = line.substring(firstSpace + 1);
            param.trim();

            if (command == "REM" || command == "#") {
                // comment
            }
            else if (command == "STRING") {
                Keyboard.print(param);
            }
            else if (command == "DELAY") {
                int delayTime = param.toInt();
                if (delayTime > 0) delay(delayTime);
            }
            else if (command == "DEFAULTDELAY") {
                defaultDelay = param.toInt();
                if (defaultDelay < 0) defaultDelay = 0;
            }
            else if (command == "REPLAY") {
                int replay = param.toInt();
                for (int i = 0; i < replay; i++) {
                    parser(lastLine);
                }
            }
            else if (command == "GUI" || command == "WINDOWS") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_GUI, param[0]);
                }
            }
            else if (command == "CTRL" || command == "CONTROL") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_CTRL, param[0]);
                }
            }
            else if (command == "SHIFT") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_SHIFT, param[0]);
                }
            }
            else if (command == "ALT") {
                if (param.length() == 1) {
                    keyboard_utils::pressCombination(KEY_LEFT_ALT, param[0]);
                }
            }
            else if (command == "LOCALE") {
                if (param == "FR") {
                    keyboard_utils::setLayout(keyboard_utils::LAYOUT_FR);
                } else if (param == "US") {
                    keyboard_utils::setLayout(keyboard_utils::LAYOUT_US);
                } else {
                    Serial.print("Unknown locale: ");
                    Serial.println(param);
                }
            }
            else {
                Serial.print("Unknown command: ");
                Serial.println(command);
            }
        }

        if (defaultDelay > 0) delay(defaultDelay);
    }

    bool execute(const String& script) {
        if (script.length() == 0) {
            Serial.println("execute: empty script");
            return false;
        }

        int start = 0;
        int end = script.indexOf('\n');

        while (end != -1) {
            String line = script.substring(start, end);
            line.trim();
            lastLine = line;

            if (line != "") {
                parser(line);
            }

            start = end + 1;
            end = script.indexOf('\n', start);
        }

        String lastLine = script.substring(start);
        lastLine.trim();
        if (lastLine != "") {
            parser(lastLine);
        }

        return true;
    }

}