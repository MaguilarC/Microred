
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>


#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 3  //direccion esclavo

void requestEvent();
void receiveEvent(int howMany);

String vector;
int n_mod=I2C_SLAVE_ADDR,n=0;
uint16_t lectura_1=0,lectura_2=0,retorno=0;
char formato[12]={"%d;%d;%d;n"};
char data_i2c_out[10],data_i2c_in[10];


void setup(){
  
    Serial.begin(115200);
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
    //Serial.print(lectura_1);Serial.print(";");Serial.println(lectura_2);
}

//-----------------------------------------------------------
void requestEvent(){
    
    //vector=String(n_mod)+";"+String(lectura_1)+";"+String(lectura_2)+";";
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
retorno=atoi(data_i2c_in);
Serial.println(retorno);
  for(int j=0;j<10;j++)
    data_i2c_in[j]=NULL;
      
}
