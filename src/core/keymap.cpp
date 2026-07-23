#include "keymap.h"

uint8_t getHIDCode(const String& name) {
    for (int i = 0; i < KEY_MAP_SIZE; i++) {
        KeyMap entry;
        memcpy_P(&entry, &KEY_MAP[i], sizeof(KeyMap));
        if (name.equalsIgnoreCase(entry.name)) {
            return entry.code;
        }
    }
    return 0;
}