#include <WiFi.h> //se incluye la libreria Wifi
 
void setup(){
  Serial.begin(115200); //config. puerto Serial.
  WiFi.mode(WIFI_MODE_STA); //Wifi del modulo en Modo Estacion
}
 
void loop(){

  Serial.println(WiFi.macAddress()); //se imprime la MAC por Serial
  delay(5000);//se espera 5 sg.
