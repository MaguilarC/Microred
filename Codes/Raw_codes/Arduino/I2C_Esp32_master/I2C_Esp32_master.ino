
#include <Arduino.h>
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlaveRequest.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 8  //direccion esclavo

#define MAX_SLAVE_RESPONSE_LENGTH 32

int lect1,lect2,mod,s_retorno,c1=0,c2=0,coma=0,n=0;
char c,cadena[30],c_retorno[10];
int n_retorno=4200,rearmado;
uint8_t data0=0,data1;
char *indice;

void setup()
{
    Serial.begin(115200);           // start serial for output
    Wire.begin(SDA_PIN, SCL_PIN);   // join i2c bus
}

void loop()
{
    static unsigned long lastReadMillis = 0;
    static unsigned long lastWireTransmit = 0;
    static byte x = 0, y=10;
   
//*********************************************************************    
//----------------------------Envio datos------------------------------

    
    // send data to WireSlave device every 1000 ms
    if (abs(millis()-lastWireTransmit) > 10) {
        // first create a WirePacker that will assemble a packet

        sprintf(c_retorno,"%d\n",n_retorno);
        //Serial.println(c_retorno); 
        WirePacker packer;
        packer.write(c_retorno);
        packer.end();   
        Wire.beginTransmission(I2C_SLAVE_ADDR);
        while (packer.available()) {    // write every packet byte
            Wire.write(packer.read());
        }
        Wire.endTransmission();         // stop transmitting
        lastWireTransmit = millis();
      }
    for(int j=0;j<10;j++)
      c_retorno[j]=NULL;

//----------------------------Fin Envio datos--------------------------
//*********************************************************************  
//delay(50);
//*********************************************************************
//----------------------------Peticion de datos------------------------

    if (abs(millis()-lastReadMillis) > 10) {

        WireSlaveRequest slaveReq(Wire, I2C_SLAVE_ADDR, MAX_SLAVE_RESPONSE_LENGTH);
        slaveReq.setRetryDelay(5);
        bool success = slaveReq.request();
        
        if (success) {
            while (1 < slaveReq.available()) {  // loop through all but the last byte
                cadena[n] = slaveReq.read();       // receive byte as a character
                //Serial.println(cadena[n]);                // print the character
                n++;
            }
            indice=strtok(cadena,";");
            mod=atoi(indice);

            indice=strtok(NULL,";");
            lect1=atoi(indice);

            indice=strtok(NULL,";");
            lect2=atoi(indice);

           /* indice=strtok(NULL,";");
            s_retorno=atoi(indice);*/
            
            Serial.print("lectura 1: ");Serial.println(lect1);
            Serial.print("lectura 2: ");Serial.println(lect2);
           // Serial.print("retorno: ");Serial.println(s_retorno);
            Serial.print("Modulo: ");Serial.println(mod);
            n=0; 
        }
        else {
            Serial.println(slaveReq.lastStatusToString());
        }
        lastReadMillis = millis();
    }

//----------------------------Fin Peticion datos--------------------------
//*********************************************************************  
    
}

/*

data0= 0x00FF & retorno;
data1= 0x00FF & (retorno >> 8);
 */
