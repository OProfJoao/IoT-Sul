#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_W 128
#define SCREEN_H 64
#define RESET_PIN -1
#define SCREEN_ADDRS 0x3C

#define SDA 8
#define SCL 9

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, RESET_PIN);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA,SCL);
  Serial.println("iniciando comunicação com a display");
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRS)){
    Serial.println("Conexão com a display falhou");
    for(;;);
  }
  Serial.println("Conectado!");
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  display.setCursor(10, 0);
  display.println(F("Texto"));
  display.display();      // Show initial text
  delay(1000);

  // Scroll in various directions, pausing in-between:
  // display.startscrollright(0x00, 0x0F);
}
