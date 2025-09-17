#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Controle de Acesso"
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "";
char pass[] = "";

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
}

void loop() {
  Blynk.run();
}