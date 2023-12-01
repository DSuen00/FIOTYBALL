#include <WiFi.h>
#include <HTTPClient.h>

//PINOUTS

#define LIMIT_UP_G 19
#define LIMIT_DOWN_G 21

#define MOTOR_UP_G 25
#define MOTOR_DOWN_G 26
#define MOTOR_LEFT_G 4
#define MOTOR_RIGHT_G 5

#define LIMIT_UP_P 32
#define LIMIT_DOWN_P 14

#define MOTOR_UP_P 12
#define MOTOR_DOWN_P 27
#define MOTOR_LEFT_P 33
#define MOTOR_RIGHT_P 15


// STATES
#define IDLE 0
#define UP 1
#define DOWN 2
#define NO_UP 3
#define NO_DOWN 4

int lin_state_G = 0;
int rot_state_G = 0;
int lin_state_P = 0;
int rot_state_P = 0;
String data = "1111";
WiFiClient client;

const char* ssid = "Berkeley-IoT";
const char* password = "yNxq)I&2";
// const char* password = "yNxq)I&2";  //short cam

//Your Domain name with URL path or IP address with path
String serverName = "http://10.43.67.246";
const int serverPort = 8000;
String serverPath = "/motor";
unsigned long lastTime = 0;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 10;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(MOTOR_UP_G, OUTPUT); pinMode(MOTOR_DOWN_G, OUTPUT);
  pinMode(MOTOR_LEFT_G, OUTPUT); pinMode(MOTOR_RIGHT_G, OUTPUT);
  pinMode(LIMIT_UP_G, INPUT_PULLUP); pinMode(LIMIT_DOWN_G, INPUT_PULLUP);
  pinMode(MOTOR_UP_P, OUTPUT); pinMode(MOTOR_DOWN_P, OUTPUT);
  pinMode(MOTOR_LEFT_P, OUTPUT); pinMode(MOTOR_RIGHT_P, OUTPUT);
  pinMode(LIMIT_UP_P, INPUT_PULLUP); pinMode(LIMIT_DOWN_P, INPUT_PULLUP);}

void loop() {
 data = getServerRequest();
 Serial.print(data[0]);Serial.print(data[1]);
 Serial.println(" ");
 lin_state_G = linear_motor_G(lin_state_G);
 rot_state_G = rot_motor_G(rot_state_G);
 lin_state_P = linear_motor_P(lin_state_P);
 rot_state_P = rot_motor_P(rot_state_P);
 }

String getServerRequest(){
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String serverpath = serverName + ":" + String(serverPort) + serverPath;
      // Serial.println(serverpath);
      // Your Domain name with URL path or IP address with path
      http.begin(serverpath.c_str());

      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Send HTTP GET request
      int httpResponseCode = http.GET();
      String payload = "";
      if (httpResponseCode > 0) {
        // Serial.print("HTTP Response code: ");
        // Serial.println(httpResponseCode);
        payload = http.getString();
        // Serial.println(payload);
      } else {
        // Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
      return payload;
    } else {
      // Serial.println("WiFi Disconnected");
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

int linear_motor_G(int state){
   switch (state) {

    case IDLE:

      // Serial.println("IDLE");
      
      if (data[1] == '0') {
        digitalWrite(MOTOR_UP_G, HIGH);
        state = UP;}
      
      if (data[1] == '2') {
        digitalWrite(MOTOR_DOWN_G, HIGH);
        state = DOWN;
      }
      
      if (digitalRead(LIMIT_UP_G) == HIGH) {
        state = NO_UP;}

      if (digitalRead(LIMIT_DOWN_G) == HIGH) {
        state = NO_DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[1] == '1') {
        digitalWrite(MOTOR_UP_G, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_UP_G) == HIGH) {
        digitalWrite(MOTOR_UP_G, LOW);
        state = NO_UP;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[1] == '1') {
        digitalWrite(MOTOR_DOWN_G, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_DOWN_G) == HIGH) {
        digitalWrite(MOTOR_DOWN_G, LOW);
        state = NO_DOWN;}
      break;

    case NO_UP:

      Serial.println("NO_UP");
      
      if (digitalRead(LIMIT_UP_G) == LOW) {
        state = IDLE;}

      if (data[1] == '2') {
        digitalWrite(MOTOR_DOWN_G, HIGH);
        state = DOWN;}
      break;
    
    case NO_DOWN:

      Serial.println("NO_DOWN");
      
      if (digitalRead(LIMIT_DOWN_G) == LOW) {
        state = IDLE;}

      if (data[1] == '0') {
        digitalWrite(MOTOR_UP_G, HIGH);
        state = UP;}
      break;}
      return state;}

int linear_motor_P(int state){
   switch (state) {

    case IDLE:

      // Serial.println("IDLE");
      
      if (data[3] == '0') {
        digitalWrite(MOTOR_UP_P, HIGH);
        state = UP;}
      
      if (data[3] == '2') {
        digitalWrite(MOTOR_DOWN_P, HIGH);
        state = DOWN;
      }
      
      if (digitalRead(LIMIT_UP_P) == HIGH) {
        state = NO_UP;}

      if (digitalRead(LIMIT_DOWN_P) == HIGH) {
        state = NO_DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[3] == '1') {
        digitalWrite(MOTOR_UP_P, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_UP_P) == HIGH) {
        digitalWrite(MOTOR_UP_P, LOW);
        state = NO_UP;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[3] == '1') {
        digitalWrite(MOTOR_DOWN_P, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_DOWN_P) == HIGH) {
        digitalWrite(MOTOR_DOWN_P, LOW);
        state = NO_DOWN;}
      break;

    case NO_UP:

      Serial.println("NO_UP");
      
      if (digitalRead(LIMIT_UP_P) == LOW) {
        state = IDLE;}

      if (data[3] == '2') {
        digitalWrite(MOTOR_DOWN_P, HIGH);
        state = DOWN;}
      break;
    
    case NO_DOWN:

      Serial.println("NO_DOWN");
      
      if (digitalRead(LIMIT_DOWN_P) == LOW) {
        state = IDLE;}

      if (data[3] == '0') {
        digitalWrite(MOTOR_UP_P, HIGH);
        state = UP;}
      break;}
      return state;}

int rot_motor_G(int state){
   int motor = 0;
   switch (state) {

    case IDLE:

      Serial.println("IDLE");
      
      if (data[motor] == '2') {
        digitalWrite(MOTOR_LEFT_G, HIGH);
        state = UP;}
      
      if (data[motor] == '0') {
        digitalWrite(MOTOR_RIGHT_G, HIGH);
        state = DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[motor] == '1') {
        digitalWrite(MOTOR_LEFT_G, LOW);
        state = IDLE;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[motor] == '1') {
        digitalWrite(MOTOR_RIGHT_G, LOW);
        state = IDLE;}
      break;}
      return state;}

int rot_motor_P(int state){
   int motor = 2;
   switch (state) {

    case IDLE:

      Serial.println("IDLE");
      
      if (data[motor] == '2') {
        digitalWrite(MOTOR_LEFT_P, HIGH);
        state = UP;}
      
      if (data[motor] == '0') {
        digitalWrite(MOTOR_RIGHT_P, HIGH);
        state = DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[motor] == '1') {
        digitalWrite(MOTOR_LEFT_P, LOW);
        state = IDLE;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[motor] == '1') {
        digitalWrite(MOTOR_RIGHT_P, LOW);
        state = IDLE;}
      break;}
      return state;}