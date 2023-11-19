#include <WiFi.h>
#include <HTTPClient.h>

//PINOUTS

#define LIMIT_UP 32
#define LIMIT_DOWN 14

#define MOTOR_UP 26
#define MOTOR_DOWN 25
#define MOTOR_LEFT 4
#define MOTOR_RIGHT 5


// STATES
#define IDLE 0
#define UP 1
#define DOWN 2
#define NO_UP 3
#define NO_DOWN 4

int lin_state = 0;
int rot_state = 0;
int motorGPIO[] = {MOTOR_UP,MOTOR_DOWN,MOTOR_LEFT,MOTOR_RIGHT};
int motorOut[] = {1,1,1,1,1,1,1,1};
String data = "1111";

const char* ssid = "Berkeley-IoT";
// // const char* password = "0%$MB,(y";
const char* password = "yNxq)I&2";  //short cam

//Your Domain name with URL path or IP address with path
String serverName = "http://10.40.67.88:8000";
String serverExt = "/motor";
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  for (int i = 0; i++; i == sizeof(motorGPIO) / sizeof(motorGPIO[0])){
  pinMode(motorGPIO[i], OUTPUT);}}

void loop() {
 data = getServerRequest();
 Serial.print(data[0]);Serial.print(data[1]);
 Serial.println(" ");
 lin_state = linear_motor(lin_state);
 rot_state = rot_motor(rot_state);
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
        // Serial.println(payload);
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
  Serial.println(WiFi.localIP());}

int linear_motor(int state){
   switch (state) {

    case IDLE:

      Serial.println("IDLE");
      
      if (data[1] == '2') {
        digitalWrite(MOTOR_UP, HIGH);
        state = UP;}
      
      if (data[1] == '0') {
        digitalWrite(MOTOR_DOWN, HIGH);
        state = DOWN;
      }
      
      if (digitalRead(LIMIT_UP) == HIGH) {
        state = NO_UP;}

      if (digitalRead(LIMIT_DOWN) == HIGH) {
        state = NO_DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[1] == '1') {
        digitalWrite(MOTOR_UP, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_UP) == HIGH) {
        digitalWrite(MOTOR_UP, LOW);
        state = NO_UP;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[1] == '1') {
        digitalWrite(MOTOR_DOWN, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_DOWN) == HIGH) {
        digitalWrite(MOTOR_DOWN, LOW);
        state = NO_DOWN;}
      break;

    case NO_UP:

      Serial.println("NO_UP");
      
      if (digitalRead(LIMIT_UP) == LOW) {
        state = IDLE;}

      if (data[1] == '0') {
        digitalWrite(MOTOR_DOWN, HIGH);
        state = DOWN;}
      break;
    
    case NO_DOWN:

      Serial.println("NO_DOWN");
      
      if (digitalRead(LIMIT_DOWN) == LOW) {
        state = IDLE;}

      if (data[1] == '2') {
        digitalWrite(MOTOR_UP, HIGH);
        state = UP;}
      break;}
      return state;}

int rot_motor(int state){
   int motor = 0;
   switch (state) {

    case IDLE:

      Serial.println("IDLE");
      
      if (data[motor] == '2') {
        digitalWrite(MOTOR_LEFT, HIGH);
        state = UP;}
      
      if (data[motor] == '0') {
        digitalWrite(MOTOR_RIGHT, HIGH);
        state = DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[motor] == '1') {
        digitalWrite(MOTOR_LEFT, LOW);
        state = IDLE;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[motor] == '1') {
        digitalWrite(MOTOR_RIGHT, LOW);
        state = IDLE;}
      break;}
      return state;}