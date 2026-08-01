#include "keymap.h"

uint8_t getHIDCode(const String& name, const KeyMap* map, size_t mapSize) {
    for (int i = 0; i < mapSize; i++) {
        KeyMap entry;
        memcpy_P(&entry, &map[i], sizeof(KeyMap));
        if (name.equalsIgnoreCase(entry.name)) {
            return entry.code;
        }
    }
    return 0;
}