#include <Arduino.h>
#include <LittleFS.h>
#include "web_server.h"
#include "webfiles.h"
#include "duckyparser.h"
#include "settings.h"

WebServer server(WEB_SERVER_PORT);

void reply(WebServer* server, int code, PGM_P content_type, PGM_P content, size_t contentLength) {
    server->sendHeader("Content-Encoding", "gzip");
    server->send_P(code, content_type, content, contentLength);
}

void initWebServer() {
    initRoutes();
    server.begin();
    Serial.println("HTTP server started !");
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
    server.on("/reboot", HTTP_POST, reboot);
    server.on("/reset", HTTP_POST, []() {
        LittleFS.remove("/config.json");
        delay(200);
        reboot();
    });
    server.onNotFound(handle404);
}

void handleIndex() {
    reply(&server, 200, "text/html", PAGE_INDEX_HTML_GZ, sizeof(PAGE_INDEX_HTML_GZ));
}

void handleSettings() {
    reply(&server, 200, "text/html", PAGE_SETTINGS_HTML_GZ, sizeof(PAGE_SETTINGS_HTML_GZ));
}

void handleMainJS() {
    reply(&server, 200, "application/javascript", PAGE_MAIN_JS_GZ, sizeof(PAGE_MAIN_JS_GZ));
}

void handleSettingsJS() {
    reply(&server, 200, "application/javascript", PAGE_SETTINGS_JS_GZ, sizeof(PAGE_SETTINGS_JS_GZ));
}

void handleEditorJS() {
    reply(&server, 200, "application/javascript", PAGE_EDITOR_JS_GZ, sizeof(PAGE_EDITOR_JS_GZ));
}

void handleCSS() {
    reply(&server, 200, "text/css", PAGE_STYLE_CSS_GZ, sizeof(PAGE_STYLE_CSS_GZ));
}

void handle404() {
    reply(&server, 404, "text/html", PAGE_404_HTML_GZ, sizeof(PAGE_404_HTML_GZ));
}

void handleDuckyScript() {
    if (server.hasArg("script")) {
        String errorMsg;
        String script = server.arg("script");
        Serial.printf("Script recieved:\n%s\n", script.c_str());
        
        if (duckyparser::execute(script, errorMsg)) {
            server.send(200, "text/html", "Script executed successfully.");
        } else {
            server.send(400, "text/html", "[Error] " + errorMsg);
        }

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

void reboot() {
    server.send(200, "text/plain", "Rebooting...");
    delay(100);
    #ifdef PLATFORM_ESP32
        #include <Esp.h>
        ESP.restart();
    #elif defined(PLATFORM_PICO)
        watchdog_reboot(0, 0, 0);
    #endif
    while (1);
}