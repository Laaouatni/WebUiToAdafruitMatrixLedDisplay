#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoMatrix.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

Adafruit_NeoMatrix thisPannello = Adafruit_NeoMatrix(32,8,15,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
JsonDocument doc;

String completeJsonString = "";

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

  server.on("/updateDisplayPixels", HTTP_POST, 
    [](AsyncWebServerRequest *request) {}, NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      const String thisStringData = String((char*)data).substring(0, len);
      completeJsonString += thisStringData;

      if(index+len == total) {
        DeserializationError error = deserializeJson(doc, completeJsonString);
        if(error) {
          request->send(400, "text/plain", "JSON Parsing Error: " + String(error.c_str()));
          return;
        };
        JsonArray pixelArrayColors = doc["arrayColors"];
        
        for(int forY = 0; forY < 8; forY++) {
          JsonArray arrayRigaPannello = pixelArrayColors[forY];
          for(int forX = 0; forX < 32; forX++) {
            long int t1 = millis();
            JsonArray pixelColor = arrayRigaPannello[forX];
            int R = pixelColor[0];
            int G = pixelColor[1];
            int B = pixelColor[2];

            thisPannello.drawPixel(forX, forY, thisPannello.Color(R,G,B));
            long int t2 = millis();
            Serial.println("ms: " + String(t2-t1));
          };
        };
        thisPannello.show();
        doc.clear();
        completeJsonString = "";
      
        request->send(200, "text/plain", "Display updated");
      };
  });

  server.begin();
}

void loop() {}