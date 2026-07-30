#include <Arduino.h>
#include <LittleFS.h>
#include "web_server.h"
#include "webfiles.h"
#include "duckyparser.h"
#include "settings.h"
#include "debug.h"
#include <vector>

AsyncWebServer server(WEB_SERVER_PORT);
std::vector<PendingRequest> pendingRequests;
const unsigned long TASK_TIMEOUT_MS = 60000;

void reply(AsyncWebServerRequest* request, int code, const char* content_type, const uint8_t* content, size_t contentLength, std::initializer_list<Header> headers) {
    AsyncWebServerResponse* response = request->beginResponse(code, content_type, content, contentLength);
    response->addHeader("Content-Encoding", "gzip");

    for (const auto& header: headers) {
        response->addHeader(header.key, header.value);
    }

    request->send(response);
}

void initWebServer() {
    initRoutes();
    server.begin();
    debugln("HTTP server started !");
}

void initRoutes() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        reply(request, 200, "text/html", PAGE_INDEX_HTML_GZ, sizeof(PAGE_INDEX_HTML_GZ), {cache_control});
    });
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        reply(request, 200, "text/html", PAGE_INDEX_HTML_GZ, sizeof(PAGE_INDEX_HTML_GZ), {cache_control});
    });
    server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        reply(request, 200, "text/html", PAGE_SETTINGS_HTML_GZ, sizeof(PAGE_SETTINGS_HTML_GZ), {cache_control});
    });
    server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        reply(request, 200, "application/javascript", PAGE_MAIN_JS_GZ, sizeof(PAGE_MAIN_JS_GZ), {cache_control});
    });
    server.on("/settings.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        reply(request, 200, "application/javascript", PAGE_SETTINGS_JS_GZ, sizeof(PAGE_SETTINGS_JS_GZ), {cache_control});
    });
    server.on("/editor.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        reply(request, 200, "application/javascript", PAGE_EDITOR_JS_GZ, sizeof(PAGE_EDITOR_JS_GZ), {cache_control});
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        reply(request, 200, "text/css", PAGE_STYLE_CSS_GZ, sizeof(PAGE_STYLE_CSS_GZ), {cache_control});
    });
    server.onNotFound([](AsyncWebServerRequest *request) {
        reply(request, 404, "text/html", PAGE_404_HTML_GZ, sizeof(PAGE_404_HTML_GZ), {cache_control});
    });

    server.on("/run", HTTP_POST, [](AsyncWebServerRequest *request) {
        handleDuckyScript(request);
    });
    server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
        getSettings(request);
    });
    server.on("/settings", HTTP_POST, [](AsyncWebServerRequest *request) {
        postSettings(request);
    });
    server.on("/reboot", HTTP_POST, [](AsyncWebServerRequest *request) {
        reboot(request);
    });
    server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
        LittleFS.remove("/config.json");
        delay(200);
        reboot(request);
    });
}

void handleDuckyScript(AsyncWebServerRequest *request) {
    if (!request->hasArg("script")) {
        request->send(400, "text/html", "Error: missing script parameter.");
        return;
    }

    if (!pendingRequests.empty()) {
        request->send(409, "text/plain", "A script is already running. Please wait.");
        return;
    }

    String script = request->arg("script");
    debugf("Script received:\n%s\n", script.c_str());

    AsyncWebServerRequestPtr ptr = request->pause();

    PendingRequest pending;
    pending.requestPtr = ptr;
    pending.script = script;
    pending.isRunning = true;
    pending.isCompleted = false;
    pending.success = false;
    pending.result = "";
    pending.startTime = millis();
    pendingRequests.push_back(pending);
}

void processPendingRequests() {
    for (auto it = pendingRequests.begin(); it != pendingRequests.end(); ) {
        PendingRequest& pending = *it;

        if (pending.isCompleted) {
            auto shared = pending.requestPtr.lock();
            if (shared) {
                String response = pending.success ? "Script executed successfully " : "[Error] " + pending.result;
                unsigned long execTime = millis() - pending.startTime;
                if (execTime >= 1000) {
                    float seconds = execTime / 1000.0f;
                    response += "(" + String(seconds, 2) + "s)";
                } else {
                    response += "(" + String(execTime) + "ms)";
                }
                shared->send(pending.success ? 200 : 400, "text/html", response);
            }
            it = pendingRequests.erase(it);
            continue;
        }

        if (pending.isRunning) {
            pending.isRunning = false;
            pending.startTime = millis();
            String errorMsg;
            pending.success = duckyparser::execute(pending.script, errorMsg);
            pending.result = errorMsg;
            pending.isCompleted = true;
        }

        if (millis() - pending.startTime > TASK_TIMEOUT_MS) {
            pending.success = false;
            pending.result = "Timeout (60s)";
            pending.isCompleted = true;
        }

        ++it;
    }
}

void getSettings(AsyncWebServerRequest *request) {
    request->send(200, "application/json", settings::getSettingsJson());
}

void postSettings(AsyncWebServerRequest *request) {
    if (!request->hasArg("plain")) {
        request->send(400, "text/plain", "Missing JSON body");
        return;
    }

    String json = request->arg("plain");
    if (!settings::setSettingsFromJson(json)) {
        request->send(400, "text/plain", "Invalid JSON");
        return;
    }

    settings::save();
    request->send(200, "text/plain", "Settings saved");
}

void reboot(AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Rebooting...");
    delay(100);
    #ifdef PLATFORM_ESP32
        #include <Esp.h>
        ESP.restart();
    #elif defined(PLATFORM_PICO)
        watchdog_reboot(0, 0, 0);
    #endif
    while (1);
}