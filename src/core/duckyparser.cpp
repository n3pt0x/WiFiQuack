#include "duckyparser.h"

namespace duckyparser {

    int defaultDelay = 0;
    String lastLine = "";
    Layout currentLayout = LAYOUT_US;

    void setLayout(Layout layout) {
        currentLayout = layout;
        Keyboard.end();
        switch (layout) {
            case LAYOUT_FR:
                Keyboard.begin(KeyboardLayout_fr_FR);
                break;
            case LAYOUT_US:
            default:
                Keyboard.begin(KeyboardLayout_en_US);
                break;
        }
    }

    void begin(Layout initialLayout) {
        setLayout(initialLayout);
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
            Keyboard.press(KEY_LEFT_GUI);
            delay(50);
            Keyboard.release(KEY_LEFT_GUI);
        }
        else if (command == "CTRL" || command == "CONTROL") {
            Keyboard.press(KEY_LEFT_CTRL);
            delay(50);
            Keyboard.release(KEY_LEFT_CTRL);
        }
        else if (command == "SHIFT") {
            Keyboard.press(KEY_LEFT_SHIFT);
            delay(50);
            Keyboard.release(KEY_LEFT_SHIFT);
        }
        else if (command == "ALT") {
            Keyboard.press(KEY_LEFT_ALT);
            delay(50);
            Keyboard.release(KEY_LEFT_ALT);
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
                    Keyboard.press(KEY_LEFT_GUI);
                    Keyboard.print(param[0]);
                    Keyboard.releaseAll();
                }
            }
            else if (command == "CTRL" || command == "CONTROL") {
                if (param.length() == 1) {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.print(param[0]);
                    Keyboard.releaseAll();
                }
            }
            else if (command == "SHIFT") {
                if (param.length() == 1) {
                    Keyboard.press(KEY_LEFT_SHIFT);
                    Keyboard.print(param[0]);
                    Keyboard.releaseAll();
                }
            }
            else if (command == "ALT") {
                if (param.length() == 1) {
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.print(param[0]);
                    Keyboard.releaseAll();
                }
            }
            else if (command == "LOCALE") {
                if (param == "FR") {
                    setLayout(LAYOUT_FR);
                } else if (param == "US") {
                    setLayout(LAYOUT_US);
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