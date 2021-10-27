#include <esp_now.h>
#include <WiFi.h>



int num1=1;
int num2=100;
int num3=1000;

// REPLACE WITH THE MAC Address of your receiver 
uint8_t broadcastAddress[] = {0xAC, 0x67, 0xB2, 0x3B, 0xFD, 0x00}; //direccion modulo 1 (maestro)

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
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
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {


enviar.numero=recibido.numero;
enviar.mod_selec=0;
enviar.mod_orig=3;

if(recibido.mod_selec==2){
esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &enviar, sizeof(enviar));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

 
delay(5000);
}
