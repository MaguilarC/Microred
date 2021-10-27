
//librerias para uso ESP-NOW
#include <esp_now.h>
#include <WiFi.h>

//********************************************************************
//------------------ Variables globales -----------------------------

uint8_t id=1; //id modulo ESP32

// Direcciones de modulos ESP32 de la red 
uint8_t broadcastAddress0[] = {0xAC, 0x67, 0xB2, 0x3B, 0xE0, 0x78}; //direccion arbitro
uint8_t broadcastAddress1[] = {0xAC, 0x67, 0xB2, 0x3B, 0xDE, 0xF4}; //direccion modulo 1 
uint8_t broadcastAddress2[] = {0xAC, 0x67, 0xB2, 0x3C, 0x02, 0x30}; //direccion modulo 2
uint8_t broadcastAddress3[] = {0xAC, 0x67, 0xB2, 0x3B, 0xC5, 0x00}; //direccion modulo 3  
uint8_t broadcastAddress4[] = {0xAC, 0x67, 0xB2, 0x3B, 0xE3, 0x80}; //direccion modulo 4
uint8_t broadcastAddress5[] = {0xAC, 0x67, 0xB2, 0x3B, 0xFD, 0x00}; //direccion modulo 5
uint8_t broadcastAddress6[] = {0xAC, 0x67, 0xB2, 0x3C, 0x21, 0x2C}; //direccion modulo 6
uint8_t broadcastAddress7[] = {0xAC, 0x67, 0xB2, 0x3B, 0xFF, 0x50}; //direccion modulo 7
uint8_t broadcastAddress8[] = {0xAC, 0x67, 0xB2, 0x3B, 0xE0, 0x1C}; //direccion modulo 8


// Variable que almacena si los datos fueron enviados correctamente
String success;

//Formato de estructura para mensaje transmitido
typedef struct struct_mensaje {
    uint8_t id_mod; //ID de este ESP32
    uint8_t selec;  //ID del modulo selecionado por el arbitro
    uint16_t lectura_1; //lectura analogica 1
    uint16_t lectura_2; //lectura analogica 2
} struct_mensaje;

//Se crea estructura para los modulos ESP32
struct_mensaje mod1,mod2,mod3,mod4,mod5,mod6,mod7,mod8;

// Se crea estructura para los mensajes a enviar
struct_mensaje enviar;

//estructura que recibe los datos desde la funcion
struct_mensaje myData;

//------------------ Fin variables globales -------------------------
//********************************************************************


// Callback de cuando se envian datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { 
  /*Serial.print("\r\nEstado ultimo envio:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Entrega satisfactoria" : "Entrega fallida");
  if (status ==0){
    success = "Entrega satisfactoria :)";
  }
  else{
    success = "Entrega fallida  :(";
  }*/
} 

//funcion de la tarea de recepcion de datos

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  //se copia el mensaje a la estructura
  memcpy(&myData, incomingData, sizeof(myData));

  //si en el mensaje viene el id de este ESP32
  if(myData.selec==id){ 
    mod_enviar(); //funcion que envia en modo broadcast
  }
  
  //si el mensaje no es del arbitro
  if(myData.id_mod!=0){
    mod_receptor(); //funcion que guarda datos segun el modulo ESP32
   
   //se imprime por consola las lecturas
   /*Serial.print(myData.id_mod);Serial.print(";");
   Serial.print(myData.lectura_1);Serial.print(";");
   Serial.print(myData.lectura_2);Serial.println(";");*/
  }
}
 
void setup() {
  
  Serial.begin(115200); //se inicia y config. el puerto Serial

  // se configura el dispositivo como Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // iniciar ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Se registra una funcion para obtener el
  // estado de los paquetes enviados
  esp_now_register_send_cb(OnDataSent);
  
  // Configuracion del emparejamiento
  esp_now_peer_info_t peerInfo;
  peerInfo.channel = 0; //canal de comunicacion
  peerInfo.encrypt = false; //encriptacion


  //emparejamiento de modulo arbitro
  memcpy(peerInfo.peer_addr, broadcastAddress0, 6);      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
/*
  //emparejamiento de modulo 1
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }*/

  //emparejamiento de modulo 2
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

   //emparejamiento de modulo 3
  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }


   //emparejamiento de modulo 4
  memcpy(peerInfo.peer_addr, broadcastAddress4, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

   //emparejamiento de modulo 5
  memcpy(peerInfo.peer_addr, broadcastAddress5, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

   //emparejamiento de modulo 6
  memcpy(peerInfo.peer_addr, broadcastAddress6, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

   //emparejamiento de modulo 7
  memcpy(peerInfo.peer_addr, broadcastAddress7, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

   //emparejamiento de modulo 8
  memcpy(peerInfo.peer_addr, broadcastAddress8, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Registro de una función de devolución de llamada
  // que se llamará cuando se reciban los datos
  esp_now_register_recv_cb(OnDataRecv);
  
}//fin void setup

 
void loop() {

//se establece variable para registro del tiempo
static unsigned long lastRead= 0;

//*********************************************************
//------------------ESP-NOW----------------------------------
enviar.id_mod=id; //id modulo
//lecturas analog. desde los pines 34 y 35
enviar.lectura_1=analogRead(34);             
enviar.lectura_2=analogRead(35);  
  
//-------------------Fin ESP-NOW--------------------------
//*********************************************************

} //fin void loop

//------------------------------------------------------
//funcion que guarda datos segun el modulo ESP32
void mod_receptor(){ 
   if(myData.id_mod==1)
    mod1= myData;
   if(myData.id_mod==2)
    mod2= myData;
   if(myData.id_mod==3)
    mod3= myData;
   if(myData.id_mod==4)
    mod4= myData;
   if(myData.id_mod==5)
    mod5= myData;
   if(myData.id_mod==6)
    mod6= myData;
   if(myData.id_mod==7)
    mod7= myData;
   if(myData.id_mod==8)
    mod8= myData;    
  }
//------------------------------------------------------

//funcion para mandar el mensaje en broadcast
void mod_enviar(){  
  esp_err_t result0 = esp_now_send(broadcastAddress0, (uint8_t *) &enviar, sizeof(enviar));
  //se manda a todos los modulos, excepto a este
  //esp_err_t result1 = esp_now_send(broadcastAddress1, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result2 = esp_now_send(broadcastAddress2, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result3 = esp_now_send(broadcastAddress3, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result4 = esp_now_send(broadcastAddress4, (uint8_t *) &enviar, sizeof(enviar));   
  esp_err_t result5 = esp_now_send(broadcastAddress5, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result6 = esp_now_send(broadcastAddress6, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result7 = esp_now_send(broadcastAddress7, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result8 = esp_now_send(broadcastAddress8, (uint8_t *) &enviar, sizeof(enviar));
    
  }
//-----------------------------------------------------------------