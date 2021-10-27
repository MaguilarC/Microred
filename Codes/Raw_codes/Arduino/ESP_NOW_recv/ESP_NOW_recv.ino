#include <esp_now.h>
#include <WiFi.h>


int num1=1;
int num2=100;
int num3=1000;

// REPLACE WITH THE MAC Address of your receiver 
//uint8_t broadcastAddress[] = {0xAC, 0x67, 0xB2, 0x3B, 0xDE, 0xF4}; //direccion modulo 2

// Variable to store if sending data was successful
String success;

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int numero1;
    float numero2;
    float numero3;
} struct_message;


struct_message mod1;
struct_message mod2;
struct_message mod3;
//struct_message modStruct[3]= {mod1, mod2, mod3};

// Create a struct_message called BME280Readings to hold sensor readings
struct_message recibido;

// Create a struct_message to hold incoming sensor readings
struct_message enviar;

struct_message myData;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  if(myData.numero1==1)
   mod1= myData;
   if(myData.numero1==2)
   mod2= myData;
   if(myData.numero1==3)
   mod3= myData;
  //recibido= myData;
  
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
/*
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
  */
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {

  Serial.print("numero 1: ");
  Serial.print(mod1.numero1);Serial.print(" ; ");
  Serial.print("numero 2: ");
  Serial.print(mod1.numero2);Serial.print(" ; ");
  Serial.print("numero 3: ");
  Serial.print(mod1.numero3);Serial.print(" ; ");
  Serial.println();
 
delay(5000);
}
