#include <SPI.h>
#include <mcp2515.h>

//definicion pines UART 2 
#define RXD2 16
#define TXD2 17

void selec_mod(int);

struct can_frame can_in,can_out;
MCP2515 mcp2515(2);
uint8_t ref1_1,ref1_2;
uint16_t lectura_1=0,lectura_2=0;//recomposicion de
uint8_t count=1; //contador modulo
uint8_t n_char=0,n_char2=0,flag1=0,flag2=0;
int n_ser=0; //contador serial
int can_id,lect[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//datos de los esclavos
uint16_t ref_mat[3]; //referencias desde matlab
uint8_t ref[]={0,0,0,0,0,0,0,0}; //referencias
uint8_t ref2[]={0,0,0,0,0,0,0,0}; //referencias
char cadena[40],cadena2[40];
char data_ser_in[15],data_ser_out[15]; //cadenas con info Serial I/O
char *vec_ser; //punteros descomposicion
char formato[]={";%d#%d;%d#%d"};

void setup() {
  can_out.can_id=0; //id del modulo arbitraje
  can_out.can_dlc=8; //cantidad de datos del bus
  can_out.data[0]=0;can_out.data[1]=0;
  can_out.data[2]=0;can_out.data[3]=0;
  can_out.data[4]=0;can_out.data[5]=0;
  can_out.data[6]=0;can_out.data[7]=0;
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);//Serial 2
  mcp2515.reset();
  mcp2515.setBitrate(CAN_100KBPS);
  mcp2515.setNormalMode();
}

void loop() {

static unsigned long t_Read = 0;
static unsigned long t_Write = 0;

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
//--------------------------Comunicacion CAN---------------------------
    
while(count<9){
  
//----------------------------Envio datos------------------------------
  /*
  ref1_1=(uint8_t) 0x00FF & ref[count-1];
  ref1_2=(uint8_t) 0x00FF & (ref[count-1]>>8);*/
  
  can_out.data[0]=count; //envia a quien corresponde el pase
  can_out.data[1]=ref[count-1];//duty cycle
  can_out.data[2]=ref2[count-1];//señal
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


} //fin void loop


//---------------------------------------------------------
void selec_mod(int id){
  if(id==1){
    lect[0]=lectura_1;lect[1]=lectura_2;
    sprintf(data_ser_out,formato,1,
    lect[0],2,lect[1]);
    Serial2.write(data_ser_out);
  }
  if(id==2){
    lect[2]=lectura_1;lect[3]=lectura_2; 
    sprintf(data_ser_out,formato,3,
    lect[2],4,lect[3]);
    Serial2.write(data_ser_out);
  }
  if(id==3){
    lect[4]=lectura_1;lect[5]=lectura_2;
    sprintf(data_ser_out,formato,5,
    lect[4],6,lect[5]);
    Serial2.write(data_ser_out); 
  }
  if(id==4){
    lect[6]=lectura_1;lect[7]=lectura_2;
    sprintf(data_ser_out,formato,7,
    lect[6],8,lect[7]);
    Serial2.write(data_ser_out);
  }
  if(id==5){
    lect[8]=lectura_1;lect[9]=lectura_2;
    sprintf(data_ser_out,formato,9,
    lect[8],10,lect[9]);
    Serial2.write(data_ser_out);
  }
  if(id==6){
    lect[10]=lectura_1;lect[11]=lectura_2;
    sprintf(data_ser_out,formato,11,
    lect[10],12,lect[11]);
    Serial2.write(data_ser_out);
  }
  if(id==7){
    lect[12]=lectura_1;lect[13]=lectura_2;
    sprintf(data_ser_out,formato,13,
    lect[12],14,lect[13]);
    Serial2.write(data_ser_out);
  }
  if(id==8){
    lect[14]=lectura_1;lect[15]=lectura_2;
    sprintf(data_ser_out,formato,15,
    lect[14],16,lect[15]);
    Serial2.write(data_ser_out); 
  }
}
