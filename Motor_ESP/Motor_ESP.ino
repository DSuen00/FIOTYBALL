#include <WiFi.h>
#include <HTTPClient.h>
#define m11 13
#define m12 12
#define m21 11
#define m22 10
#define m31 9
#define m32 6
#define m41 5
#define m42 4

int motorGPIO[] = {m11,m12,m21,m22,m31,m32,m41,m42}
int motorOut[] = {1,1,1,1,1,1,1,1}

const char* ssid = "Berkeley-IoT";
// // const char* password = "0%$MB,(y";
const char* password = "yNxq)I&2";  //short cam

//Your Domain name with URL path or IP address with path
String serverName = "http://10.40.67.88:8000";
String serverExt = "/motor";
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10;

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  for (int i = 0; i++; i == motorGPIO.length()){
  pinMode(motorGPIO[i], OUTPUT);}}

void loop() {
 String data = String getServerRequest()
 for (int i = 0; i++; i == data.length())
 Serial.println()
}

String getServerRequest(){
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String serverPath = serverName + serverExt;

      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());

      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      } else {
        // Serial.print("Error code: ");
        // Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
      return payload
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}