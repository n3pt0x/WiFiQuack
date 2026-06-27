#pragma once

#include <WebServer.h>

const int WEB_SERVER_PORT = 80;
extern WebServer server;

void initWebServer();
void initRoutes();
void handleIndex();
void handleJS();
void handleCSS();
void handleDuckyScript();
void handle404();