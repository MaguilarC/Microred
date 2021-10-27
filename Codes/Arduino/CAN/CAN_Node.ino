//se incluyen librerias para uso MCP2515
#include <SPI.h>
#include <mcp2515.h>

//se establece una tarea (TASK) con nombre 'CAN_task'
TaskHandle_t CAN_task;

//se definen estructuras del tipo CAN I/O
struct can_frame can_out,can_in; 

MCP2515 mcp2515(2); //se establece CS(SPI) en pin 2

//se declara funcion que guarda datos segun ESP32
void receptor(int ); 

//********************************************************************
//------------------ Variables globales -----------------------------

//se crea estructura con un formato para guardar lecturas
typedef struct struct_mensaje {
    int lect1;
    int lect2;
} struct_mensaje;

//Se crean estructuras con el mismo formato
struct_mensaje mod1,mod2,mod3,mod4,mod5,mod6,mod7,mod8;
struct_mensaje buf_In;

//se crean variables globales
uint16_t lectura_1=0,lectura_2=0; //variable lecturas
uint8_t lect1_1,lect1_2,lect2_1,lect2_2;
int id=1; //id de este modulo ESP32
int t_com=0;

//------------------ Fin variables globales -------------------------
//********************************************************************

void setup() {
  
  can_out.can_id= id; //id de este modulo ESP32
  can_out.can_dlc=8; //cantidad de datos del bus

  //se config. el modo de operacion del modulo CAN
  mcp2515.reset();
  mcp2515.setBitrate(CAN_100KBPS);
  mcp2515.setNormalMode();

  Serial.begin(115200);//se inicia y config. el puerto Serial

//Se establece la 'CAN_task'  en el Nucleo 0
    xTaskCreatePinnedToCore(
                    Com_CAN,   /* funcion de la tarea */
                    "Com_CAN",     /* nombre de la tarea. */
                    10000,       /* Tamaño de pila de la tarea*/
                    NULL,        /* Parametro de la tarea */
                    1,           /* prioridad de la tarea(0-24) */
                    &CAN_task,      /* Ident. de tareas para seguimiento de la tarea creada */
                    0);          /* fijar la tarea al Nucleo 0 */
    delay(500); //se espera 500ms
  
}

//funcion que realiza la 'CAN_task' en el Nucleo 0
void Com_CAN( void * pvParameters ){
  /*
   //imprime en que nucleo se ejecuta la tarea
  Serial.print("Com. CAN corre en el núcleo: ");
  Serial.println(xPortGetCoreID()); */
  
static unsigned long lastWrite = 0; //variable para registro de tiempo

for(;;){ //ciclo infinito

  //mientras no se reciban datos desde el Bus CAN   
  while(!(mcp2515.readMessage(&can_in) == MCP2515::ERROR_OK)){
     
    //se realiza lecturas analog. en los pines 34 y 35
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
  
  //si el mensaje es del arbitro y se selecciono este modulo
  if((can_in.can_id==0)&&(can_in.data[0]==id)){ 
    mcp2515.sendMessage(&can_out); //envia sus datos
    /*
    t_com=millis()-lastWrite;
    lastWrite=millis(); //marca de tiempo*/
  }
  if(can_in.can_id!=0){ //si el mensaje no es del arbitro
    //al recibir datos se recomponen
    buf_In.lect1=(int)(can_in.data[1] << 8 | can_in.data[0]);
    buf_In.lect2=(int)(can_in.data[3] << 8 | can_in.data[2]);
    if((buf_In.lect1<4096)&&(buf_In.lect2<4096)) //Se comprueba que los valores sean correctos
      mod_receptor(can_in.can_id); //funcion guarda datos de mod corresp.
      
  }
  vTaskDelay(1); //la tarea para por 1 milisegundo 
     
 }//fin for
 
}//fin Tarea comunicación CAN

void loop() {

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