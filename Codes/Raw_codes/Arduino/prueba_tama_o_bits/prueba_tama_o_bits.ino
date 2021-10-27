
uint16_t dato[2];
uint16_t cantidad=0;
uint8_t mac_addr[] = {0xAC, 0x67, 0xB2, 0x3B, 0xDE, 0xF4};
char mac_text[]={"ac:67:b2:3b:de:f4"};
char macStr[18];

void setup() {
  Serial.begin(115200);

}

void loop() {

snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  if(strcmp(mac_text,macStr)==0)
    Serial.println("Iguales");

Serial.println(sizeof(mac_addr));
}
