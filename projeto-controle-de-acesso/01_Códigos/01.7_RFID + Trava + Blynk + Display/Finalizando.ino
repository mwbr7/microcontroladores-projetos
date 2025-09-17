#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Trava"
#define BLYNK_AUTH_TOKEN ""
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

char ssid[] = "";
char pass[] = "";

#define SS_PIN 5
#define RST_PIN 16
MFRC522 rfid(SS_PIN, RST_PIN);

const int pinoRele = 17;

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);

  SPI.begin();
  rfid.PCD_Init();

  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, HIGH);

  Wire.begin(21, 22);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aproxime o cartao");
  lcd.setCursor(0, 1);
  lcd.print("ou use o app");

  Serial.println("Sistema iniciado.");
}

void loop() {
  Blynk.run();
  leituraRfid();
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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acesso liberado");
    digitalWrite(pinoRele, LOW);
    Blynk.logEvent("acesso_autorizado", "Cartão autorizado: " + strID);
    delay(3000);
    digitalWrite(pinoRele, HIGH);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cartao invalido");
    delay(2000);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aproxime o cartao");
  lcd.setCursor(0, 1);
  lcd.print("ou use o app");

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

BLYNK_WRITE(V17) {
  int estado = param.asInt();
  if (estado == 1) {
    digitalWrite(pinoRele, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acesso via app");
    delay(3000);
    digitalWrite(pinoRele, HIGH);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aproxime o cartao");
    lcd.setCursor(0, 1);
    lcd.print("ou use o app");
  }
}
