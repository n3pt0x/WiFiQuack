#pragma once

#include <WebServer.h>

const int WEB_SERVER_PORT = 80;
extern WebServer server;

void initWebServer();
void initRoutes();
void handleIndex();
void handleSettings();
void handleMainJS();
void handleSettingsJS();
void handleCSS();
void handle404();
void handleDuckyScript();
void getSettings();
void postSettings();
void reboot();