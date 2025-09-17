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
#include <Keypad.h>

char ssid[] = "";
char pass[] = "";

#define SS_PIN 5
#define RST_PIN 16
MFRC522 rfid(SS_PIN, RST_PIN);

const int pinoRele = 17;

const int pinoBuzzer = 4;

LiquidCrystal_I2C lcd(0x27, 20, 4);

const byte LINHAS = 4;
const byte COLUNAS = 3;
char matriz_teclas[LINHAS][COLUNAS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte PinosLinhas[LINHAS] = {12, 33, 25, 27};
byte PinosColunas[COLUNAS] = {14, 13, 26};
Keypad meuteclado = Keypad(makeKeymap(matriz_teclas), PinosLinhas, PinosColunas, LINHAS, COLUNAS);

// Senha
String senhaCorreta = "1234";
String senhaDigitada = "";

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, HIGH);

  pinMode(pinoBuzzer, OUTPUT);
  digitalWrite(pinoBuzzer, LOW);

  Wire.begin(21, 22);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aproxime o cartao");
  lcd.setCursor(0, 1);
  lcd.print("use o app ou digite");
  lcd.setCursor(0, 2);
  lcd.print("a senha no teclado");
}

void loop() {
  Blynk.run();
  leituraRfid();
  leituraTeclado();
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
    acessoPermitido("Acesso liberado");
    Blynk.logEvent("acesso_autorizado", "Cartão autorizado: " + strID);
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Cartao invalido");
    buzzerLongo();
    delay(2000);
    resetDisplay();
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

BLYNK_WRITE(V17) {
  int estado = param.asInt();
  if (estado == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acesso via app");
    digitalWrite(pinoRele, LOW);
    buzzerCurto();
    Blynk.logEvent("acesso_autorizado", "Trava aberta via app");
  } else {
    digitalWrite(pinoRele, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Trava fechada");
    delay(2000);
    resetDisplay();
  }
}

void leituraTeclado() {
  char tecla = meuteclado.getKey();
  if (tecla) {
    Serial.print("Tecla: ");
    Serial.println(tecla);
    
    if (tecla == '#') {
      if (senhaDigitada == senhaCorreta) {
        acessoPermitido("Acesso liberado");
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Senha incorreta!");
        buzzerLongo();
        delay(2000);
        resetDisplay();
      }
      senhaDigitada = "";
    } else if (tecla == '*') {
      senhaDigitada = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha apagada");
      delay(1000);
      resetDisplay();
    } else {
      senhaDigitada += tecla;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Senha: ");
      for (int i = 0; i < senhaDigitada.length(); i++) lcd.print("*");
    }
  }
}

void acessoPermitido(String mensagem) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(mensagem);
  buzzerCurto();
  digitalWrite(pinoRele, LOW);
  delay(5000); 
  digitalWrite(pinoRele, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Trava fechada");
  delay(2000);

  resetDisplay();
}

void resetDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Aproxime o cartao");
  lcd.setCursor(0, 1);
  lcd.print("use o app ou digite");
  lcd.setCursor(0, 2);
  lcd.print("a senha no teclado");
}

void buzzerCurto() {
  digitalWrite(pinoBuzzer, HIGH);
  delay(100);
  digitalWrite(pinoBuzzer, LOW);
}

void buzzerLongo() {
  digitalWrite(pinoBuzzer, HIGH);
  delay(500);
  digitalWrite(pinoBuzzer, LOW);
}
