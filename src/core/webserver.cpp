#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <vector>
#include "webserver.h"
#include "webfiles.h"
#include "ducky/duckyparser.h"
#include "wifi_manager.h"
#include "config.h"
#include "settings.h"
#include "debug.h"

namespace {
    static constexpr webserver::Header _cache_control = {.key = "Cache-Control", .value = "max-age=3600"};
    AsyncWebServer _server(WEB_SERVER_PORT);
    std::vector<PendingRequest> _pendingRequests;
    const unsigned long _TASK_TIMEOUT_MS = 300000;

    void reply(AsyncWebServerRequest* request, int code, const char* content_type, const uint8_t* content, size_t contentLength, std::initializer_list<webserver::Header> headers) {
        AsyncWebServerResponse* response = request->beginResponse(code, content_type, content, contentLength);
        response->addHeader("Content-Encoding", "gzip");

        for (const auto& header: headers) {
            response->addHeader(header.key, header.value);
        }

        request->send(response);
    }

    void handleDuckyScript(AsyncWebServerRequest *request) {
        if (!request->hasArg("script")) {
            request->send(400, "text/html", "Error: missing script parameter.");
            return;
        }

        if (!_pendingRequests.empty()) {
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
        _pendingRequests.push_back(pending);
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

    void getInfos(AsyncWebServerRequest *request) {
        JsonDocument docJson;
        JsonObject network = docJson["network"].to<JsonObject>();
        JsonObject wifi = docJson["wifi"].to<JsonObject>();
        JsonObject heap = docJson["heap"].to<JsonObject>();
        
        // Network
        network["ip_addr"] = wifi::getIP().toString();
        network["subnetmask"] = wifi::getSubnetMask().toString();
        network["gateway"] = wifi::getGateway().toString();
        network["mac_addr"] = wifi::getMAC();

        // WiFi
        wifi["ssid"] = settings::getWiFiSSID();
        wifi["channel"] = wifi::getChannel();

        #ifdef PLATFORM_ESP32
            // Heap
            uint32_t total_heap = ESP.getHeapSize();
            uint32_t free_heap = ESP.getFreeHeap();
            // LittleFS
            JsonObject littleFSJson = docJson["littleFS"].to<JsonObject>();
            size_t littleFS_total = LittleFS.totalBytes();
            size_t littleFS_used = LittleFS.usedBytes();
            littleFSJson["total"] = littleFS_total;
            littleFSJson["used"] = littleFS_used;
            littleFSJson["free"] = littleFS_total - littleFS_used;
        #elif defined(PLATFORM_PICO)
            uint32_t total_heap = rp2040.getTotalHeap();
            uint32_t free_heap = rp2040.getFreeHeap();
        #endif
        uint32_t used_heap = total_heap - free_heap;

        // Heap
        heap["total"] = total_heap;
        heap["used"] = used_heap;
        heap["free"] = free_heap;

        unsigned long endTime = millis() - START_TIME_BOARD;
        String timeStr;
        if (endTime < 1000) {
            timeStr = String(endTime) + "ms";
        } else {
            unsigned long totalSeconds = endTime / 1000;
            unsigned long minutes = totalSeconds / 60;
            unsigned long secondes = totalSeconds % 60;
            unsigned long hours = totalSeconds / 3600;

            if (endTime < 60000) {
                timeStr = String(totalSeconds) + "s";
            } else if (endTime < 3600000) {
                timeStr = String(minutes) + "m " + String(secondes) + "s";
            } else {
                unsigned long remainingMinutes = (totalSeconds % 3600) / 60;
                timeStr = String(hours) + "h " + String(remainingMinutes) + "m " + String(secondes) + "s";
            }
        }

        docJson["uptime"] = timeStr;

        String finalJson;
        serializeJson(docJson, finalJson);
        
        request->send(200, "application/json", finalJson);
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

    void initRoutes() {
        _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            reply(request, 200, "text/html", PAGE_INDEX_HTML_GZ, sizeof(PAGE_INDEX_HTML_GZ), {_cache_control});
        });
        _server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request) {
            reply(request, 200, "text/html", PAGE_INDEX_HTML_GZ, sizeof(PAGE_INDEX_HTML_GZ), {_cache_control});
        });
        _server.on("/settings.html", HTTP_GET, [](AsyncWebServerRequest *request) {
            reply(request, 200, "text/html", PAGE_SETTINGS_HTML_GZ, sizeof(PAGE_SETTINGS_HTML_GZ), {_cache_control});
        });
        _server.on("/main.js", HTTP_GET, [](AsyncWebServerRequest *request) {
            reply(request, 200, "application/javascript", PAGE_MAIN_JS_GZ, sizeof(PAGE_MAIN_JS_GZ), {_cache_control});
        });
        _server.on("/settings.js", HTTP_GET, [](AsyncWebServerRequest *request) {
            reply(request, 200, "application/javascript", PAGE_SETTINGS_JS_GZ, sizeof(PAGE_SETTINGS_JS_GZ), {_cache_control});
        });
        _server.on("/editor.js", HTTP_GET, [](AsyncWebServerRequest *request) {
            reply(request, 200, "application/javascript", PAGE_EDITOR_JS_GZ, sizeof(PAGE_EDITOR_JS_GZ), {_cache_control});
        });
        _server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
            reply(request, 200, "text/css", PAGE_STYLE_CSS_GZ, sizeof(PAGE_STYLE_CSS_GZ), {_cache_control});
        });
        _server.onNotFound([](AsyncWebServerRequest *request) {
            reply(request, 404, "text/html", PAGE_404_HTML_GZ, sizeof(PAGE_404_HTML_GZ), {_cache_control});
        });

        _server.on("/run", HTTP_POST, [](AsyncWebServerRequest *request) {
            handleDuckyScript(request);
        });
        _server.on("/settings", HTTP_GET, [](AsyncWebServerRequest *request) {
            getSettings(request);
        });
        _server.on("/settings", HTTP_POST, [](AsyncWebServerRequest *request) {
            postSettings(request);
        });
        _server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
            getInfos(request);
        });
        _server.on("/reboot", HTTP_POST, [](AsyncWebServerRequest *request) {
            reboot(request);
        });
        _server.on("/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
            LittleFS.remove("/config.json");
            delay(200);
            reboot(request);
        });
    }
}

void webserver::init() {
    initRoutes();
    _server.begin();
    debugln("HTTP server started !");
}

void webserver::processPendingRequests() {
    for (auto it = _pendingRequests.begin(); it != _pendingRequests.end(); ) {
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
            it = _pendingRequests.erase(it);
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

        if (millis() - pending.startTime > _TASK_TIMEOUT_MS) {
            pending.success = false;
            pending.result = "Timeout (60s)";
            pending.isCompleted = true;
        }

        ++it;
    }
}