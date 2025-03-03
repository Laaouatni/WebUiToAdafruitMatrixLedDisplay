#include <WiFi.h>
#include <Adafruit_NeoMatrix.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

const int PANNELLO_WIDTH = 32;
const int PANNELLO_HEIGHT = 8;
Adafruit_NeoMatrix thisPannello = Adafruit_NeoMatrix(PANNELLO_WIDTH,PANNELLO_HEIGHT,15,NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
String completeHttpBodyString = "";

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
      const String thisPartStringData = String((char*)data).substring(0, len);
      completeHttpBodyString += thisPartStringData;
      if (index + len != total) return;
      if(completeHttpBodyString[completeHttpBodyString.length-1] != ',') {
        completeHttpBodyString += ',';
      };
      int thisX = 0;
      int thisY = 0;
      String thisRGBAarray[4] = [ "", "", "", "" ];
      int thisRGBAindex = 0;

      for (int charIndex = 0; charIndex <= completeHttpBodyString.length; charIndex++) {
        const char thisChar = completeHttpBodyString[charIndex];
        const bool canGoToNextSplittedString = thisChar == ',';

        if(!canGoToNextSplittedString) {
          thisRGBAarray[thisRGBAindex] += String(thisChar);
          continue;
        }

        const bool isLastColorFullyWritten = thisRGBAindex == (thisRGBAarray.length - 1)

        if(!isLastColorFullyWritten) {
          thisRGBAindex++;
          continue;
        }

        for (let thisRGBAstringToResetIndex = 0; thisRGBAstringToResetIndex <= (thisRGBAarray.length-1); thisRGBAstringToResetIndex++) {
          thisRGBAarray[thisRGBAstringToResetIndex] = "";
        }
        
        thisX++;
        thisRGBAindex = 0;
        if(thisX % PANNELLO_WIDTH == 0) {
          thisY++;
          thisX = 0;
        }
      }

        // thisPannello.drawPixel(forX, forY, thisPannello.Color(R,G,B));

        thisPannello.show();
      completeJsonString = "";
      request->send(200, "text/plain", "Display updated");
  });

  server.begin();
}

void loop() {}