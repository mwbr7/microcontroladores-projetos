///////////////////////////////////////////////// CÓDIGO UNIFICADO //////////////////////////////////////////////////

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Controle de Acesso"
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>

char ssid[] = "";
char pass[] = "";

// Definições RFID
#define SS_PIN 5
#define RST_PIN 22
MFRC522 rfid(SS_PIN, RST_PIN);

// LEDs
const int pinoLedVerde = 13;
const int pinoLedVermelho = 4;

void setup() {
  Serial.begin(9600);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);

  SPI.begin();
  rfid.PCD_Init();

  pinMode(pinoLedVerde, OUTPUT);
  pinMode(pinoLedVermelho, OUTPUT);
  digitalWrite(pinoLedVerde, LOW);
  digitalWrite(pinoLedVermelho, LOW);
}

void loop() {
  Blynk.run();        // Mantém a conexão com o Blynk
  leituraRfid();      // Executa a leitura do RFID
}

void leituraRfid() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    strID += String(rfid.uid.uidByte[i], HEX);
    if (i != 3) strID += ":";
  }
  strID.toUpperCase();

  Serial.print("ID do cartão: ");
  Serial.println(strID);

  if (strID.indexOf("04:AD:6A:A7") >= 0) {
    digitalWrite(pinoLedVerde, HIGH);
    Blynk.logEvent("acesso_autorizado", "Cartão autorizado: " + strID);
    delay(3000);
    digitalWrite(pinoLedVerde, LOW);
  } else {
    digitalWrite(pinoLedVermelho, HIGH);
    Blynk.logEvent("acesso_negado", "Cartão não autorizado: " + strID);
    delay(3000);
    digitalWrite(pinoLedVermelho, LOW);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
