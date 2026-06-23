#include "web_server.h"

WebServer server(WEB_SERVER_PORT);

void setup() {
    initRoute();
    server.begin();
    Serial.println("HTTP server stated !");
}

void runWebServer(){
    setup();
}

void initRoute() {
    server.on("/", HTTP_GET, homePage);
    server.on("/run", HTTP_POST, runDuckyScript);
    server.onNotFound(notFound404);
}

void homePage() {

}

void runDuckyScript() {}

void notFound404() {
    const char notFound[] PROGMEM = R"EOD(
    <!DOCTYPE html>
        <head>
            <title>404 Not Found</title>
        </head>
        <html>
            <body>
                <h1>404 Not Found</h1>
            </body>
        </html>
    )EOD";
    server.send_P(404, "text/html", notFound);
}