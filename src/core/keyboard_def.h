#pragma once

#ifdef PLATFORM_ESP32
    #include <USB.h>
    #include <USBHIDKeyboard.h>
#elif defined(PLATFORM_PICO)
    #include <Keyboard.h>
    #include <HID_Keyboard.h>
#endif