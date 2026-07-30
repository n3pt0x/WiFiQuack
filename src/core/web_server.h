#pragma once
#include <ESPAsyncWebServer.h>
#include <Arduino.h>

#define WEB_SERVER_PORT 80

extern AsyncWebServer server;

void initWebServer();
void initRoutes();
void reply(AsyncWebServerRequest* request, int code, const char* content_type, const uint8_t* content, size_t contentLength);
void handleDuckyScript(AsyncWebServerRequest *request);
void getSettings(AsyncWebServerRequest *request);
void postSettings(AsyncWebServerRequest *request);
void reboot(AsyncWebServerRequest *request);
void processPendingRequests();

struct PendingRequest {
    AsyncWebServerRequestPtr requestPtr;
    String script;
    bool isRunning;
    bool isCompleted;
    String result;
    bool success;
    unsigned long startTime;
};