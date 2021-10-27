int lectura=3500;
int data0;
int data1;
int recompuesto;

void setup() {
  Serial.begin(115200);

}

void loop() {
data0= (int) 0x00FF & lectura;
data1= (int) 0x00FF & (lectura>>8);
recompuesto=(int) (data1 << 8 | data0);
Serial.print("n1: ");Serial.print(data0);
Serial.print(" n2: ");Serial.println(data1);
Serial.println(recompuesto);
 

}
