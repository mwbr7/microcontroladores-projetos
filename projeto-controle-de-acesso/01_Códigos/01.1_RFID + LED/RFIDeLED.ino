#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 5
#define RST_PIN 22
 
MFRC522 rfid(SS_PIN, RST_PIN);
 
 
const int pinoLedVerde = 13;
const int pinoLedVermelho = 4;
 
void setup(){
 
  Wire.begin();
  SPI.begin();
  rfid.PCD_Init();
  
  pinMode(pinoLedVerde, OUTPUT);
  pinMode(pinoLedVermelho, OUTPUT);
  
  digitalWrite(pinoLedVerde, LOW);
  digitalWrite(pinoLedVermelho, LOW);
}
 
void loop() {
  leituraRfid();
}
 
void leituraRfid(){
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;
 
  String strID = ""; 
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
 
  if (strID.indexOf("04:AD:6A:A7") >= 0) {
    digitalWrite(pinoLedVerde, HIGH);
    delay(3000);
    digitalWrite(pinoLedVerde, LOW);
  }else{
    digitalWrite(pinoLedVermelho, HIGH);
    delay(3000);
    digitalWrite(pinoLedVermelho, LOW);
  }
 
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  }