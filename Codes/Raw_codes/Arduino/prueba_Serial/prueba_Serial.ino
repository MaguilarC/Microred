int LDRPin = 0;
int valor;



int led = 16;

void setup () {
      Serial.begin(9600);
      pinMode( led, OUTPUT );
}

void loop () {
      if(Serial.available()) {
            char c = Serial.read();
            if (c=='H') {
                  digitalWrite( led, HIGH);
                  Serial.println("introdujo H");
                  
            } else if (c=='L') {
                  digitalWrite( led, LOW);
                  Serial.println("introdujo L");
            } 
      }
}
