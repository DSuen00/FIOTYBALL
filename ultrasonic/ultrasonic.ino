# define trigPin 5
# define echoPin 21

//define sound speed in 10m/uS
const int SOUND_SPEED = 34;

long duration;
int distance;

int score = 0;
int count = 0;
volatile bool new_game = false;

int distance_reset_min = 5000;
int distance_reset_max = 10000;


unsigned long previousTime = 0;

void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  unsigned long currentTime = micros();
  if (currentTime - previousTime < 120) {
    digitalWrite(trigPin, HIGH);
//    Serial.print(1);
  } else if (currentTime - previousTime < 160) {
    digitalWrite(trigPin, LOW);
    scoretracker();
  } else {
    previousTime = currentTime;
  }

  // // Clears the trigPin
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(4);
  // // Sets the trigPin on HIGH state for 10 micro seconds
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(12);
  // digitalWrite(trigPin, LOW);
  
  // // Reads the echoPin, returns the sound wave travel time in microseconds
  // scoretracker();
}

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
      if (count == 100) { count = 0; score += 1; new_game = false; }
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