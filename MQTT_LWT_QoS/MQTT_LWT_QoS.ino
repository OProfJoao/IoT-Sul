#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoJson.h>

// Configurações de rede WiFi
const char* SSID = "iPhone";
const char* PSWD = "123456789";

// Configurações do Broker MQTT
const char* brokerUrl = "test.mosquitto.org";  // URL do Broker MQTT
const int port = 1883;                          // Porta do Broker MQTT

// Configurações do LWT (Last Will and Testament) MQTT
const char* Topic_LWT = "Aula/Boards/PlacaTOP/Status";
String Msg_LWT = "";
const byte QoS_LWT = 1;
const bool Retain_LWT = true;

// Instâncias dos objetos
WiFiClient espClient;           // Cliente WiFi
PubSubClient mqttClient(espClient);  // Cliente MQTT
WiFiUDP ntpUDP;                 // Conexão para NTP (Time Protocol)
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600, 60000);  // Cliente NTP

void connectToWifi();            // Conectar à rede WiFi
void connectToBroker();          // Conectar ao Broker MQTT
void printStatus(String status); // Função para exibir status (ainda não implementada)

void setup() {
  Serial.begin(115200);
  connectToWifi();     // Conectar à rede WiFi
  connectToBroker();   // Conectar ao Broker MQTT
  timeClient.begin();  // Inicializar o cliente NTP
  timeClient.setTimeOffset(13500);  // Ajuste do fuso horário (UTC +3:30)
}

void loop() {
  Serial.println(timeClient.getFormattedTime());  // Imprimir o horário atual (do NTP)

  // Verificar se a conexão WiFi está ativa
  if (WiFi.status() != WL_CONNECTED) {
    printStatus(String("Offline"));
    connectToWifi();  // Reconectar à WiFi se perdido
  }

  // Verificar se a conexão MQTT está ativa
  if (!mqttClient.connected()) {
    printStatus(String("Offline"));
    connectToBroker();  // Reconectar ao Broker MQTT se perdido
  }

  mqttClient.loop();
  delay(1000);  // Espera de 1 segundo entre as verificações
}

void connectToBroker() {
  StaticJsonDocument<200> doc;  
  doc["Status"] = "Offline";
  serializeJson(doc, Msg_LWT);

  mqttClient.setServer(brokerUrl, port);  // Definir o servidor do Broker

  String userId = "ESP-JOAO" + String(random(0xffff), HEX);  // ID aleatório para o cliente

  while (!mqttClient.connected()) {
    mqttClient.connect(userId.c_str(), "", "", Topic_LWT, QoS_LWT, Retain_LWT, Msg_LWT.c_str());
    delay(2000);
  }

  mqttClient.subscribe(Topic_LWT, QoS_LWT);
  doc["Status"] = "Online";
  serializeJson(doc, Msg_LWT);
  mqttClient.publish(Topic_LWT, Msg_LWT.c_str(), Retain_LWT);  // Publicar status "Online"
}

void callback(char* topic, byte* payload, unsigned int length) {
  if (length > 0) {
    String message = "";
    for (int i = 0; i < length; i++) {
      message += (char)payload[i];
    }
    if (String(topic) == Topic_LWT) {
      StaticJsonDocument<200> doc;
      deserializeJson(doc, message);
      printStatus(doc["Status"]);  // Atualizar o status
    }
  }
}

void connectToWifi() {
  WiFi.begin(SSID, PSWD);  // Conectar ao WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
  }
  Serial.println("Conectado à rede WiFi!");
}

void printStatus(String status) {
  Serial.println(status);  // Exibir o status na Serial Monitor
}
