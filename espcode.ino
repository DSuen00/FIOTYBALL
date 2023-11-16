// DEFINE -----------------------------------------------
  // PINOUTS

#define UP_BTN 33
#define DOWN_BTN 15
#define LEFT_BTN 12
#define RIGHT_BTN 27

#define LIMIT_A_UP 32
#define LIMIT_A_DOWN 14
//#define LIMIT_B_UP 101
//#define LIMIT_B_DOWN 102

#define MOTOR_A1_UP 26
#define MOTOR_A1_DOWN 25
#define MOTOR_A2_LEFT 4
#define MOTOR_A2_RIGHT 5
//#define MOTOR_B1_UP 34
//#define MOTOR_B1_DOWN 39
//#define MOTOR_B2_LEFT 34
//#define MOTOR_B2_RIGHT 39

#define IDLE 0
#define UP 1
#define DOWN 2
#define NO_UP 3
#define NO_DOWN 4

int state = 0;

void IRAM_ATTR A2_LEFT_isr() {  // the function to be called when interrupt is triggered
  digitalWrite(MOTOR_A2_LEFT,digitalRead(LEFT_BTN)); 
}

void IRAM_ATTR A2_RIGHT_isr() {  // the function to be called when interrupt is triggered
  digitalWrite(MOTOR_A2_RIGHT,digitalRead(RIGHT_BTN)); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(UP_BTN, INPUT);
  pinMode(DOWN_BTN, INPUT);
  pinMode(LEFT_BTN, INPUT);
  pinMode(RIGHT_BTN, INPUT);
 

  pinMode(LIMIT_A_UP, INPUT);
  pinMode(LIMIT_A_DOWN, INPUT); 

  pinMode(MOTOR_A1_UP, OUTPUT);
  pinMode(MOTOR_A1_DOWN, OUTPUT);
  pinMode(MOTOR_A2_LEFT, OUTPUT);
  pinMode(MOTOR_A2_RIGHT, OUTPUT);
  digitalWrite(MOTOR_A1_UP, LOW);
  digitalWrite(MOTOR_A1_DOWN, LOW);
  digitalWrite(MOTOR_A2_LEFT, LOW);
  digitalWrite(MOTOR_A2_RIGHT, LOW);

  attachInterrupt(LEFT_BTN, A2_LEFT_isr, CHANGE);  // to control A2 LEFT motor
  attachInterrupt(RIGHT_BTN, A2_RIGHT_isr, CHANGE);  // to control A2 RIGHT motor
}

void testloop() {
//   // for checking BTN and LIMIT SWITCHES inputs
// Serial.print(digitalRead(UP_BTN));
// Serial.print(digitalRead(DOWN_BTN));
//   Serial.print(digitalRead(LIMIT_A_UP));
//   Serial.print(digitalRead(LIMIT_A_DOWN));  
// Serial.println();
    Serial.print(digitalRead(LEFT_BTN));
    Serial.print(digitalRead(RIGHT_BTN));
//  Serial.print(digitalRead(LIMIT_A_UP));
//  Serial.print(digitalRead(LIMIT_A_DOWN));  
    Serial.println();
  
//   if (digitalRead(LIMIT_A_UP) == LOW) {
//   //   // ledcWrite(ledChannel_1, NOM_PWM_VOLTAGE);
//   //   // ledcWrite(ledChannel_2, LOW);
//     digitalWrite(MOTOR_A1_UP, HIGH);
//     digitalWrite(MOTOR_A1_DOWN, LOW);
//   } else {
//      // ledcWrite(ledChannel_1, LOW);
//      // ledcWrite(ledChannel_2, NOM_PWM_VOLTAGE);
//     digitalWrite(MOTOR_A1_UP, LOW);
//     digitalWrite(MOTOR_A1_DOWN, HIGH);
//   }
 }

void loop() {

  switch (state) {

    case IDLE:

      Serial.println("IDLE");
      
      if (digitalRead(UP_BTN) == HIGH) {
        digitalWrite(MOTOR_A1_UP, HIGH);
        state = UP;
      }
      
      if (digitalRead(DOWN_BTN) == HIGH) {
        digitalWrite(MOTOR_A1_DOWN, HIGH);
        state = DOWN;
      }
      
      if (digitalRead(LIMIT_A_UP) == HIGH) {
        state = NO_UP;
      }

      if (digitalRead(LIMIT_A_DOWN) == HIGH) {
        state = NO_DOWN;
      }

      break;
    
    case UP:

      Serial.println("UP");
      
      if (digitalRead(UP_BTN) == LOW) {
        digitalWrite(MOTOR_A1_UP, LOW);
        state = IDLE;
      }
      
      if (digitalRead(LIMIT_A_UP) == HIGH) {
        digitalWrite(MOTOR_A1_UP, LOW);
        state = NO_UP;
      }
      break;

    case DOWN:

      Serial.println("DOWN");
      
      if (digitalRead(DOWN_BTN) == LOW) {
        digitalWrite(MOTOR_A1_DOWN, LOW);
        state = IDLE;
      }
      
      if (digitalRead(LIMIT_A_DOWN) == HIGH) {
        digitalWrite(MOTOR_A1_DOWN, LOW);
        state = NO_DOWN;
      }
      break;

    case NO_UP:

      Serial.println("NO_UP");
      
      if (digitalRead(LIMIT_A_UP) == LOW) {
        state = IDLE;
      }

      if (digitalRead(DOWN_BTN) == HIGH) {
        digitalWrite(MOTOR_A1_DOWN, HIGH);
        state = DOWN;
      }
      break;
    
    case NO_DOWN:

      Serial.println("NO_DOWN");
      
      if (digitalRead(LIMIT_A_DOWN) == LOW) {
        state = IDLE;
      }

      if (digitalRead(UP_BTN) == HIGH) {
        digitalWrite(MOTOR_A1_UP, HIGH);
        state = UP;
      }
      break;
    
  }
}
