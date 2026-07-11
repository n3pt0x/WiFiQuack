#pragma once

#ifdef PLATFORM_ESP32
    #include <USB.h>
    #include <USBHIDKeyboard.h>
    #ifndef KEY_POWER
    #define KEY_POWER 0x66
    #endif
    
    #ifndef KEY_SLEEP
    #define KEY_SLEEP 0x82
    #endif
    
    #ifndef KEY_RESET
    #define KEY_RESET 0x9A
    #endif
#elif defined(PLATFORM_PICO)
    #include <Keyboard.h>
    #include <HID_Keyboard.h>
#endif