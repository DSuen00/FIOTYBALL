#include <WiFi.h>
#include <HTTPClient.h>

# define trigPinOpp 26
# define echoPinOpp 25
# define trigPinPla 12
# define echoPinOpp 27

WiFiClient client;

const char* ssid = "Berkeley-IoT";
const char* password = "yNxq)I&2";

//Your Domain name with URL path or IP address with path
String serverName = "http://10.43.67.246";
const int serverPort = 8000;

//define sound speed in 10m/uS
const int SOUND_SPEED = 34;

long durationOpp; int distanceOpp;
long durationPla;int distancePla;

int score = 0;
int countOpp = 0; int countPla = 0;
volatile bool new_game = false;

const int distance_reset_min_opp = 5000;
const int distance_reset_max_opp = 11000;

const int distance_reset_min = 5000;
const int distance_reset_max = 11000;

unsigned long previousTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  pinMode(trigPinPla, OUTPUT); pinMode(echoPinPla, INPUT);
  pinMode(trigPinOpp, OUTPUT); pinMode(echoPinOpp, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = micros();
  if (currentTime - previousTime < 120) {
    digitalWrite(trigPin, HIGH);
//    Serial.print(1);
  } else if (currentTime - previousTime < 160) {
    digitalWrite(trigPin, LOW);
    scoretracker();
  } else {
    previousTime = currentTime;}
}

void setup_wifi(){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");}
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());}

void scoretracker() {
  durationPla = pulseIn(echoPinPla, HIGH);
  distancePla = duration * SOUND_SPEED/2;
  durationOpp = pulseIn(echoPinOpp, HIGH);
  distanceOpp = duration * SOUND_SPEED/2;
  
  if (!new_game) {
    if (distancePla > distance_reset_min_pla && distancePla < distance_reset_max_pla
        ) {
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
