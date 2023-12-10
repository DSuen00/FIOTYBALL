#include <WiFi.h>
#include <HTTPClient.h>

# define trigPinOpp 19
# define echoPinOpp 21
# define trigPinPla 33
# define echoPinPla 15

# define newgameLED 22 // LED that lights up when a new game has started (start tracking for a goal); off when a goal has been made

WiFiClient client;

// const char* ssid = "Berkeley-IoT";
// const char* password = "4O#u45S2";
const char* ssid = "sleepy";
const char* password = "pleasew0rk";

String serverName = "http://192.168.229.42";  //server address

const int serverPort = 8000; //Your Domain name with URL path or IP address with path

//define sound speed in 10m/uS
const int SOUND_SPEED = 34; // used later on for distance calculation

// duration = amount of time taken for echo pin to receive signal from trigger bounce, distance = distance from ultrasonic sensor
long durationOpp; int distanceOpp; 
long durationPla; int distancePla;

int score = 0;
int count = 0; int countOpp = 0; int countPla = 0; // count used to ensure consistency in data output to filter out noise for accurate detection of goals
volatile bool new_game = false;


// calibrating the distances of each ultrasonic sensor
const int distance_reset_min_opp = 6000;
const int distance_reset_max_opp = 13000;

const int distance_reset_min_pla = 6000;
const int distance_reset_max_pla = 10000;

unsigned long previousTime = 0;
unsigned long lastTime = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  pinMode(trigPinPla, OUTPUT); pinMode(echoPinPla, INPUT);
  pinMode(trigPinOpp, OUTPUT); pinMode(echoPinOpp, INPUT);
  pinMode(newgameLED, OUTPUT); 
}

void loop() {
  // to obtain the distance within the goal
  unsigned long currentTime = micros();
  if (currentTime - previousTime < 120) {
    digitalWrite(trigPinPla, HIGH); digitalWrite(trigPinOpp, HIGH);
//    Serial.print(1);
  } else if (currentTime - previousTime < 160) {
    digitalWrite(trigPinPla, LOW); digitalWrite(trigPinOpp, LOW);
    scoretracker();
  } else {
    previousTime = currentTime;}
}

void setup_wifi(){
  WiFi.begin(ssid, password); //start wifi connection
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");}
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());}

void scoretracker() {
  // to detect the duration of the trigger bounceback and then calculate the distance of any object in the goal
  durationPla = pulseIn(echoPinPla, HIGH);
  distancePla = durationPla * SOUND_SPEED/2;
  durationOpp = pulseIn(echoPinOpp, HIGH);
  distanceOpp = durationOpp * SOUND_SPEED/2;

  // after a goal has been made; ensures that goals are empty before starting to try and detect a new goal
  if (!new_game) {
    if (distancePla > distance_reset_min_pla && distancePla < distance_reset_max_pla
        && distanceOpp > distance_reset_min_opp && distanceOpp < distance_reset_max_opp) { // checks that distances output by sensors are within the normal range of having nothing in the goals
      count += 1;
      if (count == 500) { count = 0; new_game = true; digitalWrite(newgameLED, HIGH);} // if distance is consistently within the range, start a new game and beginning tracking for a goal
    } else {
      count = 0; // restarting counting due to noise
      }
  }

  // after starting a new game, begin tracking for a goal to be scored
  if (new_game) {
    if (distancePla <= distance_reset_min_pla || distancePla >= distance_reset_max_pla) { // if player's goal detects something in the goal
      countPla += 1;
      if (countPla == 50) 
      {countPla = 0; new_game = false; digitalWrite(newgameLED, LOW); // if distance is consistently within range, confirm that player has scored a goal
      bool sent = false;
      while (!sent){
      sent = sentGoal(true);}} // sending to server that a goal has been made
    } else {countPla = 0;} // restarts counter due to noise
    if (distanceOpp <= distance_reset_min_opp || distanceOpp >= distance_reset_max_opp) { // if opponent's goal detects something in the goal
      countOpp += 1;
      if (countOpp == 50) 
      {countOpp = 0; new_game = false; digitalWrite(newgameLED, LOW); // if distance is consistently within range, confirm that opponent has scored a goal
      bool sent = false;
      while (!sent){
      sent = sentGoal(false);}} // sending to server that a goal has been made
    } else {countOpp = 0;} // restarts counter due to noise
  }

  // Prints the distance in the Serial Monitor
  Serial.print("Distance Pla: "); Serial.print(distancePla);
  Serial.print(" Distance Opp: "); Serial.print(distanceOpp);
  Serial.print(" newgame: "); Serial.println(new_game);
}

// to tell server to change score of player/opponent when a goal has been made
bool sentGoal(bool player){
    bool ret = false;
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String path = "";

      // Change the server path according to who score the goal
      if (player == true){
        path = "/pla";
      } else {path = "/opp";}
      String serverpath = serverName + ":" + String(serverPort) + path;
      Serial.println(serverpath);
      // Your Domain name with URL path or IP address with path
      http.begin(serverpath.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();
      Serial.println(httpResponseCode);
      String payload = "";

      // Check if the server successfully received the request
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
