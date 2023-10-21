/*

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "AsyncElegantOTA.h"

#include "ota.h"

AsyncWebServer server(80);

void ota_init(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
            request->send(200, "text/plain", "Hi! I am ESP32.");
    });

    AsyncElegantOTA.begin(&server);
    server.begin();
}

void ota_tick(){
    AsyncElegantOTA.loop();
}


*/