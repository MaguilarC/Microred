#include <SPI.h>
#include <mcp2515.h>

struct can_frame can_out,can_in;

MCP2515 mcp2515(2);

uint16_t lectura_1=0,lectura_2=0;
uint8_t lect1_1,lect1_2,lect2_1,lect2_2;
int id=8; //id esclavo
int ref;

void setup() {
  can_out.can_id= id; //id del esclavo
  can_out.can_dlc=8; //cantidad de datos del bus
  while (!Serial);
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_100KBPS);
  mcp2515.setNormalMode();
}

void loop() {
static unsigned long lastRead = 0;

while(!(mcp2515.readMessage(&can_in) == MCP2515::ERROR_OK)) {  
  lectura_1=analogRead(34);
  lectura_2=analogRead(35);
  
  //Descomposicion entero en 2 bytes
  lect1_1=(uint8_t) 0x00FF & lectura_1;
  lect1_2=(uint8_t) 0x00FF & (lectura_1>>8);
  lect2_1=(uint8_t) 0x00FF & lectura_2;
  lect2_2=(uint8_t) 0x00FF & (lectura_2>>8);
  
  can_out.data[0]= lect1_1;can_out.data[1]= lect1_2;
  can_out.data[2]= lect2_1;can_out.data[3]= lect2_2;
  can_out.data[4]= 0;can_out.data[5]= 0;
  can_out.data[6]= 0;can_out.data[7]= 0;
}
  if(can_in.can_id==id){
    mcp2515.sendMessage(&can_out);
    ref=(int)(can_in.data[1] << 8 | can_in.data[0]);
    Serial.println(millis()-lastRead);
    Serial.println(ref);
    lastRead=millis();
  }  
}
