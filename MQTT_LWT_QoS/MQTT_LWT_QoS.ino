//Placa 1
#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const String SSID = "iPhone";
const String PSWD = "123456789";
const String brokerUrl = "test.mosquitto.org";  //URL do broker   (servidor)
const int port = 1883;                          //Porta do Broker (servidor)

const char* Topic_LWT = "Aula/Boards/PlacaTOP/Status";
const int   QoS_LWT   = 1;
const bool  Retain_LWT= true;
const char* Msg_LWT   = "Offline"; 

WiFiClient espClient;                           //Criando Cliente WiFi
PubSubClient mqttClient(espClient);             //Criando Cliente MQTT
WiFiUDP ntpUDP;                                 //conexão ao servicor que retorna o horário
NTPClient timeClient(ntpUDP,"pool.ntp.org",3600,60000);

void connectToWifi();
void connectToBroker();

void setup() {
  Serial.begin(115200);
  connectToWifi();
  connectToBroker();
  timeClient.begin();
  timeClient.setTimeOffset(13500);
}

void loop() {
  Serial.println(timeClient.getFormattedTime());
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Conexão WiFi perdida!");
    connectToWifi();
  }
  if(!mqttClient.connected()){
    Serial.println("Conexão MQTT perdida!");
    connectToBroker();
  }
  delay(1000);
  mqttClient.loop();
}


void connectToBroker(){
  Serial.println("Conectando ao broker...");
  mqttClient.setServer(brokerUrl.c_str(),port);
  String userId = "ESP-JOAO";
  userId += String(random(0xffff), HEX);
  while(!mqttClient.connected()){

    mqttClient.connect(userId.c_str(),
                      "",
                      "",
                      Topic_LWT,
                      QoS_LWT,
                      Retain_LWT,
                      Msg_LWT);

    Serial.print(".");
    delay(2000);
  }
  mqttClient.publish(Topic_LWT, "Online",Retain_LWT);
  Serial.println("Conectado com sucesso!");
}

void connectToWifi(){
  Serial.println("Iniciando conexão com rede WiFi");
  WiFi.begin(SSID,PSWD);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(2000);
  }
  Serial.println("\nConectado!");
}




