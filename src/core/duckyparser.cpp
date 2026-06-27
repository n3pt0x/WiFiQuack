#include "duckyparser.h"

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
        // Modifiers
        if      (command == "GUI" || command == "WINDOWS")      keyboard_utils::writeKey(KEY_LEFT_GUI);
        else if (command == "CTRL" || command == "CONTROL")     keyboard_utils::writeKey(KEY_LEFT_CTRL);
        else if (command == "SHIFT")                            keyboard_utils::writeKey(KEY_LEFT_SHIFT);
        else if (command == "ALT")                              keyboard_utils::writeKey(KEY_LEFT_ALT);
        
        // Keys
        else if (command == "ENTER")                            keyboard_utils::writeKey(KEY_RETURN);
        else if (command == "UP" || command == "UPARROW")       keyboard_utils::writeKey(KEY_UP_ARROW);
        else if (command == "DOWN" || command == "DOWNARROW")   keyboard_utils::writeKey(KEY_DOWN_ARROW);
        else if (command == "LEFT" || command == "LEFTARROW")   keyboard_utils::writeKey(KEY_LEFT_ARROW);
        else if (command == "RIGHT" || command == "RIGHTARROW") keyboard_utils::writeKey(KEY_RIGHT_ARROW);
        else if (command == "BACKSPACE")                        keyboard_utils::writeKey(KEY_BACKSPACE);
        else if (command == "TAB")                              keyboard_utils::writeKey(KEY_TAB);
        else if (command == "MENU")                             keyboard_utils::writeKey(KEY_MENU);
        else if (command == "ESC" || command == "ESCAPE")       keyboard_utils::writeKey(KEY_ESC);
        else if (command == "INSERT")                           keyboard_utils::writeKey(KEY_INSERT);
        else if (command == "DELETE")                           keyboard_utils::writeKey(KEY_DELETE);
        else if (command == "PAGEUP")                           keyboard_utils::writeKey(KEY_PAGE_UP);
        else if (command == "PAGEDOWN")                         keyboard_utils::writeKey(KEY_PAGE_DOWN);
        else if (command == "HOME")                             keyboard_utils::writeKey(KEY_HOME);
        else if (command == "END")                              keyboard_utils::writeKey(KEY_END);
        else if (command == "CAPSLOCK")                         keyboard_utils::writeKey(KEY_CAPS_LOCK);
        else if (command == "PRINTSCREEN")                      keyboard_utils::writeKey(KEY_PRINT_SCREEN);
        else if (command == "SCROLLLOCK")                       keyboard_utils::writeKey(KEY_SCROLL_LOCK);
        else if (command == "PAUSE")                            keyboard_utils::writeKey(KEY_PAUSE);

        // Function keys
        else if (command == "F1")                               keyboard_utils::writeKey(KEY_F1);
        else if (command == "F2")                               keyboard_utils::writeKey(KEY_F2);
        else if (command == "F3")                               keyboard_utils::writeKey(KEY_F3);
        else if (command == "F4")                               keyboard_utils::writeKey(KEY_F4);
        else if (command == "F5")                               keyboard_utils::writeKey(KEY_F5);
        else if (command == "F6")                               keyboard_utils::writeKey(KEY_F6);
        else if (command == "F7")                               keyboard_utils::writeKey(KEY_F7);
        else if (command == "F8")                               keyboard_utils::writeKey(KEY_F8);
        else if (command == "F9")                               keyboard_utils::writeKey(KEY_F9);
        else if (command == "F10")                              keyboard_utils::writeKey(KEY_F10);
        else if (command == "F11")                              keyboard_utils::writeKey(KEY_F11);
        else if (command == "F12")                              keyboard_utils::writeKey(KEY_F12);

        // Numeric keypad
        else if (command == "NUM_LOCK")                         keyboard_utils::writeKey(KEY_NUM_LOCK);
        else if (command == "NUM_ASTERISK")                     keyboard_utils::writeKey(KEY_KP_ASTERISK);
        else if (command == "NUM_ENTER")                        keyboard_utils::writeKey(KEY_KP_ENTER);
        else if (command == "NUM_MINUS")                        keyboard_utils::writeKey(KEY_KP_MINUS);
        else if (command == "NUM_PLUS")                         keyboard_utils::writeKey(KEY_KP_PLUS);
        else if (command == "NUM_0")                            keyboard_utils::writeKey(KEY_KP_0);
        else if (command == "NUM_1")                            keyboard_utils::writeKey(KEY_KP_1);
        else if (command == "NUM_2")                            keyboard_utils::writeKey(KEY_KP_2);
        else if (command == "NUM_3")                            keyboard_utils::writeKey(KEY_KP_3);
        else if (command == "NUM_4")                            keyboard_utils::writeKey(KEY_KP_4);
        else if (command == "NUM_5")                            keyboard_utils::writeKey(KEY_KP_5);
        else if (command == "NUM_6")                            keyboard_utils::writeKey(KEY_KP_6);
        else if (command == "NUM_7")                            keyboard_utils::writeKey(KEY_KP_7);
        else if (command == "NUM_8")                            keyboard_utils::writeKey(KEY_KP_8);
        else if (command == "NUM_9")                            keyboard_utils::writeKey(KEY_KP_9);
        else if (command == "NUM_DOT")                          keyboard_utils::writeKey(KEY_KP_DOT);

        // Power Control
        else if (command == "POWER")                            keyboard_utils::writeKey(KEY_POWER);
        else if (command == "RESET")                            keyboard_utils::writeKey(KEY_RESET);
        else if (command == "SLEEP")                            keyboard_utils::writeKey(KEY_SLEEP);

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

            if (command == "REM" || command == "#"){} // do nothing
            else if (command == "STRING") keyboard_utils::sendString(param);
            else if (command == "DELAY") {
                int delayTime = param.toInt();
                if (delayTime > 0) delay(delayTime);
            }
            else if (command == "DEFAULTDELAY" || command == "DEFAULT_DELAY") {
                defaultDelay = param.toInt();
                if (defaultDelay < 0) defaultDelay = 0;
            }
            else if (command == "REPEAT" || command == "REPLAY") {
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
            else if (command == "KEYCODE") {
                int secondSpace = line.indexOf(' ', firstSpace + 1);

                if (secondSpace != -1) {
                    String modifierStr = line.substring(firstSpace + 1, secondSpace);
                    String keyStr = line.substring(secondSpace + 1);

                    uint8_t modifier = (uint8_t)strtol(modifierStr.c_str(), NULL, 0);
                    uint8_t key = (uint8_t)strtol(keyStr.c_str(), NULL, 0);

                    Keyboard.write(modifier);
                    Keyboard.write(key);
                } else {
                    Serial.println("KEYCODE: missing parameters");
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
                if      (param == "DE") keyboard_utils::setLayout(keyboard_utils::LAYOUT_DE);
                else if (param == "US") keyboard_utils::setLayout(keyboard_utils::LAYOUT_US);
                else if (param == "ES") keyboard_utils::setLayout(keyboard_utils::LAYOUT_ES);
                else if (param == "FR") keyboard_utils::setLayout(keyboard_utils::LAYOUT_FR);
                else if (param == "IT") keyboard_utils::setLayout(keyboard_utils::LAYOUT_IT);
                else if (param == "PT") keyboard_utils::setLayout(keyboard_utils::LAYOUT_PT);
                else if (param == "SE") keyboard_utils::setLayout(keyboard_utils::LAYOUT_SE);
                else if (param == "DK") keyboard_utils::setLayout(keyboard_utils::LAYOUT_DK);
                else {
                    Serial.print("Unknown locale: ");
                    Serial.println(param);
                }
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