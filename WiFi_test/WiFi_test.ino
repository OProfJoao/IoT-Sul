#include <WiFi.h>



const String SSID = "iPhone";
const String PSWD = "123456789";

void scanLocalNetworks();
void connectToWifi();

void setup() {
  Serial.begin(115200);
  //scanLocalNetworks();
  connectToWifi();
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
  	Serial.println("Conexão perdida!");
  	connectToWifi();
  }
}

void scanLocalNetworks(){
  Serial.println("Iniciando Scan de redes Wi-Fi");
  int number = WiFi.scanNetworks();
  delay(500);
  if(number == -1){
    Serial.println("ERRO!. Não deu bom");
  }
  else{
    Serial.printf("Número de redes encontradas: %d\n", number);
    for(int net = 0; net < number; net++){
      Serial.printf("%d - %s | %d db\n", net, WiFi.SSID(net), WiFi.RSSI(net));
    }
  }
}

void connectToWifi(){
	Serial.println("Iniciando conexão com rede WiFi");
	WiFi.begin(SSID,PSWD);
    while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(200);
    }
    Serial.println("\nConectado!");
}