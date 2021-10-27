
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>


#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 8  //direccion esclavo

void requestEvent();
void receiveEvent(int howMany);

String vector;
int n_mod=I2C_SLAVE_ADDR,n=0;
uint16_t lectura_1=0,lectura_2=0,ref=0;
char formato[12]={"%d;%d;%d;n"};
char data_i2c_out[10],data_i2c_in[30];
int pin_led1=25,pin_led2=33;
int mod,duty,senal,dcycle=512;
char *indice;

//---------propiedasdes PWM ---------
const int pinPWM1 = 27;  // Pin GPIO PWM_1
const int pinPWM2 = 12;  // Pin GPIO PWM_2
const int freq = 25000; //freq. PWM a 25KHz
const int PWM_Channel_1 = 0; //canal PWM_1
const int PWM_Channel_2 = 1; //canal PWM_2
const int resolution = 10; //(10 bits)0-1023 duty cycle values

void setup(){
  
  Serial.begin(115200);
  pinMode(pin_led1,OUTPUT);
  pinMode(pin_led2,OUTPUT);
  ledcSetup(PWM_Channel_1, freq, resolution);
  ledcSetup(PWM_Channel_2, freq, resolution);
  ledcAttachPin(pinPWM1, PWM_Channel_1);
  ledcAttachPin(pinPWM2, PWM_Channel_2);
  
    bool res = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
    if (!res) {
        Serial.println("I2C slave init failed");
        while(1) delay(100);
    }
    WireSlave.onReceive(receiveEvent);
    WireSlave.onRequest(requestEvent);
    Serial.printf("Slave joined I2C bus with addr #%d\n", I2C_SLAVE_ADDR);
}

void loop(){
    
    lectura_1=analogRead(34);
    lectura_2=analogRead(35);
    WireSlave.update();
    // let I2C and other ESP32 peripherals interrupts work
    delay(1);

    if(senal==1){
      if(duty>50)
        digitalWrite(pin_led1,HIGH);
      else
        digitalWrite(pin_led1,LOW);
      dcycle=map(duty,0,100,0,1023);
      ledcWrite(PWM_Channel_1,dcycle);
    }
    
    if(senal==2){
      if(duty>50)
        digitalWrite(pin_led2,HIGH);
      else
        digitalWrite(pin_led2,LOW);
      dcycle=map(duty,0,100,0,1023);
      ledcWrite(PWM_Channel_2,dcycle);
    }
    Serial.print(mod);Serial.print(";");
    Serial.print(duty);Serial.print(";");
    Serial.println(senal);
}

//-----------------------------------------------------------
void requestEvent(){
    
    sprintf(data_i2c_out,formato,n_mod,lectura_1,lectura_2);
    WireSlave.print(data_i2c_out);  
}
//-----------------------------------------------------------
void receiveEvent(int howMany){
  
  while (1 < WireSlave.available()){ // loop through all but the last byte
        data_i2c_in[n] = WireSlave.read();  // receive byte as a character
        n++;
  }
n=0;
indice=strtok(data_i2c_in,";");mod=atoi(indice);
indice=strtok(NULL,";");duty=atoi(indice);
indice=strtok(NULL,";");senal=atoi(indice);

  for(int j=0;j<30;j++)
    data_i2c_in[j]=NULL;  
}
