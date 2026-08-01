#pragma once
#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <initializer_list>

#define WEB_SERVER_PORT 80

struct Header {
    const char* key;
    const char* value;
};

static constexpr Header cache_control = {.key = "Cache-Control", .value = "max-age=3600"};

namespace webserver {
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