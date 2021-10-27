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
char formato[]={"%d;%d"};
char bt_out[48],bt_in[12];
int id_mod=1;
int n_bt=0;
int lect[2],ref;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("MOD1"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
   Serial.print("BT MAC: ");
  printDeviceAddress();
  Serial.println();
}

void loop() {

static unsigned long lastRead = 0;

lect[0]=analogRead(34);
lect[1]=analogRead(35);

//****************************************************
//-----------Comunicacion Bluetooth-------------------

  if (SerialBT.available()>0){
    c=SerialBT.read();
    if(c=='A'){
      while((SerialBT.available()>0)&&(c!='B')){
        c=SerialBT.read();
        if(c!='B'){
          bt_in[n_bt]=c;
          n_bt++;
        }
        if(c=='B'){
          sprintf(bt_out,formato,lect[0],lect[1]);
          SerialBT.print(bt_out);
          ref=atoi(bt_in);
          Serial.print(millis()-lastRead);
          Serial.print(" ; ");
          Serial.println(ref);
          lastRead=millis();
          for(int j=0;j<n_bt;j++)
            bt_in[j]=NULL;
          n_bt=0;  
        }    
      }
    }else{
      SerialBT.print("F");
    }      
  }
//-----------Fin comunicacion Bluetooth----------------
//*****************************************************
  
}//fin void loop
