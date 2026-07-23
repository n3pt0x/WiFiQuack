#pragma once
#include <Arduino.h>
#include "keyboard_def.h"

struct KeyMap {
    const char* name;
    uint8_t code;
};

const KeyMap KEY_MAP[] PROGMEM = {
    {"CTRL", KEY_LEFT_CTRL},
    {"CONTROL", KEY_LEFT_CTRL},
    {"SHIFT", KEY_LEFT_SHIFT},
    {"ALT", KEY_LEFT_ALT},
    {"GUI", KEY_LEFT_GUI},
    {"WINDOWS", KEY_LEFT_GUI},
    
    {"ENTER", KEY_RETURN},
    {"ESC", KEY_ESC},
    {"ESCAPE", KEY_ESC},
    {"BACKSPACE", KEY_BACKSPACE},
    {"TAB", KEY_TAB},
    {"SPACE", KEY_SPACE},
    {"MENU", KEY_MENU},
    {"INSERT", KEY_INSERT},
    {"DELETE", KEY_DELETE},
    {"HOME", KEY_HOME},
    {"END", KEY_END},
    {"PAGEUP", KEY_PAGE_UP},
    {"PAGEDOWN", KEY_PAGE_DOWN},
    {"CAPSLOCK", KEY_CAPS_LOCK},
    {"PRINTSCREEN", KEY_PRINT_SCREEN},
    {"SCROLLLOCK", KEY_SCROLL_LOCK},
    {"PAUSE", KEY_PAUSE},
    
    {"UP", KEY_UP_ARROW},
    {"UPARROW", KEY_UP_ARROW},
    {"DOWN", KEY_DOWN_ARROW},
    {"DOWNARROW", KEY_DOWN_ARROW},
    {"LEFT", KEY_LEFT_ARROW},
    {"LEFTARROW", KEY_LEFT_ARROW},
    {"RIGHT", KEY_RIGHT_ARROW},
    {"RIGHTARROW", KEY_RIGHT_ARROW},
    
    {"F1", KEY_F1},
    {"F2", KEY_F2},
    {"F3", KEY_F3},
    {"F4", KEY_F4},
    {"F5", KEY_F5},
    {"F6", KEY_F6},
    {"F7", KEY_F7},
    {"F8", KEY_F8},
    {"F9", KEY_F9},
    {"F10", KEY_F10},
    {"F11", KEY_F11},
    {"F12", KEY_F12},
    
    {"NUM_LOCK", KEY_NUM_LOCK},
    {"NUM_ASTERISK", KEY_KP_ASTERISK},
    {"NUM_ENTER", KEY_KP_ENTER},
    {"NUM_MINUS", KEY_KP_MINUS},
    {"NUM_PLUS", KEY_KP_PLUS},
    {"NUM_0", KEY_KP_0},
    {"NUM_1", KEY_KP_1},
    {"NUM_2", KEY_KP_2},
    {"NUM_3", KEY_KP_3},
    {"NUM_4", KEY_KP_4},
    {"NUM_5", KEY_KP_5},
    {"NUM_6", KEY_KP_6},
    {"NUM_7", KEY_KP_7},
    {"NUM_8", KEY_KP_8},
    {"NUM_9", KEY_KP_9},
    {"NUM_DOT", KEY_KP_DOT},
};

const int KEY_MAP_SIZE = sizeof(KEY_MAP) / sizeof(KEY_MAP[0]);

uint8_t getHIDCode(const String& name);