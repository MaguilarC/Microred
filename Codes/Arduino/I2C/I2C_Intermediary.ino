
//inclusion librerias
#include <Arduino.h>
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlaveRequest.h>

//definicion pines UART 2 
#define RXD2 16
#define TXD2 17

//********************************************************************
//------------------ Variables globales -----------------------------

#define SDA_PIN 21
#define SCL_PIN 22 
#define MAX_RESP_LENGTH 32

//se asigna una dir. a cada ESP32
uint8_t I2C_slave_dir[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08}; 
char data_i2c_out[10],data_i2c_in[12]; //cadenas con la info I2C I/O
char data_ser_in[48],data_ser_out[48]; //cadenas con info Serial I/O 
int n_modulo,lect[16]; //arreglo datos de los esclavos
uint16_t ref[8]; //arreglo referencias recibidas
int flag=0; //aux. comunicacion I2C para alternar entre envio y peticion
int c_dir=0,c_lect=0,n_i2c=0,n_ser=0; //contadores indices
char *indice,*vec_ser; //punteros descomposicion
char formato[]={"%d#%d;%d#%d"}; //formato Trama Virtual de salida
char carac='B'; //caracter que indica inicio y termino de trama

//------------------ Fin variables globales -------------------------
//********************************************************************

void setup(){
  // Se inicia y establece vel. Serial
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); //Serial 2  
  Wire.begin(SDA_PIN, SCL_PIN); // Se inicia el I2C

void loop(){
  
//se establecen variables para registro del tiempo
  static unsigned long lastReadMillis = 0; 
  static unsigned long lastWireTransmit = 0;

//********************************************************************* 
//--------------------------Recepcion Serial--------------------------- 
  if(Serial2.available()>0){  
      while(carac!='A'){   //se espera hasta el inicio de trama
        carac=Serial2.read();  
      }
      while(carac!='B'){  //se copian los caracteres hasta el fin de trama
        carac=Serial2.read();
        if(carac!='B'){
          data_ser_in[n_ser]= carac;
          n_ser++;
        }
      }
    //Se descompone la Trama Virtual recibida
    vec_ser=strtok(data_ser_in,";");ref[0]=atoi(vec_ser);
    vec_ser=strtok(NULL,";");ref[1]=atoi(vec_ser);
    vec_ser=strtok(NULL,";");ref[2]=atoi(vec_ser);
    vec_ser=strtok(NULL,";");ref[3]=atoi(vec_ser);
    vec_ser=strtok(NULL,";");ref[4]=atoi(vec_ser);
    vec_ser=strtok(NULL,";");ref[5]=atoi(vec_ser);
    vec_ser=strtok(NULL,";");ref[6]=atoi(vec_ser);
    vec_ser=strtok(NULL,";");ref[7]=atoi(vec_ser);

    for (int m=0;m<n_ser-1;m++) //ciclo para limpiar cadena
      data_ser_in[m]=NULL;
    
    n_ser=0; //reset incide ref
  } 
  
//------------------------Fin Recepcion Serial-------------------------
//********************************************************************* 
   
//********************************************************************* 
//--------------------------Comunicacion I2C---------------------------

while(c_dir<8){ //ciclo para comunicar con los 8 ESP32 esclavos
  
//----------------------------Envio datos------------------------------
//se espera un t para enviar datos
  if (abs((millis()-lastWireTransmit)>5)&&(flag==0)) {  
    //se copia el valor de referencia a un formato
    sprintf(data_i2c_out,"%d;\n",ref[c_dir]); 
    //se empaqueta  
    WirePacker packer;
      packer.write(data_i2c_out);
    packer.end();
    //se envia al ESP32 con la dir. correspondiente    
    Wire.beginTransmission(I2C_slave_dir[c_dir]);
      while (packer.available()) {    
        Wire.write(packer.read());
      }
    Wire.endTransmission();

    //se guarda el tiempo en que se envio datos
    lastWireTransmit = millis();
      
    flag=1;//se cambia el modo del I2C a petición de datos 
    
    for(int j=0;j<12;j++)  //ciclo para limpiar cadena
      data_i2c_out[j]=NULL;

    //se guarda el tiempo en que se envio datos 
    lastReadMillis = millis();  
  }
  
//----------------------------Fin Envio datos--------------------------

//----------------------------Peticion de datos------------------------
//se espera un t para pedir datos 
  if ((abs(millis()-lastReadMillis)>5)&&(flag==1)) { 
    //Se realiza la peticion de datos al ESP32 de la dir. correspondiente
    WireSlaveRequest slaveReq(Wire, I2C_slave_dir[c_dir], MAX_RESP_LENGTH); 
    slaveReq.setRetryDelay(5);
    //se guarda el estado de la peticion
    bool success = slaveReq.request(); 
      
      if (success) { //si la peticion resulto satisfactoria
        while (1 < slaveReq.available()) {
         //se guarda de a un caracter
          data_i2c_in[n_i2c] = slaveReq.read(); 
          n_i2c++;
        }
        //Se descompone la Trama Virtual recibida
        indice=strtok(data_i2c_in,";");n_modulo=atoi(indice);
        indice=strtok(NULL,";");lect[c_lect]=atoi(indice);
        indice=strtok(NULL,";");lect[c_lect+1]=atoi(indice);
        n_i2c=0; //reset cont. indice
        
      }else {//si la peticion no resulto satisfactoria
            //se imprime el error
            Serial.println(slaveReq.lastStatusToString()); 
       }
       
//*****************************************************************
//--------------------------Envio Serial---------------------------
       //se copia lecturas a un formato de Trama Virtual
       sprintf(data_ser_out,formato,c_dir+1,lect[c_lect],
       c_dir+1,lect[c_lect+1]);
       //vector que se manda por serial
       Serial2.write(data_ser_out); 
//------------------------Fin Envio Serial-------------------------
//******************************************************************
      
      for(int j=0;j<12;j++)  //ciclo para limpiar cadena
        data_i2c_in[j]=NULL;  
        
     //aumento indices
     c_dir++;  
     c_lect=c_lect+2;
     
     flag=0; //se cambia el modo del I2C a envío de datos  
  }         
//----------------------------Fin Peticion datos--------------------------
}

//reset contadores indices
c_dir=0;
if(c_lect>15)
    c_lect=0;
//---------------------------Fin comunicacion I2C-------------------------
//************************************************************************  


}//fin void loop 
