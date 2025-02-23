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
  Serial.begin(4800);
  // Serial2.begin(4800, SERIAL_8N1, 16, 17);
  WiFi.begin("nomeWifi", "passwordWifi");

  while (WiFi.status() != WL_CONNECTED) {};
  Serial.println("WiFi connected! IP Address: " + WiFi.localIP().toString());

  ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client,
                AwsEventType type, void *arg, uint8_t *data, size_t len) {
    const bool isConnecting = type == WS_EVT_CONNECT;
    const bool isDisconnecting = type == WS_EVT_DISCONNECT;
    const bool hasData = type == WS_EVT_DATA;

    if (isConnecting)
      Serial.println("WebSocket Client Connected! IP Address: " +
                     client->remoteIP().toString());
    if (isDisconnecting)
      Serial.println("WebSocket Client Disconnected! IP Address: " +
                     client->remoteIP().toString());

    if (!hasData) return;

    const String thisStringData = String((char *)data).substring(0, len);


    // ws.textAll(thisStringData);
    // Serial2.println(thisStringData);
    // Serial.println("⬅️ ho ricevuto dal CLIENT un messaggio e ➡️ inoltrato ai altri: " + String(thisStringData));
  });

  server.addHandler(&ws);
  server.begin();
}

void loop() {
  ws.cleanupClients();
  // listenToNewSerialData(&myCallback);
};

// void myCallback(String arduinoValue) {
//   Serial.println("⬅️ ho ricevuto da ARDUINO un messaggio e ➡️ inoltrato ai altri: " + arduinoValue);
//   ws.textAll(arduinoValue);
// }

// String listenToNewSerialData(void (*myCallback)(String)) {
//   static String receivedString;

//   while (Serial2.available() > 0) {
//     const char thisChar = Serial2.read();
//     const bool isLastChar = thisChar == '\n';

//     if (!isLastChar) {
//       receivedString += thisChar;
//       continue;
//     };

//     myCallback(receivedString);
//     receivedString = "";
//   };

//   return "";
// };