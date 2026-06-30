#pragma once

const long SERIAL_BAUD = 115200;

#ifdef DEBUG
    #define STARTUP_DELAY 4000
#else
    #define STARTUP_DELAY 1000
#endif