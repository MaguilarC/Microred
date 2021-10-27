
//librerias para usar el Bluetooth
#include "BluetoothSerial.h"
#include "esp_bt_device.h"

//habilitacion de bluetooth y comprobacion 
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//se establece objeto 'SerialBT'
BluetoothSerial SerialBT;

//funcion para imprimir MAC bluetooth por consola
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

//********************************************************************
//------------------ Variables globales -----------------------------

char c; //variable aux. caracteres
char formato[]={"%d;%d"}; //formato Trama Virtual de salida
char bt_out[48],bt_in[12]; //cadenas con info Serial I/O
int id_mod=1; //id de este ESP32
int n_bt=0; //contador aux. cadena
int lect[2],ref; //valores lecturas

//------------------ Fin variables globales -------------------------
//********************************************************************

void setup() {
  Serial.begin(115200); //se inicia y config. el puerto Serial
  SerialBT.begin("MOD1"); // Nombre dispositivo bluetooth
  Serial.println("El dispositivo a iniciado,
  ahora se puede emparejar con el bluetooth!");
   Serial.print("BT MAC: ");
  printDeviceAddress();
  Serial.println();
  
} //fin void setup

void loop() {
  
//se establece variable para registro del tiempo
static unsigned long lastRead = 0;

//lecturas analog. pines 34 y 35
lect[0]=analogRead(34);
lect[1]=analogRead(35);

//****************************************************
//-----------Comunicacion Bluetooth-------------------

  if (SerialBT.available()>0){ //si hay algo en el buffer bluetooth

    c=SerialBT.read(); // lee de a un caracter

    //si el carcater es del principio de la Trama Virtual
    if(c=='A'){ 

      //lee hasta el ultimo caracter de la Trama Virtual
      while((SerialBT.available()>0)&&(c!='B')){
        
        c=SerialBT.read(); // lee de a un caracter
        
        //guarda los caracteres, excepto el ultimo
        if(c!='B'){ 
          bt_in[n_bt]=c;
          n_bt++;
        }
        
        //si se ha llegado al caracter final
        //se procede a enviar un mesaje a la Rasp. PI
        if(c=='B'){
          //--------------------- Envio bluetooth-------------------------
          //se copia lecturas a un formato de Trama Virtual
          sprintf(bt_out,formato,lect[0],lect[1]);
          //vector que se manda por bluetooth
          SerialBT.print(bt_out);
          //--------------------- Fin Envio bluetooth---------------------
          ref=atoi(bt_in); //se convierte a numero la cadena
          Serial.print(millis()-lastRead);//se imprime el tiempo por ciclo 
          Serial.print(" ; ");
          Serial.println(ref); //se imprime el numero recibido
          lastRead=millis(); //se hace registro de tiempo
          
          for(int j=0;j<n_bt;j++) //se limpia la cadena
            bt_in[j]=NULL;
            
          n_bt=0; //reset contador  
        }    
      }
     
    }else{ //si no se ha tomado bien la Trama Virtual
           //se manda un caracter a la Rasp. PI 
      SerialBT.print("F");
    }      
  }
//-----------Fin comunicacion Bluetooth----------------
//*****************************************************
  
}//fin void loop