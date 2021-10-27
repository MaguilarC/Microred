// the number of the LED pin
const int ledPin = 27;  // Pin GPIO

// setting PWM properties
const int freq = 25000; //25KHz
const int ledChannel = 0; //canal PWM
const int resolution = 10; //0-1023 values duty cycle

void setup() {
   // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  pinMode(26,OUTPUT);
  pinMode(25,OUTPUT);
  pinMode(33,OUTPUT);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  delay(15);

}

void loop() {
  /*
  digitalWrite(26,HIGH);
  delay(2000);
  digitalWrite(25,HIGH);
  digitalWrite(26,LOW);
  delay(2000);
  digitalWrite(33,HIGH);
  digitalWrite(25,LOW);
  delay(2000);
  digitalWrite(33,LOW);
  delay(2000);
  digitalWrite(25,HIGH);
  digitalWrite(26,HIGH);
  digitalWrite(33,HIGH);
  delay(2000);
  digitalWrite(25,LOW);
  digitalWrite(26,LOW);
  digitalWrite(33,LOW);
  delay(2000);*/
  
  
  ledcWrite(ledChannel,512);
  delay(5000);
  ledcWrite(ledChannel,255);
  delay(5000);
  ledcWrite(ledChannel,102);
  delay(5000);

  
}
