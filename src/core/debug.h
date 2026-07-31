#pragma once
#include <Arduino.h>

#ifdef DEBUG
    #define STARTUP_DELAY 4000
#else
    #define STARTUP_DELAY 1000
#endif

#ifdef DEBUG
    #define debug(msg) Serial.print(msg)
    #define debugln(msg) Serial.println(msg)
    #define debugf(fmt, ...) Serial.printf(fmt, ##__VA_ARGS__)
#else
    #define debug(...)
    #define debugln(...)
    #define debugf(...)
#endif