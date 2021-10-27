//se incluyen librerias para uso MCP2515
#include <SPI.h>
#include <mcp2515.h>


//definicion pines UART 2 
#define RXD2 16
#define TXD2 17


//se declara funcion que guarda datos segun ESP32
void selec_mod(int);

//se definen estructuras del tipo CAN I/O
struct can_frame can_in,can_out; 

MCP2515 mcp2515(2); //se establece CS(SPI) en pin 2

//********************************************************************
//------------------ Variables globales -----------------------------

uint8_t ref1_1=0,ref1_2=0; //se guardan los numeros descompuestos  
uint16_t lectura_1=0,lectura_2=0;//recomposicion de
uint8_t count=1; //contador modulo
int c_lect=0; //contador serial
int can_id,lect[16];//datos de los esclavos
uint16_t ref[8]; //referencias desde maestro
char data_ser_in[48],data_ser_out[48]; //cadenas con info Serial I/O
char *vec_ser; //puntero descomposicion
char formato[]={"%d#%d;%d#%d"}; //formato Trama Virtual de salida

//------------------ Fin variables globales -------------------------
//********************************************************************

void setup() {
  
  can_out.can_id=0; //id del modulo arbitraje
  can_out.can_dlc=8; //cantidad de datos del bus
  //se establecen en 0 los Bytes de la trama
  can_out.data[0]=0;can_out.data[1]=0;
  can_out.data[2]=0;can_out.data[3]=0;
  can_out.data[4]=0;can_out.data[5]=0;
  can_out.data[6]=0;can_out.data[7]=0;
  
  //se inicia y config. el puerto Serial
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);//Serial 2
  
  //se config. el modo de operacion del modulo CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_100KBPS);
  mcp2515.setNormalMode();
}

void loop() {

//se establecen variables para registro del tiempo
static unsigned long t_Read = 0;
static unsigned long t_Write = 0;


//********************************************************************* 
//--------------------------Comunicacion CAN---------------------------
    
while(count<9){ //ciclo para alternar el orden de comunicacion de los ESP32
  
//----------------------------Envio datos------------------------------

  //ref1_1=(uint8_t) 0x00FF & ref[count-1]; 
  //ref1_2=(uint8_t) 0x00FF & (ref[count-1]>>8);
  can_out.data[0]=count; //establece a que ESP32 le toca enviar al BUS CAN
  mcp2515.sendMessage(&can_out); //se envia el mensaju al Bus CAN
  
//----------------------------Fin Envio datos--------------------------

//----------------------------Recepcion de datos------------------------  

  t_Read=millis(); //se guarda un registro de tiempo
  
    //se establece un tiempo máximo para esperar respuesta desde el ESP32 selecionado
    while(((abs(millis()-t_Read))<15)&&(!(mcp2515.readMessage(&can_in) == MCP2515::ERROR_OK)))
    {}
    
   //se recomponen los valores originales  de lecturas
   can_id=can_in.can_id;
   lectura_1=(int)(can_in.data[1] << 8 | can_in.data[0]);
   lectura_2=(int)(can_in.data[3] << 8 | can_in.data[2]);
   
   if((lectura_1<4096)&&(lectura_2<4096)) //filtro contra numeros con errores
    selec_mod(can_id); //se guardan lecturas en variables de ESP32 correspondiente
    
    count++; //se avanza al sig. ESP32 de la red

//-------------------------Fin Recepcion de datos-----------------------


//********************************************************************* 
//--------------------------Envio Serial--------------------------- 

//se copia lecturas a un formato de Trama Virtual
sprintf(data_ser_out,formato,can_id,lect[c_lect],can_id,lect[c_lect+1]);
//vector que se manda por serial
Serial2.write(data_ser_out); 

c_lect=c_lect+2; //contador lecturas

//reset contador
  if(c_lect>14)
    c_lect=0;

//------------------------Fin Envio Serial-------------------------
//*********************************************************************

}
count=1; //se reinicia el contador de modulo ESP32


//------------------------Fin Comunicacion CAN-------------------------
//********************************************************************* 


} //fin void loop


//--------------------------------------------------------------------
void selec_mod(int id){    //funcion que guarda datos segun ESP32
  if(id==1){
    lect[0]=lectura_1;lect[1]=lectura_2; 
  }
  if(id==2){
    lect[2]=lectura_1;lect[3]=lectura_2; 
  }
  if(id==3){
    lect[4]=lectura_1;lect[5]=lectura_2; 
  }
  if(id==4){
    lect[6]=lectura_1;lect[7]=lectura_2; 
  }
  if(id==5){
    lect[8]=lectura_1;lect[9]=lectura_2; 
  }
  if(id==6){
    lect[10]=lectura_1;lect[11]=lectura_2; 
  }
  if(id==7){
    lect[12]=lectura_1;lect[13]=lectura_2; 
  }
  if(id==8){
    lect[14]=lectura_1;lect[15]=lectura_2; 
  }
}