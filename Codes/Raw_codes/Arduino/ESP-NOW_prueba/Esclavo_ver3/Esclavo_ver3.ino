#include <esp_now.h>
#include <WiFi.h>

//variables globales


// Direcciones de modulos esclavos
uint8_t broadcastAddress1[] = {0xAC, 0x67, 0xB2, 0x3B, 0xC8, 0xEC}; //direccion de maestro 

// Variable que almacena si los datos fueron enviados correctamente
String success;

//Formato de estructura para datos transmitidos
typedef struct struct_mensaje {
    uint8_t id_mod;
    uint8_t activacion;
    uint8_t retorno;
    uint16_t lectura_1;
    uint16_t lectura_2;
} struct_mensaje;

//estructura de mensaje recibido
struct_mensaje recibido;

// Se crea estructura para los mensajes a enviar
struct_mensaje enviar;

//estructura que recibe los datos desde la funcion
struct_mensaje myData;

// Callback de cuando se envian datos
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  /*Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Entrega satisfactoria" : "Entrega fallida");
  if (status ==0){
    success = "Entrega satisfactoria :)";
  }
  else{
    success = "Entrega fallida  :(";
  } */
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  recibido= myData;
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

  //registro y emparejado de maestro
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

    
//ejecucion hasta recibir habilitacion de maestro
    enviar.lectura_1= analogRead(34);
    enviar.lectura_2= analogRead(35);
    enviar.id_mod= 7;             //id modulo esclavo

  //cuerpo codigo de control
  //-
  //- 
  //-  
     
 if(recibido.activacion==1){ //se envia los datos al maestro
  enviar.retorno=recibido.retorno;
  esp_err_t result = esp_now_send(broadcastAddress1, (uint8_t *) &enviar, sizeof(enviar));
  imprimir_Serial(); 
 }

 
}

//----------------------------------------------------------------------------
void imprimir_Serial(){
  Serial.print("Modulo: "); Serial.println(enviar.id_mod);
  Serial.print("Lectura 1: ");Serial.println(enviar.lectura_1);
  Serial.print("Lectura 2: ");Serial.println(enviar.lectura_2);
  Serial.print("Retorno: ");Serial.println(recibido.retorno);
  }
