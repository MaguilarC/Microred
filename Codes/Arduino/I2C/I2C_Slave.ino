
//inclusion librerias
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>

//********************************************************************
//------------------ Variables globales -----------------------------

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 4  //direccion esclavo

//declaracion funciones I2C de esclavo
void requestEvent();
void receiveEvent(int howMany);

//declaracion variables globales
int n_mod=I2C_SLAVE_ADDR; //direccion esclavo
int n=0; //indice contador
uint16_t lectura_1=0,lectura_2=0,ref=0; //variables datos 
char formato[12]={"%d;%d;%d;n"};//formato Trama Virtual de salida
char data_i2c_out[10],data_i2c_in[10]; //cadenas con la info I2C I/O
int pin_led =16; //se asigna el pin 16 a la variable
int f; //variable con la referencia
char *indice; //puntero descomposicion

//------------------ Fin variables globales -------------------------
//********************************************************************


void setup(){
  
  Serial.begin(115200); // Se inicia y establece vel. Serial
   
  // Se inicia el I2C en modo esclavo y se establece la dir.
  bool res = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
  
  if (!res) { //si hubo un error en iniciar el I2C
    Serial.println("I2C slave init failed");
    while(1) delay(100);
  }
  //se inicia la tarea de recepcion de datos I2C
  WireSlave.onReceive(receiveEvent);
  //se inicia la tarea de respuesta de datos I2C 
  WireSlave.onRequest(requestEvent);
  
  Serial.printf("Esclavo I2C iniciado con la dir. #%d\n", I2C_SLAVE_ADDR);
  
  pinMode(pin_led,OUTPUT);//se establece el pin como salida
}

void loop(){

    //se realiza lecturas analog. desde los pines 34 y 35
    lectura_1=analogRead(34);
    lectura_2=analogRead(35);
 
    WireSlave.update();//se actualiza por si hay nuevos datos
    
    // una pausa para que los periféricos tengan tiempo de
    //acceso al procesador.
    delay(1);

    //se enciende o apaga el led segun la referencia
    if(f==1)
      digitalWrite(pin_led,HIGH);
    else
      digitalWrite(pin_led,LOW);        
    
}//fin void loop

//-----------------------------------------------------------
void requestEvent(){
    //se copia lecturas a un formato de Trama Virtual
    sprintf(data_i2c_out,formato,n_mod,lectura_1,lectura_2);
    //vector que se manda por I2C
    WireSlave.print(data_i2c_out);  
}
//-----------------------------------------------------------
void receiveEvent(int howMany){
  
  // Se recorre todos los bytes excepto el ultimo
  while (1 < WireSlave.available()){ 
        //se recibe un byte como caracter
        data_i2c_in[n] = WireSlave.read(); 
        n++;
  }
  
n=0; //reset contador indice

//Se descompone la Trama Virtual recibida
indice=strtok(data_i2c_in,";");
f=atoi(indice);

  for(int j=0;j<10;j++)//ciclo para limpiar cadena
    data_i2c_in[j]=NULL;  
}