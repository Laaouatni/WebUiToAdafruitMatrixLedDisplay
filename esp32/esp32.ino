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
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  // server.on("/data", HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
  //   request->send(200, "text/plain", "");
  // });

  // server.on("/data", HTTP_POST, nullptr, nullptr, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
  //   Serial.println("ciao");
  //   // DeserializationError error = deserializeJson(doc, data);
  //   // JsonArray pixelArrayColors = doc["colorMatrixData"]["arrayColors"];
    
  //   // for(int forY = 0; forY < 8; forY++) {
  //   //   JsonArray arrayRigaPannello = pixelArrayColors[forY];
  //   //   for(int forX = 0; forX < 32; forX++) {
  //   //     JsonObject pixelColor = arrayRigaPannello[forX];
  //   //     int R = pixelColor["R"];
  //   //     int G = pixelColor["G"];
  //   //     int B = pixelColor["B"];
  //   //     thisPannello.drawPixel(forX, forY, thisPannello.Color(R,G,B));
  //   //   };
  //   // }
  //   // thisPannello.show();
  //   // doc.clear();
  //   request->send(200, "text/plain", "Display updated");
  // });

  server.onNotFound([](AsyncWebServerRequest* request) {
    if(request->method() == HTTP_OPTIONS) {
      request->send(200);
      return;
    } 

    if(request->method() == HTTP_POST) {
      request->send(200, "text/plain", "Display updated");
      return;
    }
  })

  server.begin();
}

void loop() {}



// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <WiFi.h>
// #include <ArduinoJson.h>
// #include <Adafruit_NeoMatrix.h>

// AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");

// Adafruit_NeoMatrix thisPannello = Adafruit_NeoMatrix(32,8,15,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
// JsonDocument doc;

// void setup() {
//   Serial.begin(9600);
//   thisPannello.begin();
//   thisPannello.fill(0);
//   thisPannello.setBrightness(10);

//   WiFi.begin("nomeWifi", "passwordWifi");
//   while (WiFi.status() != WL_CONNECTED) {};
//   Serial.println("WiFi connected! IP Address: " + WiFi.localIP().toString());

//   ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
//     const bool isConnecting = type == WS_EVT_CONNECT;
//     const bool isDisconnecting = type == WS_EVT_DISCONNECT;
//     const bool hasData = type == WS_EVT_DATA;

//     if (isConnecting)    Serial.println("WebSocket Client Connected! IP Address: "    + client->remoteIP().toString());
//     if (isDisconnecting) Serial.println("WebSocket Client Disconnected! IP Address: " + client->remoteIP().toString());

//     if (!hasData) return;

//     const String thisStringData = String((char *)data).substring(0, len);

//     DeserializationError error = deserializeJson(doc, thisStringData);

//     if(error) {
//       Serial.println("deserializeJson() failed: " + String(error.c_str()));
//       return;
//     };

//     for(int thisX = 0; thisX < 32; thisX++) {
//       int R,G,B;
//       if(doc["colorArray"][thisX].size() < 3) {
//         R = 255;
//         G = 255;
//         B = 255;
//       } else {
//         R = doc["colorArray"][thisX][0].as<int>();
//         G = doc["colorArray"][thisX][1].as<int>();
//         B = doc["colorArray"][thisX][2].as<int>();
//       };

//       thisPannello.drawPixel(
//         thisX, 
//         doc["yLine"].as<int>(), 
//         thisPannello.Color(R,G,B)
//       );
//     };
//     thisPannello.show();

//     doc.clear();
//   });

//   server.addHandler(&ws);
//   server.begin();
  
// }

// void loop() {
//   ws.cleanupClients();
// };