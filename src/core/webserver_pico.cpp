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

WebServer server(WEB_SERVER_PORT);

namespace {
    static constexpr webserver::Header _cache_control = {.key = "Cache-Control", .value = "max-age=3600"};

    void reply(WebServer* server, int code, const char* content_type, const void* content, size_t contentLength, std::initializer_list<webserver::Header> headers) {
        server->sendHeader("Content-Encoding", "gzip");
        for (const auto& header: headers) {
                server->sendHeader(header.key, header.value);
        }
        server->send_P(code, content_type, (const char*)content, contentLength);
    }

    void handleIndex() {
        reply(&server, 200, "text/html", PAGE_INDEX_HTML_GZ, sizeof(PAGE_INDEX_HTML_GZ), {_cache_control});
    }

    void handleSettings() {
        reply(&server, 200, "text/html", PAGE_SETTINGS_HTML_GZ, sizeof(PAGE_SETTINGS_HTML_GZ), {_cache_control});
    }

    void handleMainJS() {
        reply(&server, 200, "application/javascript", PAGE_MAIN_JS_GZ, sizeof(PAGE_MAIN_JS_GZ), {_cache_control});
    }

    void handleSettingsJS() {
        reply(&server, 200, "application/javascript", PAGE_SETTINGS_JS_GZ, sizeof(PAGE_SETTINGS_JS_GZ), {_cache_control});
    }

    void handleEditorJS() {
        reply(&server, 200, "application/javascript", PAGE_EDITOR_JS_GZ, sizeof(PAGE_EDITOR_JS_GZ), {_cache_control});
    }

    void handleCSS() {
        reply(&server, 200, "text/css", PAGE_STYLE_CSS_GZ, sizeof(PAGE_STYLE_CSS_GZ), {_cache_control});
    }

    void handle404() {
        reply(&server, 404, "text/html", PAGE_404_HTML_GZ, sizeof(PAGE_404_HTML_GZ), {_cache_control});
    }

    void handleDuckyScript() {
        if (server.hasArg("script")) {
            String errorMsg;
            String script = server.arg("script");
            debugf("Script recieved:\n%s\n", script.c_str());
            
            uint32_t startTime = millis();
            bool success = duckyparser::execute(script, errorMsg);
            uint32_t execTime = millis() - startTime;

            String response = success ? "Script executed successfully " : "[Error] " + errorMsg;

            if (execTime < 1000) {
                response += "(" + String(execTime) + "ms)";
            }
            else if (execTime < 60000) {
                float seconds = execTime / 1000.0f;
                response += "(" + String(seconds, 2) + "s)";
            }
            else {
                unsigned int minutes = execTime / 60000;
                float seconds = (execTime % 60000) / 1000.0f;
                response += "(" + String(minutes) + "m " + String(seconds, 1) + "s)";
            }
            
            server.send(success ? 200 : 400, "text/html", response);
        } else {
            server.send(400, "text/html", "Error: missing script parameter.");
        }
    }

    void getSettings() {
        server.send(200, "application/json", settings::getSettingsJson());
    }

    void postSettings() {
        if (!server.hasArg("plain")) {
            server.send(400, "text/plain", "Missing JSON body");
            return;
        }

        String json = server.arg("plain");
        
        if (!settings::setSettingsFromJson(json)) {
            server.send(400, "text/plain", "Invalid JSON");
            return;
        }

        settings::save();
        server.send(200, "text/plain", "Settings saved");
    }

    void getInfos() {
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


        uint32_t total_heap = rp2040.getTotalHeap();
        uint32_t free_heap = rp2040.getFreeHeap();
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
        server.send(200, "application/json", finalJson);
    }

    void reboot() {
        server.send(200, "text/plain", "Rebooting...");
        delay(100);
        watchdog_reboot(0, 0, 0);
        while (1);
    }

    void initRoutes() {
        // files
        server.on("/", HTTP_GET, handleIndex);
        server.on("/index.html", HTTP_GET, handleIndex);
        server.on("/settings.html", HTTP_GET, handleSettings);
        server.on("/main.js", HTTP_GET, handleMainJS);
        server.on("/settings.js", HTTP_GET, handleSettingsJS);
        server.on("/editor.js", HTTP_GET, handleEditorJS);
        server.on("/style.css", HTTP_GET, handleCSS);

        // routes
        server.on("/run", HTTP_POST, handleDuckyScript);
        server.on("/settings", HTTP_GET, getSettings);
        server.on("/settings", HTTP_POST, postSettings);
        server.on("/info", HTTP_GET, getInfos);
        server.on("/reboot", HTTP_POST, reboot);
        server.on("/reset", HTTP_POST, []() {
            LittleFS.remove("/config.json");
            delay(200);
            reboot();
        });
        server.onNotFound(handle404);
    }
}

void webserver::init() {
    initRoutes();
    server.begin();
    debugln("HTTP server started !");
}