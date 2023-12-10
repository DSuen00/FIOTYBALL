#include <WiFi.h>
#include <HTTPClient.h>

//PINOUTS

// G: Goalkeeper shaft
#define LIMIT_UP_G 19
#define LIMIT_DOWN_G 20

#define MOTOR_UP_G 25
#define MOTOR_DOWN_G 26
#define MOTOR_LEFT_G 4
#define MOTOR_RIGHT_G 5

// P: Other shaft
#define LIMIT_UP_P 32
#define LIMIT_DOWN_P 14

#define MOTOR_UP_P 12
#define MOTOR_DOWN_P 27
#define MOTOR_LEFT_P 33
#define MOTOR_RIGHT_P 15

#define wifiLED 22

// STATES
#define IDLE 0 // when there is no input from the player
#define UP 1 // when player presses up
#define DOWN 2 // when player presses down
#define NO_UP 3 // when up limit switch is activated; disables up movement
#define NO_DOWN 4 // when down limit switch is activated; disables down movement

int lin_state_G = 0;
int rot_state_G = 0;
int lin_state_P = 0;
int rot_state_P = 0;
String data = "1111"; // string away telling motors what to do
WiFiClient client;

const char* ssid = "sleepy";
// const char* password = "yNxq)I&2";
// const char* password = ")7)lf!MQ";  //micro-USB
const char* password = "pleasew0rk";

//Your Domain name with URL path or IP address with path
String serverName = "http://192.168.229.42";
const int serverPort = 8000;
String serverPath = "/motor";
unsigned long lastTime = 0;
unsigned long timerDelay = 10;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  pinMode(MOTOR_UP_G, OUTPUT); pinMode(MOTOR_DOWN_G, OUTPUT);
  pinMode(MOTOR_LEFT_G, OUTPUT); pinMode(MOTOR_RIGHT_G, OUTPUT);
  pinMode(LIMIT_UP_G, INPUT_PULLDOWN); pinMode(LIMIT_DOWN_G, INPUT_PULLUP);
  pinMode(MOTOR_UP_P, OUTPUT); pinMode(MOTOR_DOWN_P, OUTPUT);
  pinMode(MOTOR_LEFT_P, OUTPUT); pinMode(MOTOR_RIGHT_P, OUTPUT);
  pinMode(LIMIT_UP_P, INPUT_PULLDOWN); pinMode(LIMIT_DOWN_P, INPUT_PULLDOWN);
  pinMode(wifiLED,OUTPUT);}

void loop() {
 data = getServerRequest(); //get keyboard input data from the server, saves in in data
 Serial.print(data[0]);Serial.print(data[1]);
 Serial.println(" ");
 lin_state_G = linear_motor_G(lin_state_G); // tells motors for linear movement what to do
 rot_state_G = rot_motor_G(rot_state_G); // tells motors for rotational movement what to do
 lin_state_P = linear_motor_P(lin_state_P); // tells motors for linear movement what to do
 rot_state_P = rot_motor_P(rot_state_P); // tells motors for rotational movement what to do
 Serial.println(digitalRead(LIMIT_DOWN_G));
 Serial.println(digitalRead(LIMIT_UP_G));
 delay(1);
 }

String getServerRequest(){
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;

      String serverpath = serverName + ":" + String(serverPort) + serverPath;
      // Serial.println(serverpath);
      http.begin(serverpath.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();
      String payload = "";

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString(); //save server response in a string
        digitalWrite(wifiLED, HIGH); //turn LED on to indicate successful connection
        Serial.println(payload);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        digitalWrite(wifiLED, LOW); //turn LED off to indicate failed connection
      }
      // Free resources
      http.end();
      return payload;
    } else {
      // Serial.println("WiFi Disconnected");
      WiFi.begin(ssid, password);
    }
    lastTime = millis();}

//this sets up the wifi object
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
      
      if (data[1] == '0') {  // when player inputs up
        digitalWrite(MOTOR_UP_G, HIGH);
        state = UP;}
      
      if (data[1] == '2') {  // when player inputs down
        digitalWrite(MOTOR_DOWN_G, HIGH);
        state = DOWN;
      }
      
      if (digitalRead(LIMIT_UP_G) == HIGH) { // when up limit switch is activated
        state = NO_UP;}

      if (digitalRead(LIMIT_DOWN_G) == HIGH) { // when down limit switch is activated
        state = NO_DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[1] == '1') { // when player lets go of up button
        digitalWrite(MOTOR_UP_G, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_UP_G) == HIGH) { // when up limit switch is hit; cuts off up motor
        digitalWrite(MOTOR_UP_G, LOW);
        state = NO_UP;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[1] == '1') { // when player lets go of up button
        digitalWrite(MOTOR_DOWN_G, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_DOWN_G) == HIGH) { // when up limit switch is hit; cuts off down motor
        digitalWrite(MOTOR_DOWN_G, LOW);
        state = NO_DOWN;}
      break;

    case NO_UP:

      Serial.println("NO_UP");
      
      if (digitalRead(LIMIT_UP_G) == LOW) { // when limit switch is deactivated
        state = IDLE;}

      if (data[1] == '2') {   // when player inputs down
        digitalWrite(MOTOR_DOWN_G, HIGH);
        state = DOWN;}
      break;
    
    case NO_DOWN:

      Serial.println("NO_DOWN");
      
      if (digitalRead(LIMIT_DOWN_G) == LOW) { // when limit switch is deactivated
        state = IDLE;}

      if (data[1] == '0') {  // when player inputs up
        digitalWrite(MOTOR_UP_G, HIGH);
        state = UP;}
      break;}
      return state;}

int linear_motor_P(int state){
   switch (state) {

    case IDLE:

      // Serial.println("IDLE");
      
      if (data[3] == '0') {  // when player inputs up
        digitalWrite(MOTOR_UP_P, HIGH);
        state = UP;}
      
      if (data[3] == '2') {  // when player inputs down
        digitalWrite(MOTOR_DOWN_P, HIGH);
        state = DOWN;
      }
      
      if (digitalRead(LIMIT_UP_P) == HIGH) { // when up limit switch is hit
        state = NO_UP;}

      if (digitalRead(LIMIT_DOWN_P) == HIGH) {  // when down limit switch is hit
        state = NO_DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[3] == '1') {  // when player lets go of up
        digitalWrite(MOTOR_UP_P, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_UP_P) == HIGH) {  // when limit switch of up is hit; cuts off up motor
        digitalWrite(MOTOR_UP_P, LOW);
        state = NO_UP;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[3] == '1') {  // when player lets go of down
        digitalWrite(MOTOR_DOWN_P, LOW);
        state = IDLE;}
      
      if (digitalRead(LIMIT_DOWN_P) == HIGH) {  // when down limit swich is hit
        digitalWrite(MOTOR_DOWN_P, LOW);
        state = NO_DOWN;}
      break;

    case NO_UP:

      Serial.println("NO_UP");
      
      if (digitalRead(LIMIT_UP_P) == LOW) { // when up limit switch is let go of
        state = IDLE;}

      if (data[3] == '2') { // when player inputs down
        digitalWrite(MOTOR_DOWN_P, HIGH);
        state = DOWN;}
      break;
    
    case NO_DOWN:

      Serial.println("NO_DOWN");
      
      if (digitalRead(LIMIT_DOWN_P) == LOW) {  // when down limit switch is let go of
        state = IDLE;}

      if (data[3] == '0') {  // when player inputs up
        digitalWrite(MOTOR_UP_P, HIGH);
        state = UP;}
      break;}
      return state;}

int rot_motor_G(int state){
   int motor = 0;
   switch (state) {

    case IDLE:

      // Serial.println("IDLE");
      
      if (data[motor] == '2') { // when player inputs left
        digitalWrite(MOTOR_LEFT_G, HIGH);
        state = UP;}
      
      if (data[motor] == '0') {  // when player inputs right
        digitalWrite(MOTOR_RIGHT_G, HIGH);
        state = DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[motor] == '1') {  // when player lets go of left
        digitalWrite(MOTOR_LEFT_G, LOW);
        state = IDLE;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[motor] == '1') {  // when player lets go of right
        digitalWrite(MOTOR_RIGHT_G, LOW);
        state = IDLE;}
      break;}
      return state;}

int rot_motor_P(int state){
   int motor = 2;
   switch (state) {

    case IDLE:

      // Serial.println("IDLE");
      
      if (data[motor] == '2') {  // when player inputs left
        digitalWrite(MOTOR_LEFT_P, HIGH);
        state = UP;}
      
      if (data[motor] == '0') {  // when player inputs right
        digitalWrite(MOTOR_RIGHT_P, HIGH);
        state = DOWN;}

      break;
    
    case UP:

      Serial.println("UP");
      
      if (data[motor] == '1') {  // when player lets go of left
        digitalWrite(MOTOR_LEFT_P, LOW);
        state = IDLE;}
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (data[motor] == '1') {  // when player lets go of right
        digitalWrite(MOTOR_RIGHT_P, LOW);
        state = IDLE;}
      break;}
      return state;}
