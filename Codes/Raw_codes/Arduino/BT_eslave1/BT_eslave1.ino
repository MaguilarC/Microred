//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include "esp_bt_device.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void printDeviceAddress() {
 
  const uint8_t* point = esp_bt_dev_get_address();
 
  for (int i = 0; i < 6; i++) {
 
    char str[3];
 
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
 
    if (i < 5){
      Serial.print(":");
    }
 
  }
}

uint8_t num=128;
uint8_t num2=23;
char c;
char formato[]={"hola desde modulo %d"};
char bt_out[48];
int id_mod=7;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("MOD7"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
   Serial.print("BT MAC: ");
  printDeviceAddress();
  Serial.println();
}

void loop() {



  if (SerialBT.available()) {
    c=SerialBT.read();
    if(c=='g'){
      sprintf(bt_out,formato,id_mod);
      SerialBT.print(bt_out);
    }  
  }
  delay(20);
}
