#include <esp_now.h>
#include <WiFi.h>

#define RXD2 16
#define TXD2 17

uint8_t id=0; //id modulo
uint8_t mod_count=0,i=0,j=0;
uint16_t reenvio=0,veces=0;
unsigned long t=0;
int lect[16];//datos de los esclavos
char data_ser_in[48],data_ser_out[15]; //cadenas con info Serial I/O
char formato[]={"%d#%d;%d#%d"};

// Direcciones de modulos 
uint8_t broadcastAddress0[] = {0xAC, 0x67, 0xB2, 0x3B, 0xE0, 0x78}; //direccion selector 
uint8_t broadcastAddress1[] = {0xAC, 0x67, 0xB2, 0x3B, 0xDE, 0xF4}; //direccion modulo 1 
uint8_t broadcastAddress2[] = {0xAC, 0x67, 0xB2, 0x3C, 0x02, 0x30}; //direccion modulo 2
uint8_t broadcastAddress3[] = {0xAC, 0x67, 0xB2, 0x3B, 0xC5, 0x00}; //direccion modulo 3  
uint8_t broadcastAddress4[] = {0xAC, 0x67, 0xB2, 0x3B, 0xE3, 0x80}; //direccion modulo 4
uint8_t broadcastAddress5[] = {0xAC, 0x67, 0xB2, 0x3B, 0xFD, 0x00}; //direccion modulo 5
uint8_t broadcastAddress6[] = {0xAC, 0x67, 0xB2, 0x3C, 0x21, 0x2C}; //direccion modulo 6
uint8_t broadcastAddress7[] = {0xAC, 0x67, 0xB2, 0x3B, 0xFF, 0x50}; //direccion modulo 7
uint8_t broadcastAddress8[] = {0xAC, 0x67, 0xB2, 0x3B, 0xE0, 0x1C}; //direccion modulo 8
uint8_t direccion[6];


// Variable que almacena si los datos fueron enviados correctamente
String success;

//Formato de estructura para datos transmitidos
typedef struct struct_mensaje {
    uint8_t id_mod;
    uint8_t selec;
    uint16_t lectura_1;
    uint16_t lectura_2;
} struct_mensaje;

//Se crea estructura para los modulos esclavos
struct_mensaje mod1,mod2,mod3,mod4,mod5,mod6,mod7,mod8;

// Se crea estructura para los mensajes a enviar
struct_mensaje enviar;

//estructura que recibe los datos desde la funcion
struct_mensaje myData;

// Callback de cuando se envian datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
 /* Serial.print("\r\nEstado ultimo envio:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Entrega satisfactoria" : "Entrega fallida");
  if (status ==0){
    success = "Entrega satisfactoria :)";
  }
  else{
    success = "Entrega fallida  :(";
  }*/
} 

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  mod_receptor();
  /*Serial.print(myData.id_mod);Serial.print(";");
  Serial.print(myData.lectura_1);Serial.print(";");
  Serial.print(myData.lectura_2);Serial.println(";");*/
  
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2); //Serial 2
  
  // se configura el dispositivo como Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // iniciar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Una vez iniciado ESPNOW exitosamente, we will register for Send CB to
  // obtiener el estado de los paquetes enviados
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

/*
  //registro de modulo selector
  memcpy(peerInfo.peer_addr, broadcastAddress0, 6);      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }*/

  //registro de modulo 1
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  //registro de modulo 2
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
   //registro de modulo 3
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
   //registro de modulo 4
  memcpy(peerInfo.peer_addr, broadcastAddress4, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } 

   //registro de modulo 5
  memcpy(peerInfo.peer_addr, broadcastAddress5, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

   //registro de modulo 6
  memcpy(peerInfo.peer_addr, broadcastAddress6, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

   //registro de modulo 7
  memcpy(peerInfo.peer_addr, broadcastAddress7, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
 
   //registro de modulo 8
  memcpy(peerInfo.peer_addr, broadcastAddress8, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}



void loop() {
  
static unsigned long lastWrite= 0;
static unsigned long lastRead= 0;


mod_count++;
  if(mod_count>8)
    mod_count=1;


//*********************************************************
//------------------ESP-NOW----------------------------------
enviar.id_mod=id; //id modulo
enviar.selec=mod_count;
mod_enviar();
while((abs(millis()-lastWrite))<20){}
lastWrite=millis();
  
//-------------------Fin ESP-NOW--------------------------
//*********************************************************

//*********************************************************************
//--------------------------Envio Serial------------------------------- 

/*
sprintf(data_ser_out,formato,lect[0],lect[1],lect[2],lect[3],lect[4],
lect[5],lect[6],lect[7],lect[8],lect[9],lect[10],lect[11],lect[12],
lect[13],lect[14],lect[15]); */
//Serial.write(data_ser_out); //vector mandado por serial
//------------------------Fin Envio Serial-----------------------------
//*********************************************************************


}//fin void loop

//------------------------------------------------------
void mod_receptor(){ //filtro de a que modulo
                     //corresponde los datos recibidos
   if(myData.id_mod==1){
    lect[0]= myData.lectura_1;
    lect[1]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[0],myData.id_mod,lect[1]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[0]);Serial.print(";");
    Serial.print(lect[1]);Serial.println(";");*/
   }
   if(myData.id_mod==2){
    lect[2]= myData.lectura_1;
    lect[3]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[2],myData.id_mod,lect[3]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[2]);Serial.print(";");
    Serial.print(lect[3]);Serial.println(";");*/
   }
   if(myData.id_mod==3){
    lect[4]= myData.lectura_1;
    lect[5]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[4],myData.id_mod,lect[5]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[4]);Serial.print(";");
    Serial.print(lect[5]);Serial.println(";");*/
   }
   if(myData.id_mod==4){
    lect[6]= myData.lectura_1;
    lect[7]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[6],myData.id_mod,lect[7]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[6]);Serial.print(";");
    Serial.print(lect[7]);Serial.println(";");*/
   }
   if(myData.id_mod==5){
    lect[8]= myData.lectura_1;
    lect[9]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[8],myData.id_mod,lect[9]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[8]);Serial.print(";");
    Serial.print(lect[9]);Serial.println(";");*/
   }
   if(myData.id_mod==6){
    lect[10]= myData.lectura_1;
    lect[11]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[10],myData.id_mod,lect[11]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[10]);Serial.print(";");
    Serial.print(lect[11]);Serial.println(";");*/
   }
   if(myData.id_mod==7){
    lect[12]= myData.lectura_1;
    lect[13]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[12],myData.id_mod,lect[13]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[12]);Serial.print(";");
    Serial.print(lect[13]);Serial.println(";");*/
   }
   if(myData.id_mod==8){
    lect[14]= myData.lectura_1;
    lect[15]= myData.lectura_2;
    sprintf(data_ser_out,formato,myData.id_mod,
    lect[14],myData.id_mod,lect[15]);
    Serial2.println(data_ser_out);
    /*Serial.print(myData.id_mod);Serial.print(";");
    Serial.print(lect[14]);Serial.print(";");
    Serial.print(lect[15]);Serial.println(";");*/
   }        
  }
//------------------------------------------------------

void mod_enviar(){  //se envian los datos al resto de modulos


 esp_err_t result1 = esp_now_send(broadcastAddress1, (uint8_t *) &enviar, sizeof(enviar));
 esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *) &enviar, sizeof(enviar));
 esp_err_t result3 = esp_now_send(broadcastAddress3, (uint8_t *) &enviar, sizeof(enviar));
 esp_err_t result4 = esp_now_send(broadcastAddress4, (uint8_t *) &enviar, sizeof(enviar));   
 esp_err_t result5 = esp_now_send(broadcastAddress5, (uint8_t *) &enviar, sizeof(enviar));
 esp_err_t result6 = esp_now_send(broadcastAddress6, (uint8_t *) &enviar, sizeof(enviar));
 esp_err_t result7 = esp_now_send(broadcastAddress7, (uint8_t *) &enviar, sizeof(enviar));
 esp_err_t result8 = esp_now_send(broadcastAddress8, (uint8_t *) &enviar, sizeof(enviar));
    
  }

//----------------------------------------------------------------------
