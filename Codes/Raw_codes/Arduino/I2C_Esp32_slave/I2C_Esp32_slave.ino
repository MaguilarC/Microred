
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>


#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 8  //direccion esclavo

void requestEvent();
void receiveEvent(int howMany);


String lectura_1, lectura_2,c_retorno,vector;
int mod_remit, lect1_remit,lect2_remit;
String mod= String(3);
uint16_t voltaje1, voltaje2,retorno;
int n=0;
int rearmado;
char num[10];


void setup()
{
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

void loop()
{
    
    voltaje1=analogRead(34);
    voltaje2=analogRead(35);
    WireSlave.update();
    // let I2C and other ESP32 peripherals interrupts work
    delay(1);
}

//-----------------------------------------------------------
void requestEvent()
{
    lectura_1= String(voltaje1);
    lectura_2= String(voltaje2);
    c_retorno= String(retorno);
    //vector= mod+';'+lectura_1+';'+lectura_2+';'+c_retorno+';'+10;
    vector= mod+';'+lectura_1+';'+lectura_2+';'+10;
    WireSlave.print(vector);
  
}
//-----------------------------------------------------------
void receiveEvent(int howMany)
{
        while (1 < WireSlave.available()) // loop through all but the last byte
    {
        num[n] = WireSlave.read();  // receive byte as a character
        //Serial.print(c);            // print the character
        n++;
    }
    n=0;
    retorno=atoi(num);
    Serial.println(retorno);
    for(int j=0;j<10;j++)
      num[j]=NULL;
    
}
