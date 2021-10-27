
#include <Arduino.h>
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlaveRequest.h>

#define SDA_PIN 21
#define SCL_PIN 22 
#define MAX_RESP_LENGTH 32

void envio_i2c(uint8_t, char[]);
void peticion_i2c(uint8_t);

//uint8_t mod_selec=8;
uint8_t I2C_slave_dir[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
char data_i2c_out[10],data_i2c_in[12]; //cadenas con la info i2c I/O
int n_modulo,lecturas[16]; //arreglo con lecturas de los esclavos
int n_retorno=4000,n=0,flag=0;
int c_dir=0,c_lect=0; //contadores
char *indice;

void setup(){
  Serial.begin(115200);           // start serial for output
  Wire.begin(SDA_PIN, SCL_PIN);   // join i2c bus
}

void loop(){
  static unsigned long lastReadMillis = 0;
  static unsigned long lastWireTransmit = 0;
   
//********************************************************************* 
//--------------------------Comunicacion I2C---------------------------

//----------------------------Envio datos------------------------------
  if (abs((millis()-lastWireTransmit) > 10)&&(flag==0)) {
    sprintf(data_i2c_out,"%d\n",n_retorno);
    WirePacker packer;
      packer.write(data_i2c_out);
    packer.end();   
    Wire.beginTransmission(I2C_slave_dir[c_dir]); //direccion
      while (packer.available()) {    
        Wire.write(packer.read());
      }
    Wire.endTransmission();         
      lastWireTransmit = millis();
    flag=1;
  }
//----------------------------Fin Envio datos--------------------------

//----------------------------Peticion de datos------------------------

  //if (abs(millis()-lastReadMillis) > 5) {
  if(flag==1){
    WireSlaveRequest slaveReq(Wire, I2C_slave_dir[c_dir], MAX_RESP_LENGTH); //direccion
    slaveReq.setRetryDelay(5);
    bool success = slaveReq.request();
      if (success) {
        while (1 < slaveReq.available()) {  
          data_i2c_in[n] = slaveReq.read(); 
          n++;
        }
        n=0;
        indice=strtok(data_i2c_in,";");n_modulo=atoi(indice);
        indice=strtok(NULL,";");lecturas[c_lect]=atoi(indice);
        indice=strtok(NULL,";");lecturas[c_lect+1]=atoi(indice);
      }else {
            Serial.println(slaveReq.lastStatusToString());
        }
        lastReadMillis = millis();
      for(int j=0;j<10;j++)
        data_i2c_in[j]=NULL;  
     
     c_dir++;
     c_lect=c_lect+2;
      if(c_dir>7)
        c_dir=0;
      if(c_lect>15)
        c_lect=0; 
      flag=0;  
      imprimir();
  }    

     
//----------------------------Fin Peticion datos--------------------------

//---------------------------Fin comunicacion I2C-------------------------
//************************************************************************  


}

void imprimir(){
Serial.print("modulo: 1");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[0]);
Serial.print("; lectura 2: ");Serial.println(lecturas[1]);
Serial.print("modulo: 2");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[2]);
Serial.print("; lectura 2: ");Serial.println(lecturas[3]);
Serial.print("modulo: 3");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[4]);
Serial.print("; lectura 2: ");Serial.println(lecturas[5]);
Serial.print("modulo: 4");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[6]);
Serial.print("; lectura 2: ");Serial.println(lecturas[7]);
Serial.print("modulo: 5");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[8]);
Serial.print("; lectura 2: ");Serial.println(lecturas[9]);
Serial.print("modulo: 6");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[10]);
Serial.print("; lectura 2: ");Serial.println(lecturas[11]);
Serial.print("modulo: 7");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[12]);
Serial.print("; lectura 2: ");Serial.println(lecturas[13]);
Serial.print("modulo: 8");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lecturas[14]);
Serial.print("; lectura 2: ");Serial.println(lecturas[15]);
Serial.println("-------------");
  
}
