#include <SoftwareSerial.h>
SoftwareSerial Serial2(2, 3);  // RX, TX

void setup() {
  Serial2.begin(9600);
  Serial.begin(9600);
}

void loop() {
  listenToNewSerialData(&myCallback);
}

void myCallback(String esp32value) {
  Serial.println("⬅️ ho ricevuto un messaggio: " + esp32value);
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