
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
char data_i2c_out[10],data_i2c_in[12]; //cadenas con la info I2C I/O
char data_ser_in[48],data_ser_out[48]; //cadenas con info Serial I/O 
int n_modulo,lect[16]; //arreglo datos de los esclavos
uint16_t ref[8];
int flag=0; //auxiliar comunicacion I2C
int c_dir=0,c_lect=0,n_i2c=0,n_ser=0; //contadores indices
char *indice,*vec_ser; //punteros descomposicion
char formato[]={";1#%d;2#%d;3#%d;4#%d;5#%d;6#%d;7#%d;8#%d;9#%d;10#%d;11#%d;12#%d;13#%d;14#%d;15#%d;16#%d"};
char carac='B'; //caracter que indica inicio y termino de trama

void setup(){
  Serial.begin(9600);           // start serial for output
  Wire.begin(SDA_PIN, SCL_PIN);   // join i2c bus
}

void loop(){
  static unsigned long lastReadMillis = 0;
  static unsigned long lastWireTransmit = 0;

//********************************************************************* 
//--------------------------Recepcion Serial--------------------------- 
  if(Serial.available()>0){  
      while(carac!='A'){   //se espera hasta el inicio de trama
        carac=Serial.read();  
      }
      while(carac!='B'){  //se copian los caracteres hasta el fin de trama
        carac=Serial.read();
        if(carac!='B'){
          data_ser_in[n_ser]= carac;
          n_ser++;
        }
      }
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
    
    n_ser=0;
  } 
  
//------------------------Fin Recepcion Serial-------------------------
//********************************************************************* 
   
//********************************************************************* 
//--------------------------Comunicacion I2C---------------------------

while(c_dir<8){
//----------------------------Envio datos------------------------------
  if (abs((millis()-lastWireTransmit)>5)&&(flag==0)) {
    sprintf(data_i2c_out,"%d;\n",ref[c_dir]);  
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
    
    for(int j=0;j<12;j++)  //ciclo para limpiar cadena
      data_i2c_out[j]=NULL;
    lastReadMillis = millis();  
  }
  
//----------------------------Fin Envio datos--------------------------

//----------------------------Peticion de datos------------------------
  if ((abs(millis()-lastReadMillis)>5)&&(flag==1)) {
  //if(flag==1){
    WireSlaveRequest slaveReq(Wire, I2C_slave_dir[c_dir], MAX_RESP_LENGTH); //direccion
    slaveReq.setRetryDelay(5);
    bool success = slaveReq.request();
      if (success) {
        while (1 < slaveReq.available()) {  
          data_i2c_in[n_i2c] = slaveReq.read(); 
          n_i2c++;
        }
        n_i2c=0;
        indice=strtok(data_i2c_in,";");n_modulo=atoi(indice);
        indice=strtok(NULL,";");lect[c_lect]=atoi(indice);
        indice=strtok(NULL,";");lect[c_lect+1]=atoi(indice);
      }else {
            Serial.println(slaveReq.lastStatusToString());
       }
      //lastReadMillis = millis();
      
      for(int j=0;j<12;j++)  //ciclo para limpiar cadena
        data_i2c_in[j]=NULL;  
     
     c_dir++;  //aumento indices
     c_lect=c_lect+2;
     flag=0;  
     //imprimir();  //funcion para imprimir lecturas en pantalla
  }         
//----------------------------Fin Peticion datos--------------------------
}
  if(c_dir>7) //reset contadores indices
    c_dir=0;
  if(c_lect>15)
    c_lect=0;
//---------------------------Fin comunicacion I2C-------------------------
//************************************************************************  

//********************************************************************* 
//--------------------------Envio Serial--------------------------- 

sprintf(data_ser_out,formato,lect[0],lect[1],lect[2],lect[3],lect[4],
lect[5],lect[6],lect[7],lect[8],lect[9],lect[10],lect[11],lect[12],
lect[13],lect[14],lect[15]);
Serial.write(data_ser_out); //vector mandado por serial

//------------------------Fin Envio Serial-------------------------
//*********************************************************************


}//fin void loop


//---------------------------------------------------------------------------
void imprimir(){
Serial.print("modulo: 1");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[0]);
Serial.print("; lectura 2: ");Serial.println(lect[1]);
Serial.print("modulo: 2");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[2]);
Serial.print("; lectura 2: ");Serial.println(lect[3]);
Serial.print("modulo: 3");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[4]);
Serial.print("; lectura 2: ");Serial.println(lect[5]);
Serial.print("modulo: 4");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[6]);
Serial.print("; lectura 2: ");Serial.println(lect[7]);
Serial.print("modulo: 5");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[8]);
Serial.print("; lectura 2: ");Serial.println(lect[9]);
Serial.print("modulo: 6");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[10]);
Serial.print("; lectura 2: ");Serial.println(lect[11]);
Serial.print("modulo: 7");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[12]);
Serial.print("; lectura 2: ");Serial.println(lect[13]);
Serial.print("modulo: 8");//Serial.println(n_modulo);
Serial.print("; lectura 1: ");Serial.print(lect[14]);
Serial.print("; lectura 2: ");Serial.println(lect[15]);
Serial.println("-------------");
  
}
