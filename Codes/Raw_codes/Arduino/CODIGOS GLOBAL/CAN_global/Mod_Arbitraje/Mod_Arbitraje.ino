#include <SPI.h>
#include <mcp2515.h>

void selec_mod(int);

struct can_frame can_in,can_out;
MCP2515 mcp2515(2);
uint8_t ref1_1,ref1_2;
uint16_t lectura_1=0,lectura_2=0;//recomposicion de
uint8_t count=1; //contador modulo
int n_ser=0; //contador serial
int can_id,lect[16];//datos de los esclavos
uint16_t ref[8]; //referencias desde maestro
char data_ser_in[48],data_ser_out[48]; //cadenas con info Serial I/O
char *vec_ser; //punteros descomposicion
char formato[]={";1#%d;2#%d;3#%d;4#%d;5#%d;6#%d;7#%d;8#%d;9#%d;10#%d;11#%d;12#%d;13#%d;14#%d;15#%d;16#%d"};
char carac='B'; //caracter que indica inicio y termino de trama 

void setup() {
  can_out.can_id=0; //id del modulo arbitraje
  can_out.can_dlc=8; //cantidad de datos del bus
  can_out.data[0]=0;can_out.data[1]=0;
  can_out.data[2]=0;can_out.data[3]=0;
  can_out.data[4]=0;can_out.data[5]=0;
  can_out.data[6]=0;can_out.data[7]=0;
  Serial.begin(115200);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_100KBPS);
  mcp2515.setNormalMode();
}

void loop() {

static unsigned long t_Read = 0;
static unsigned long t_Write = 0;


//********************************************************************* 
//--------------------------Comunicacion CAN---------------------------
    
while(count<9){
  
//----------------------------Envio datos------------------------------

  ref1_1=(uint8_t) 0x00FF & ref[count-1];
  ref1_2=(uint8_t) 0x00FF & (ref[count-1]>>8);
  can_out.data[0]=count; //envia a quien corresponde el pase
  mcp2515.sendMessage(&can_out);
  
//----------------------------Fin Envio datos--------------------------

//----------------------------Recepcion de datos------------------------  
  t_Read=millis();

    while(((abs(millis()-t_Read))<15)&&(!(mcp2515.readMessage(&can_in) == MCP2515::ERROR_OK)))
    {}
    can_id=can_in.can_id;
    lectura_1=(int)(can_in.data[1] << 8 | can_in.data[0]);
    lectura_2=(int)(can_in.data[3] << 8 | can_in.data[2]);
    /*Serial.print(can_in.can_id);Serial.print(" ; ");
    Serial.print(lectura_1);Serial.print(" ; ");
    Serial.println(lectura_2);*/
    if((lectura_1<4096)&&(lectura_2<4096))
    selec_mod(can_id); //se guardan lecturas en el mod correspondiente
    
    count++;
}
count=1;
//-------------------------Fin Recepcion de datos-----------------------

//------------------------Fin Comunicacion CAN-------------------------
//********************************************************************* 

//********************************************************************* 
//--------------------------Envio Serial--------------------------- 


sprintf(data_ser_out,formato,lect[0],lect[1],lect[2],lect[3],lect[4],
lect[5],lect[6],lect[7],lect[8],lect[9],lect[10],lect[11],lect[12],
lect[13],lect[14],lect[15]);
Serial.write(data_ser_out); //vector mandado por serial

//------------------------Fin Envio Serial-------------------------
//*********************************************************************

} //fin void loop


//---------------------------------------------------------
void selec_mod(int id){
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
