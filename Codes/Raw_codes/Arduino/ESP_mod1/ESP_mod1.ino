#include <esp_now.h>
#include <WiFi.h>



int num=0,t=0;
int flag=2;

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress1[] = {0xAC, 0x67, 0xB2, 0x3B, 0xDE, 0xF4}; //direccion modulo 2 
uint8_t broadcastAddress2[] = {0xAC, 0x67, 0xB2, 0x3C, 0x02, 0x30}; //direccion modulo 3 

// Variable to store if sending data was successful
String success;


//Formato de estructura para datos transmitidos
typedef struct struct_message {
    float numero;
    float mod_selec;
    float mod_orig;
} struct_message;

//Se crea estructura para los mensajes recibidos
struct_message recibido;

// Se crea estructura para los mensajes a enviar
struct_message enviar;

//estructura que recibe los datos desde la funcion
struct_message myData;

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

  //registro de modulo 2
  memcpy(peerInfo.peer_addr, broadcastAddress1, 6);      
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  //registro de modulo 3
  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {


num++;
enviar.numero=num;
enviar.mod_orig=1;



    enviar.mod_selec=2;
    esp_err_t result = esp_now_send(broadcastAddress2, (uint8_t *) &enviar, sizeof(enviar));  


/*  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
*/

int num_recv=recibido.numero;
int mod=recibido.mod_orig;

  Serial.print("numero: ");
  Serial.print(num_recv);Serial.print(" ; ");
  Serial.print("modulo: ");
  Serial.print(mod);
  Serial.println();
 
delay(2000);
}
