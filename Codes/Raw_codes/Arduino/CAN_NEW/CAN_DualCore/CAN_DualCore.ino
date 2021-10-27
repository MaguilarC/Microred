#include <SPI.h>
#include <mcp2515.h>

TaskHandle_t CAN_task;

struct can_frame can_out,can_in; //tramas Can in/out

MCP2515 mcp2515(2);

void receptor(int );

typedef struct struct_mensaje {
    int lect1;
    int lect2;
} struct_mensaje;

struct_mensaje mod1,mod2,mod3,mod4,mod5,mod6,mod7,mod8;
struct_mensaje buf_In;

uint16_t lectura_1=0,lectura_2=0;
uint8_t lect1_1,lect1_2,lect2_1,lect2_2;
int id=1; //id modulo
int t_com=0;

void setup() {
  can_out.can_id= id;
  can_out.can_dlc=8; //cantidad de datos del bus
  Serial.begin(115200);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_100KBPS);
  mcp2515.setNormalMode();

    xTaskCreatePinnedToCore(
                    Com_CAN,   /* Task function. */
                    "Com_CAN",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task(0-24) */
                    &CAN_task,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */
    delay(500);
  
}

void Com_CAN( void * pvParameters ){
  Serial.print("Com. CAN corre en el núcleo: ");
  Serial.println(xPortGetCoreID());
  
static unsigned long lastWrite = 0;
for(;;){ //ciclo infinito
    
  while(!(mcp2515.readMessage(&can_in) == MCP2515::ERROR_OK)){ 
    lectura_1=analogRead(34);
    lectura_2=analogRead(35);

    //Descomposicion entero en 2 bytes
    lect1_1=(uint8_t) 0x00FF & lectura_1;
    lect1_2=(uint8_t) 0x00FF & (lectura_1>>8);
    lect2_1=(uint8_t) 0x00FF & lectura_2;
    lect2_2=(uint8_t) 0x00FF & (lectura_2>>8);

    //llenado de trama de salida
    can_out.data[0]= lect1_1;can_out.data[1]= lect1_2;
    can_out.data[2]= lect2_1;can_out.data[3]= lect2_2;
    can_out.data[4]= 0;can_out.data[5]= 0;
    can_out.data[6]= 0;can_out.data[7]= 0;
  } 
  if((can_in.can_id==0)&&(can_in.data[0]==id)){  //si el mensaje es del arbitro y dio el pase
    mcp2515.sendMessage(&can_out); //envia sus datos
    t_com=millis()-lastWrite;
    lastWrite=millis();
  }
  if(can_in.can_id!=0){
    //al recibir datos se recomponen
    buf_In.lect1=(int)(can_in.data[1] << 8 | can_in.data[0]);
    buf_In.lect2=(int)(can_in.data[3] << 8 | can_in.data[2]);
    if((buf_In.lect1<4096)&&(buf_In.lect2<4096)) //Se comprueba que los valores sean correctos
      mod_receptor(can_in.can_id); //funcion guarda datos de mod corresp.
      
  }
  vTaskDelay(1); //la tarea para por 1 milisegundo    
 }
}//fin comunicación CAN

void loop() {

Serial.print("1");Serial.print(" ; ");
Serial.print(mod1.lect1);Serial.print(" ; ");
Serial.println(mod1.lect2);
Serial.print("2");Serial.print(" ; ");
Serial.print(mod2.lect1);Serial.print(" ; ");
Serial.println(mod2.lect2);
Serial.print("3");Serial.print(" ; ");
Serial.print(mod3.lect1);Serial.print(" ; ");
Serial.println(mod3.lect2); 
Serial.print("4");Serial.print(" ; ");
Serial.print(mod4.lect1);Serial.print(" ; ");
Serial.println(mod4.lect2); 
Serial.print("5");Serial.print(" ; ");
Serial.print(mod5.lect1);Serial.print(" ; ");
Serial.println(mod5.lect2); 
Serial.print("6");Serial.print(" ; ");
Serial.print(mod6.lect1);Serial.print(" ; ");
Serial.println(mod6.lect2);
Serial.print("7");Serial.print(" ; ");
Serial.print(mod7.lect1);Serial.print(" ; ");
Serial.println(mod7.lect2);
Serial.print("8");Serial.print(" ; ");
Serial.print(mod8.lect1);Serial.print(" ; ");
Serial.println(mod8.lect2);
Serial.println(t_com);
vTaskDelay(1);//la tarea para por 1 milisegundo    
}//fin void loop
//-----------------------------------------------------------------
void mod_receptor(int id_in){ //filtro de a que modulo
                     //corresponde los datos recibidos
   if(id_in==1)
    mod1=buf_In;
   if(id_in==2)
    mod2=buf_In;
   if(id_in==3)
    mod3=buf_In;
   if(id_in==4)
    mod4=buf_In;
   if(id_in==5)
    mod5=buf_In;
   if(id_in==6)
    mod6=buf_In;
   if(id_in==7)
    mod7=buf_In;
   if(id_in==8)
    mod8=buf_In;    
  }
//-----------------------------------------------------------------
