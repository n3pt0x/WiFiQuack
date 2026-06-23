#ifndef WEB_SERVER_H
#define WEB_SERVER_H

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

#endif