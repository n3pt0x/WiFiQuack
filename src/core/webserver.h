#ifdef PLATFORM_ESP32
#pragma once
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <initializer_list>

#define WEB_SERVER_PORT 80

namespace webserver {
    struct Header {
        const char* key;
        const char* value;
    };

    void init();
    void processPendingRequests();
}

struct PendingRequest {
    AsyncWebServerRequestPtr requestPtr;
    String script;
    bool isRunning;
    bool isCompleted;
    String result;
    bool success;
    unsigned long startTime;
};
#elifdef PLATFORM_PICO
#pragma once
#include <WebServer.h>

const int WEB_SERVER_PORT = 80;
extern WebServer server;

namespace webserver {
    struct Header {
        const char* key;
        const char* value;
    };

    void init();
}
#endif