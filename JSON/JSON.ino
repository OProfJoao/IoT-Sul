#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
 int sensor = 100;
 String mensagem = "";

 JsonDocument doc;

 doc["ValorSensor"] = sensor;
 doc["ValorSensor2"] = sensor;
 serializeJson(doc, mensagem);
 Serial.println(mensagem);

  JsonDocument doc;
  String JSON_Message = "";
  deserializeJson(doc, json);
  int val1 = doc["ValorSensor"];
  int val2 = doc["ValorSensor2"];


 delay(1000);
}
