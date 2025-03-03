#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoMatrix.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

Adafruit_NeoMatrix thisPannello = Adafruit_NeoMatrix(32,8,15,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
JsonDocument doc;

void setup() {
  Serial.begin(115200);

  thisPannello.begin();
  thisPannello.fill(0);
  thisPannello.setBrightness(10);

  WiFi.begin("nomeWifi", "passwordWifi");
  while (WiFi.status() != WL_CONNECTED) {};
  Serial.println("WiFi connected! IP Address: " + WiFi.localIP().toString());
  
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  server.on("/updateDisplayPixels", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "OK. CORS bypassed");
  });

  server.on("/updateDisplayPixels", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "YES. POST is working");
  });

  server.begin();
}

void loop() {}