#include <SPI.h>
#include <mcp2515.h>

void selec_mod(int);

struct can_frame can_in,can_out;
MCP2515 mcp2515(2);
uint8_t ref1_1,ref1_2;
int lectura_1,lectura_2;//recomposicion de
int n_ser=0,count=1; //contadores
int can_id,lect[16];//datos de los esclavos
uint16_t ref[8]; //referencias desde maestro
char data_ser_in[48],data_ser_out[48]; //cadenas con info Serial I/O
char *vec_ser; //punteros descomposicion
char formato[]={";1#%d;2#%d;3#%d;4#%d;5#%d;6#%d;7#%d;8#%d;9#%d;10#%d;11#%d;12#%d;13#%d;14#%d;15#%d;16#%d"};
char carac='B'; //caracter que indica inicio y termino de trama 

void setup() {
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

static unsigned long lastRead = 0;

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
//--------------------------Comunicacion CAN---------------------------
    
while(count<9){
  
//----------------------------Envio datos------------------------------

  ref1_1=(uint8_t) 0x00FF & ref[count-1];
  ref1_2=(uint8_t) 0x00FF & (ref[count-1]>>8);
  can_out.data[0]=ref1_1;can_out.data[1]=ref1_2;
  can_out.can_id=count;
  mcp2515.sendMessage(&can_out);
  
//----------------------------Fin Envio datos--------------------------

//----------------------------Recepcion de datos------------------------  
  lastRead=millis();

    while(((abs(millis()-lastRead))<10)&&(!(mcp2515.readMessage(&can_in) == MCP2515::ERROR_OK)))
    {}
  can_id=can_in.can_id;
  lectura_1=(int)(can_in.data[1] << 8 | can_in.data[0]);
  lectura_2=(int)(can_in.data[3] << 8 | can_in.data[2]);
  selec_mod(can_id); //se guardan lecturas en el mod correspondiente
  //imprimir();
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

//---------------------------------------------------------
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
