
String vector,a,b,c, mod=String(0);
char cadena[40],buf[40];
char *indice;
int num1=1,num2=1,num3;
int n=0;
int f=300;
char form[]={";1#%d;2#%d;3#%d;4#%d;5#%d;6#%d;7#%d;8#%d;9#%d;10#%d;11#%d;12#%d;13#%d;14#%d;15#%d;16#%d"};

void setup () {
      Serial.begin(9600);
}

void loop () {

  
//vector= mod+','+String(num1)+','+30+',';
sprintf(buf,form,f+1,f+2,f,f,f,f,f,f,f,f,f,f,f,f,f,f+16);
  Serial.println(buf);
if(Serial.available()>0){
    while(Serial.available()>0){
      cadena[n]= Serial.read();
      n++;
    }
  n=0;

  indice=strtok(cadena,";");
  num1=atoi(indice);

  indice=strtok(NULL,";");
  num2=atoi(indice);

  indice=strtok(NULL,";");
  num3=atoi(indice);

}

}
