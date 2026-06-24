#pragma once

#include <Arduino.h>
#include <Keyboard.h>
#include <HID_Keyboard.h>

struct AccentMap {
    const char* utf8;         // UTF-8 encoded character (e.g., "ê")
    uint8_t deadKey;          // HID code for the dead key (0x35 = ^, 0x2D = ´)
    uint8_t keyCode;          // HID code for the vowel (0x08 = E, 0x04 = A, etc.)
    uint8_t modifier;         // Modifier for dead key (0x00 = none, 0x02 = Shift)
};

const AccentMap accentTable[] = {
    // -------- Dead key: ^ (0x35) --------
    {"ê", 0x35, 0x08, 0x00},   // ^ + e
    {"â", 0x35, 0x04, 0x00},   // ^ + a
    {"î", 0x35, 0x0C, 0x00},   // ^ + i
    {"ô", 0x35, 0x12, 0x00},   // ^ + o
    {"û", 0x35, 0x18, 0x00},   // ^ + u
    {"Ê", 0x35, 0x08, 0x02},   // Shift + ^ + E
    {"Â", 0x35, 0x04, 0x02},   // Shift + ^ + A
    {"Î", 0x35, 0x0C, 0x02},   // Shift + ^ + I
    {"Ô", 0x35, 0x12, 0x02},   // Shift + ^ + O
    {"Û", 0x35, 0x18, 0x02},   // Shift + ^ + U

    // -------- Dead key: ¨ (0x35 + Shift) --------
    {"ë", 0x35, 0x08, 0x02},   // Shift + ^ + e (¨ = Shift + ^)
    {"ä", 0x35, 0x04, 0x02},   // Shift + ^ + a
    {"ï", 0x35, 0x0C, 0x02},   // Shift + ^ + i
    {"ö", 0x35, 0x12, 0x02},   // Shift + ^ + o
    {"ü", 0x35, 0x18, 0x02},   // Shift + ^ + u
    {"Ë", 0x35, 0x08, 0x02},   // Shift + ^ + E (¨ + E)
    {"Ä", 0x35, 0x04, 0x02},   // Shift + ^ + A
    {"Ï", 0x35, 0x0C, 0x02},   // Shift + ^ + I
    {"Ö", 0x35, 0x12, 0x02},   // Shift + ^ + O
    {"Ü", 0x35, 0x18, 0x02},   // Shift + ^ + U

    // -------- Dead key: ´ (0x2D + Shift) --------
    {"é", 0x2D, 0x08, 0x02},   // Shift + - + e
    {"É", 0x2D, 0x08, 0x02},   // Shift + - + E

    // -------- Dead key: ` (0x35) --------
    {"è", 0x35, 0x08, 0x00},   // ` + e (on AZERTY, ` is on key 0)
    {"à", 0x35, 0x04, 0x00},   // ` + a
    {"ù", 0x35, 0x18, 0x00},   // ` + u
    {"È", 0x35, 0x08, 0x02},   // Shift + ` + E
    {"À", 0x35, 0x04, 0x02},   // Shift + ` + A
    {"Ù", 0x35, 0x18, 0x02},   // Shift + ` + U

    // -------- Cédille (key 9) --------
    {"ç", 0x26, 0x00, 0x00},   // 9 (no dead key)
    {"Ç", 0x26, 0x00, 0x02},   // Shift + 9
};

namespace keyboard_utils {

    enum Layout : uint8_t {
        LAYOUT_US,
        LAYOUT_FR
    };

    void setLayout(Layout layout);
    void pressKey(uint8_t key);
    void pressCombination(uint8_t modifier, uint8_t key);
    void handleChar(String string);
};