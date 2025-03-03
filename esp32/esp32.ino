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
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  server.onNotFound([](AsyncWebServerRequest* request) {
    if(request->method() == HTTP_OPTIONS) {
      request->send(200);
      return;
    } 

    if(request->method() == HTTP_POST) {
      if(request->url() == "/updateDisplayPixels") {
        request->onBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
          DeserializationError error = deserializeJson(doc, data);
          JsonArray pixelArrayColors = doc["colorMatrixData"]["arrayColors"];
          
          for(int forY = 0; forY < 8; forY++) {
            JsonArray arrayRigaPannello = pixelArrayColors[forY];
            for(int forX = 0; forX < 32; forX++) {
              JsonObject pixelColor = arrayRigaPannello[forX];
              int R = pixelColor["R"];
              int G = pixelColor["G"];
              int B = pixelColor["B"];
              thisPannello.drawPixel(forX, forY, thisPannello.Color(R,G,B));
            };
          }
          thisPannello.show();
          doc.clear();
        });
        request->send(200, "text/plain", "Display updated");
        return;
      }
    }

    request->send(404,"text/plain", "send the correct POST request instead");
  });

  server.begin();
}

void loop() {}