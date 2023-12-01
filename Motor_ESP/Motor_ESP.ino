#include <WiFi.h>
#include <HTTPClient.h>

//PINOUTS

#define LIMIT_UP 27
#define LIMIT_DOWN 12

#define MOTOR_UP 25
#define MOTOR_DOWN 26
#define MOTOR_LEFT 4
#define MOTOR_RIGHT 5

# define trigPin 19
# define echoPin 21

// STATES
#define IDLE 0
#define UP 1
#define DOWN 2
#define NO_UP 3
#define NO_DOWN 4

int lin_state = 0;
int rot_state = 0;
int motorGPIO[4] = {MOTOR_UP,MOTOR_DOWN,MOTOR_LEFT,MOTOR_RIGHT};
bool goal_flag = false;
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

//define sound speed in 10m/uS
const int SOUND_SPEED = 34;

long duration;
int distance;

int score = 0;
int count = 0;
volatile bool new_game = false;

const int distance_reset_min = 5000;
const int distance_reset_max = 11000;


unsigned long previousTime = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  for (int i = 0; i++; i == sizeof(motorGPIO) / sizeof(motorGPIO[0])){
  pinMode(motorGPIO[i], OUTPUT);}
  pinMode(MOTOR_UP, OUTPUT); pinMode(MOTOR_DOWN, OUTPUT);
  pinMode(MOTOR_LEFT, OUTPUT); pinMode(MOTOR_RIGHT, OUTPUT);
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT);}

void loop() {
 unsigned long currentTime = micros();
  if (currentTime - previousTime < 120) {
    digitalWrite(trigPin, HIGH);
//    Serial.print(1);
  } else if (currentTime - previousTime < 160) {
    digitalWrite(trigPin, LOW);
    scoretracker();
  } else {
    previousTime = currentTime;}
//  data = getServerRequest();
// //  Serial.print(data[0]);Serial.print(data[1]);
// //  Serial.println(" ");
//  lin_state = linear_motor(lin_state);
//  rot_state = rot_motor(rot_state);
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

int linear_motor(int state){
   switch (state) {

    case IDLE:

      // Serial.println("IDLE");
      
      if (data[1] == '0') {
        digitalWrite(MOTOR_UP, HIGH);
        state = UP;}
      
      if (data[1] == '2') {
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

      if (data[1] == '2') {
        digitalWrite(MOTOR_DOWN, HIGH);
        state = DOWN;}
      break;
    
    case NO_DOWN:

      Serial.println("NO_DOWN");
      
      if (digitalRead(LIMIT_DOWN) == LOW) {
        state = IDLE;}

      if (data[1] == '0') {
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

void scoretracker() {
  duration = pulseIn(echoPin, HIGH);
  distance = duration * SOUND_SPEED/2;
  
  if (!new_game) {
    if (distance > distance_reset_min && distance < distance_reset_max) {
      count += 1;
      if (count == 1000) { count = 0; new_game = true; }
    } else {
      count = 0;
      }
  }

  if (new_game) {
    if (distance <= distance_reset_min || distance >= distance_reset_max) {
      count += 1;
      if (count == 100) 
      {count = 0; score += 1; new_game = false;
      bool sent = false;
      while (!sent){
      sent = sentGoal(true);} }
    } else {
      count = 0;
      }
  }

  // Prints the distance in the Serial Monitor
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.print("   Score : ");
  Serial.println(score);
}

bool sentGoal(bool player){
    bool ret = false;
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String path = "";
      if (player == true){
        path = "/pla";
      } else {path = "/opp";}
      String serverpath = serverName + ":" + String(serverPort) + path;
      Serial.println(serverpath);
      // Your Domain name with URL path or IP address with path
      http.begin(serverpath.c_str());

      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

      // Send HTTP GET request
      int httpResponseCode = http.GET();
      Serial.println(httpResponseCode);
      String payload = "";
      if (httpResponseCode == 200) {
        ret = true;
      } else {
        ret = false;}
      // Free resources
      http.end();
    } else {
      // Serial.println("WiFi Disconnected")
      ret = false;
    }
    lastTime = millis();
    return ret;}