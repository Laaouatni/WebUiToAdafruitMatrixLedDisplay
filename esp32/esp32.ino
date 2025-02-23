#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoMatrix.h>

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

Adafruit_NeoMatrix thisPannello = Adafruit_NeoMatrix(32,8,6,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
JsonDcoument doc;

void setup() {
  Serial.begin(9600);
  thisPannello.begin();
  thisPannello.fill(0);
  thisPannello.setBrightness(10);

  WiFi.begin("nomeWifi", "passwordWifi");
  while (WiFi.status() != WL_CONNECTED) {};
  Serial.println("WiFi connected! IP Address: " + WiFi.localIP().toString());

  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
    const bool isConnecting = type == WS_EVT_CONNECT;
    const bool isDisconnecting = type == WS_EVT_DISCONNECT;
    const bool hasData = type == WS_EVT_DATA;

    if (isConnecting)    Serial.println("WebSocket Client Connected! IP Address: "    + client->remoteIP().toString());
    if (isDisconnecting) Serial.println("WebSocket Client Disconnected! IP Address: " + client->remoteIP().toString());

    if (!hasData) return;

    const String thisStringData = String((char *)data).substring(0, len);
    DeserializationError error = deserializeJson(doc, thisStringData);

    if(error) {
      Serial.println("deserializeJson() failed: " + String(error.c_str()));
      return;
    };

    Serial.println("xLine: " + doc["xLine"].as<String>());

    for(int thisY = 0; thisY < 8; thisY++) {
      int R,G,B;
      if(doc["colorArray"][thisY].size() < 3) {
        R = 255;
        G = 255;
        B = 255;
      } else {
        R = doc["colorArray"][thisY][0].as<int>();
        G = doc["colorArray"][thisY][1].as<int>();
        B = doc["colorArray"][thisY][2].as<int>();
      };
      Serial.println(String(R) + "\t" + String(G) + "\t" + String(B));

      thisPannello.drawPixel(
        doc["xLine"].as<int>(), 
        thisY, 
        thisPannello.Color(R,G,B)
      );

      thisPannello.show();
    };

    doc.clear();
  });

  server.addHandler(&ws);
  server.begin();
}

void loop() {
  ws.cleanupClients();
};