#define m11 12
#define m12 27
#define m21 33
#define m22 15
#define m31 32
#define m32 14
#define m41 A0
#define m42 A1


int motorGPIO[] = {m11,m12,m21,m22,m31,m32,m41,m42};
int motorData[] = {2,0,0,0};
int motorOut[] = {0,0,0,0,0,0,0,0};
int inputMax = 3;
int outputMax = 8;

// setting PWM properties ----------------------------
const int freq = 5000;
const int ledChannel_1 = 1;
const int ledChannel_2 = 2;
const int resolution = 8;
const int MAX_PWM_VOLTAGE = 255;
const int NOM_PWM_VOLTAGE = MAX_PWM_VOLTAGE *0.6;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < sizeof(motorGPIO) / sizeof(motorGPIO[0]); i++){
    pinMode(motorGPIO[i], OUTPUT); ledcSetup(i, freq, resolution); ledcAttachPin(motorGPIO[i],i);}
    Serial.println("done");}

void loop() {
  dataToOut(); outToMotor();
}
  // put your main code here, to run repeatedly:


void dataToOut(){
  for (int i = 0; i < sizeof(motorData) / sizeof(motorData[0]); i++){
    if (motorData[i]>= inputMax){motorData[i] = inputMax-1;}
    else if (motorData[i] < 0){motorData[i] = 0;}
    
    if (motorData[i] == (inputMax-1)/2){
      motorOut[2*i] = 0; motorOut[2*i+1] = 0;}
      else if (motorData[i] < (inputMax-1)/2){
      motorOut[2*i] = 0; motorOut[2*i+1] +=1;}
      else if (motorData[i] > (inputMax-1)/2){
      motorOut[2*i+1] = 0; motorOut[2*i] +=1;}
    
    if (motorOut[2*i] > outputMax){
      motorOut[2*i] = outputMax;}
    if (motorOut[2*i+1] > outputMax){
      motorOut[2*i+1] = outputMax;}
    Serial.print(2*i); Serial.print(" ");Serial.println(motorOut[2*i]);
    Serial.print(2*i+1); Serial.print(" ");Serial.println(motorOut[2*i+1]);}}

void outToMotor(){
  Serial.println("TO Motor");
  for (int i = 0; i < sizeof(motorOut) / sizeof(motorOut[0]); i++){
    ledcWrite(i, convertMotor(motorOut[i], inputMax));
    Serial.print(i); Serial.print(" ");Serial.println(convertMotor(motorOut[i], inputMax));}}

int convertMotor(int data, int dataMax){
  int out = NOM_PWM_VOLTAGE/dataMax * data;
  return out;}