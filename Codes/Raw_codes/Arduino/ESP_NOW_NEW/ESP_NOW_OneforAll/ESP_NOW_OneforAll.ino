#include <esp_now.h>
#include <WiFi.h>

uint8_t mod_count=1,i=0,j=0;
uint16_t reenvio=0,veces=0;
unsigned long t=0;

// Direcciones de modulos esclavos
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
  /*Serial.print("\r\nEstado ultimo envio:\t");
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
 
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

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


  //registro de modulo 1
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  /*
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
  }*/ 

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
 /*
   //registro de modulo 8
  memcpy(peerInfo.peer_addr, broadcastAddress8, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  } */ 
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  
static unsigned long lastRead= 0;

//*********************************************************
//------------------ESP-NOW----------------------------------
enviar.id_mod=8; //id modulo
enviar.lectura_1=analogRead(34);             
enviar.lectura_2=analogRead(35);  

mod_receptor();

if((myData.id_mod==7)||((abs(millis()-lastRead))>40)){ //de quien recibe el "pase"
  Serial.println((abs(millis()-lastRead)));
  esp_err_t result1 = esp_now_send(broadcastAddress1, (uint8_t *) &enviar, sizeof(enviar));   
  esp_err_t result5 = esp_now_send(broadcastAddress5, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result6 = esp_now_send(broadcastAddress6, (uint8_t *) &enviar, sizeof(enviar));
  esp_err_t result7 = esp_now_send(broadcastAddress7, (uint8_t *) &enviar, sizeof(enviar));
  //esp_err_t result8 = esp_now_send(broadcastAddress8, (uint8_t *) &enviar, sizeof(enviar));

lastRead=millis();
}

imprimir_Serial();
  
//-------------------Fin ESP-NOW--------------------------
//*********************************************************


}

//------------------------------------------------------
void mod_receptor(){ //filtro de a que modulo
                     //corresponde los datos recibidos
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

void mod_enviar(){  //obtencion de direcion mac del modulo 
                    //donde se mandaran los datos
  if(mod_count==1)
   memcpy(direccion, broadcastAddress1, 6); 
  if(mod_count==2)
    memcpy(direccion, broadcastAddress2, 6); 
  if(mod_count==3)
   memcpy(direccion, broadcastAddress3, 6);   
  if(mod_count==4)
    memcpy(direccion, broadcastAddress4, 6); 
  if(mod_count==5)
   memcpy(direccion, broadcastAddress5, 6);   
  if(mod_count==6)
   memcpy(direccion, broadcastAddress6, 6); 
  if(mod_count==7)
   memcpy(direccion, broadcastAddress7, 6); 
  if(mod_count==8)
   memcpy(direccion, broadcastAddress8, 6);  
  }
//-----------------------------------------------------------------
void imprimir_Serial(){

Serial.print("Modulo 1: ");
Serial.print("lectura 1= ");Serial.print(mod1.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod1.lectura_2);Serial.print(" ; ");
Serial.println(" ");
Serial.print("Modulo 2: ");
Serial.print("lectura 1= ");Serial.print(mod2.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod2.lectura_2);Serial.print(" ; ");
Serial.println(" ");
Serial.print("Modulo 3: ");
Serial.print("lectura 1= ");Serial.print(mod3.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod3.lectura_2);Serial.print(" ; ");
Serial.println(" ");
Serial.print("Modulo 4: ");
Serial.print("lectura 1= ");Serial.print(mod4.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod4.lectura_2);Serial.print(" ; ");
Serial.println(" ");
Serial.print("Modulo 5: ");
Serial.print("lectura 1= ");Serial.print(mod5.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod5.lectura_2);Serial.print(" ; ");
Serial.println(" ");
Serial.print("Modulo 6: ");
Serial.print("lectura 1= ");Serial.print(mod6.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod6.lectura_2);Serial.print(" ; ");
Serial.println(" ");
Serial.print("Modulo 7: ");
Serial.print("lectura 1= ");Serial.print(mod7.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod7.lectura_2);Serial.print(" ; ");
Serial.println(" "); 
Serial.print("Modulo 8: ");
Serial.print("lectura 1= ");Serial.print(mod8.lectura_1);Serial.print(" ; ");
Serial.print("lectura 2= ");Serial.print(mod8.lectura_2);Serial.println(" ; ");
Serial.println("-------------------------- ");   
  }

//----------------------------------------------------------------------

void error_comunicacion(){

for(j=0;j<10;j++){
      Serial.print("******Modulo:");Serial.print(mod_count); 
      Serial.println(" no recibe datos correctamente******");
    }   
}

//-----------------------------------------------------------------  
