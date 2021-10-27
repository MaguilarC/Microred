#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg1;

MCP2515 mcp2515(2);

uint16_t lectura_1=0,lectura_2=0;
uint8_t lect1_1,lect1_2;
uint8_t lect2_1,lect2_2;


void setup() {
  canMsg1.can_id  =1; //id del esclavo
  canMsg1.can_dlc = 8; //cantidad de datos del bus
  while (!Serial);
  Serial.begin(115200);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_100KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {

  
lectura_1=analogRead(34);
lectura_2=analogRead(35);

/*
lectura_1=400;
lectura_2=300;
*/  
lect1_1=(uint8_t) 0x00FF & lectura_1;
lect1_2=(uint8_t) 0x00FF & (lectura_1>>8);
lect2_1=(uint8_t) 0x00FF & lectura_2;
lect2_2=(uint8_t) 0x00FF & (lectura_2>>8);
canMsg1.data[0]= lect1_1;canMsg1.data[1]= lect1_2;
canMsg1.data[2]= lect2_1;canMsg1.data[3]= lect2_2;
canMsg1.data[4]= 0;canMsg1.data[5]= 0;
canMsg1.data[6]= 0;canMsg1.data[7]= 0;
mcp2515.sendMessage(&canMsg1);
/*Serial.println(lectura_1);
Serial.print(lect1_1);Serial.println(lect1_2);
*/
//delay(20);
}
