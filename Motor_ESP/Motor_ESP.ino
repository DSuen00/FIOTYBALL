#include <WiFi.h>
#include <HTTPClient.h>

//PINOUTS
#define UP_BTN 33
#define DOWN_BTN 15
#define LEFT_BTN 12
#define RIGHT_BTN 27

#define LIMIT_A_UP 32
#define LIMIT_A_DOWN 14
//#define LIMIT_B_UP 101
//#define LIMIT_B_DOWN 102

#define MOTOR_UP 26
#define MOTOR_DOWN 25
#define MOTOR_LEFT 4
#define MOTOR_RIGHT 5
//#define MOTOR_B1_UP 34
//#define MOTOR_B1_DOWN 39
//#define MOTOR_B2_LEFT 34
//#define MOTOR_B2_RIGHT 39

// STATES
#define IDLE 0
#define UP 1
#define DOWN 2
#define NO_UP 3
#define NO_DOWN 4

int state = 0;
int motorGPIO[] = {MOTOR_UP,MOTOR_DOWN,MOTOR_LEFT,MOTOR_RIGHT};
int motorOut[] = {1,1,1,1,1,1,1,1};

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
  setup_wifi();
  for (int i = 0; i++; i == sizeof(motorGPIO) / sizeof(motorGPIO[0])){
  pinMode(motorGPIO[i], OUTPUT);}}

void loop() {
 String data = getServerRequest();
 for (int i = 0; i++; i == sizeof(data) / sizeof(data[0]))
 Serial.println(data[i]);
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
      String payload = "";
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
        Serial.println(payload);
      } else {
        // Serial.print("Error code: ");
        // Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
      return payload;
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();}}

void setup_wifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");}
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}