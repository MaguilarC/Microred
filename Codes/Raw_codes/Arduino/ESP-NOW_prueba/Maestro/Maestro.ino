#include <esp_now.h>
#include <WiFi.h>


// Direcciones de modulos esclavos
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

//Formato de estructura para datos transmitidos
typedef struct struct_mensaje {
    int id_mod;
    float lectura_1;
    float lectura_2;
} struct_mensaje;

//Se crea estructura para los modulos esclavos
struct_mensaje mod1,mod2,mod3,mod4,mod5,mod6,mod7,mod8;

//Arreglo de estructuras de modulos
//struct_mensaje modStruct[3]= {mod1, mod2, mod3};

// Se crea estructura para los mensajes a enviar
struct_mensaje enviar;

//estructura que recibe los datos desde la funcion
struct_mensaje myData;

// Callback de cuando se envian datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Entrega satisfactoria" : "Entrega fallida");
  if (status ==0){
    success = "Entrega satisfactoria :)";
  }
  else{
    success = "Entrega fallida  :(";
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  
 /* if(myData.id_mod==1)
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
   mod6= myData;*/
   if(myData.id_mod==7)
   mod7= myData;
   //if(myData.id_mod==8)
   //mod8= myData; 
/*
   modStruct[myData.id_mod-1].lectura_1=myData.lectura_1;
   modStruct[myData.id_mod-1].lectura_2=myData.lectura_2;
*/
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

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
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

delay(500);

}
