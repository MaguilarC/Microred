
#include <Arduino.h>
#include <Wire.h>
#include <WirePacker.h>
#include <WireSlaveRequest.h>

#define SDA_PIN 21
#define SCL_PIN 22 
#define MAX_RESP_LENGTH 32

//definicion pines UART 2 
#define RXD2 16
#define TXD2 17


//uint8_t mod_selec=8;
uint8_t I2C_slave_dir[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
char data_i2c_out[10],data_i2c_in[12]; //cadenas con la info I2C I/O
char data_ser_in[48],data_ser_out[30]; //cadenas con info Serial I/O 
int n_modulo,lect[16]; //arreglo datos de los esclavos
int flag=0; //auxiliar comunicacion I2C
uint8_t flag1=0,flag2=0,n_char=0,n_char2=0;
int c_dir=0,c_lect=0,n_i2c=0; //contadores indices
uint8_t ref[]={0,0,0,0,0,0,0,0}; //referencias
uint8_t ref2[]={0,0,0,0,0,0,0,0}; //referencias
uint16_t ref_mat[3]; //referencias desde matlab
int n_ser=0; //contador serial
char *indice,*vec_ser; //punteros descomposicion
char cadena[40],cadena2[40];
char formato[]={";%d#%d;%d#%d"};


void setup(){
  Serial.begin(115200);           // start serial for output
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);//Serial 2
  Wire.begin(SDA_PIN, SCL_PIN);   // join i2c bus
}

void loop(){
  static unsigned long lastReadMillis = 0;
  static unsigned long lastWireTransmit = 0;

   //*******************************************
//-------------Serial de entrada-------------

  if(Serial2.available()>1){
    while(Serial2.available()>0){
      cadena[n_ser]=Serial2.read();
      n_ser++;
    }
  
  n_ser=0;

  while((flag1==0)&&(n_char<40)){
    if(cadena[n_char]=='A')
      flag1=1;
    else
      n_char++;  
  }
  n_char++;
  while((flag2==0)&&((n_char<40))){
    if(cadena[n_char]=='B')
      flag2=1;
    else
      cadena2[n_char2]=cadena[n_char];
    n_char++;
    n_char2++;
  }  
flag1=0;flag2=0;
n_char=0;n_char2=0;

    vec_ser=strtok(cadena2,";");ref_mat[0]=atoi(vec_ser);//modulo
    vec_ser=strtok(NULL,";");ref_mat[1]=atoi(vec_ser);//duty cycle
    vec_ser=strtok(NULL,";");ref_mat[2]=atoi(vec_ser);//señal
/*    
Serial.print(ref_mat[0]);Serial.print(";");
Serial.print(ref_mat[1]);Serial.print(";");
Serial.println(ref_mat[2]);*/

ref[ref_mat[0]-1]=ref_mat[1];//duty cycle
ref2[ref_mat[0]-1]=ref_mat[2];//señal

  for(int g=0;g<40;g++){
    cadena[g]=NULL;
    cadena2[g]=NULL;  
  }
  } 
//-----------fin Serial de entrada-------------
//*******************************************
//********************************************************************* 
//--------------------------Comunicacion I2C---------------------------

while(c_dir<8){
//----------------------------Envio datos------------------------------
  if (abs((millis()-lastWireTransmit)>8)&&(flag==0)) {
    sprintf(data_i2c_out,"%d;%d;%d;\n",c_dir+1,ref[c_dir],ref2[c_dir]);  
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
  if ((abs(millis()-lastReadMillis)>8)&&(flag==1)) {

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
        
//********************************************************************* 
//--------------------------Envio Serial--------------------------- 
        sprintf(data_ser_out,formato,c_lect+1,lect[c_lect],c_lect+2,lect[c_lect+1]);
        Serial2.write(data_ser_out);
        Serial.println(millis()-lastReadMillis);
//------------------------Fin Envio Serial-------------------------
//*********************************************************************

      }else {
            Serial.println(slaveReq.lastStatusToString());
       }
      //lastReadMillis = millis();
      
      for(int j=0;j<12;j++)  //ciclo para limpiar cadena
        data_i2c_in[j]=NULL;  
     
     c_dir++;  //aumento indices
     c_lect=c_lect+2;
     flag=0;  
     
  }         
//----------------------------Fin Peticion datos--------------------------
}
  if(c_dir>7) //reset contadores indices
    c_dir=0;
  if(c_lect>15)
    c_lect=0;
//---------------------------Fin comunicacion I2C-------------------------
//************************************************************************  



}//fin void loop


//---------------------------------------------------------------------------
