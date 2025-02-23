#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial Serial2(2, 3);  // RX, TX

void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);
}

void loop() {
  listenToNewSerialData(&myCallback);
}

void myCallback(String esp32value) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, esp32value);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  for(int y = 0; y < doc.size(); y++) {
    for(int x = 0; x < doc[y].size(); x++) {
      Serial.println(doc[y][x].as<String>());
    }
  }

  Serial.println("⬅️ ho ricevuto un messaggio: " + esp32value);

  doc.clear();
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