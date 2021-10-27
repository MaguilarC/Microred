char texto[]={"a6000;b4000;c1000;d5000;"};
char num[10];
char *indice;
int ref[8];

void setup() {
  Serial.begin(9600); 

}

void loop() {

  indice=strtok(texto,";");strcpy(num,indice+1);ref[0]=atoi(num);
  indice=strtok(NULL,";");strcpy(num,indice+1);ref[1]=atoi(num);
  indice=strtok(NULL,";");strcpy(num,indice+1);ref[2]=atoi(num);
  indice=strtok(NULL,";");strcpy(num,indice+1);ref[3]=atoi(num);
  for(int i=0;i<3;i++)
    Serial.println(ref[i]);
    
}
