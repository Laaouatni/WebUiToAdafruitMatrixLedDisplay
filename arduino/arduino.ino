#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoMatrix.h>

Adafruit_NeoMatrix thisPannello = Adafruit_NeoMatrix(5,8,6,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
SoftwareSerial Serial2(2, 3);  // RX, TX
JsonDocument doc;

void setup() {
  thisPannello.begin();
  thisPannello.fill(0);
  Serial2.begin(4800);
  Serial.begin(4800);
  thisPannello.setBrightness(10);
}

void loop() {
  listenToNewSerialData(&myCallback);
}

void myCallback(String esp32value) {
  DeserializationError error = deserializeJson(doc, esp32value);

  if (error) {
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
  delay(20);

  // Serial.println("⬅️ ho ricevuto un messaggio: " + esp32value);
};

String listenToNewSerialData(void (*myCallback)(String)) {
  static String receivedString;

  while (Serial2.available() > 0) {
    const char thisChar = Serial2.read();
    const bool isLastChar = thisChar == '\n';

    if (!isLastChar) {
      receivedString += thisChar;
      continue;
    };

    myCallback(receivedString);
    receivedString = "";
  };

  return "";
};