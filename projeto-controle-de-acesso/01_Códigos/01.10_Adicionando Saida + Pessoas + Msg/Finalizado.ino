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

const int pinoBotaoSaida = 32;

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

struct UsuarioRFID {
  String id;     // RFID ID (ex: "04:AD:6A:A7")
  String nome;   // Nome do usuário
  String senha;  // Senha desse usuário
};

UsuarioRFID usuarios[] = {
  {"04:AD:6A:A7", "Matheus", "1234"},
  {"E3:1A:21:21", "Sandro", "4321"},
  {"J3:1A:21:21", "Pablo", "5678"},
  {"A3:1A:21:21", "Patricia", "9876"}
};

const int qtdUsuarios = sizeof(usuarios)/sizeof(usuarios[0]);

String senhaDigitada = "";

bool blynkHabilitado = false;

void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, pass);
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 10) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado.");
    Blynk.config(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
    if (Blynk.connect(1000)) {
      Serial.println("Blynk conectado.");
      blynkHabilitado = true;
    } else {
      Serial.println("Blynk não conectado.");
    }
  } else {
    Serial.println("\nFalha ao conectar ao WiFi.");
  }

  SPI.begin();
  rfid.PCD_Init();

  pinMode(pinoRele, OUTPUT);
  digitalWrite(pinoRele, HIGH);

  pinMode(pinoBuzzer, OUTPUT);
  digitalWrite(pinoBuzzer, LOW);

  pinMode(pinoBotaoSaida, INPUT_PULLUP);

  Wire.begin(21, 22);
  lcd.begin(20, 4);
  lcd.backlight();
  lcd.clear();
  resetDisplay();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && Blynk.connected()) {
    Blynk.run();
  }

  leituraRfid();
  leituraTeclado();

  if (digitalRead(pinoBotaoSaida) == LOW) {
    acessoPermitido("Saida liberada", "");

    if (blynkHabilitado) {
      Blynk.logEvent("acesso_autorizado", "Saída liberada por botão interno");
    }
    delay(1000);
  }
}

void leituraRfid() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] < 0x10) strID += "0";
    strID += String(rfid.uid.uidByte[i], HEX);
    if (i != 3) strID += ":";
  }
  strID.toUpperCase();

  Serial.print("ID do cartão: ");
  Serial.println(strID);

  String nomeAchado = "";
  for (int i = 0; i < qtdUsuarios; i++) {
    if (strID == usuarios[i].id) {
      nomeAchado = usuarios[i].nome;
      break;
    }
  }

  if (nomeAchado != "") {
    acessoPermitido("Bem-vindo,", "Sr(a) " + nomeAchado);
    if (blynkHabilitado) {
      Blynk.logEvent("acesso_autorizado", "Cartão autorizado: " + strID);
    }
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
    if (blynkHabilitado) {
      Blynk.logEvent("acesso_autorizado", "Trava aberta via app");
    }
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
      String nomeAchado = "";
      for (int i = 0; i < qtdUsuarios; i++) {
        if (senhaDigitada == usuarios[i].senha) {
          nomeAchado = usuarios[i].nome;
          break;
        }
      }

      if (nomeAchado != "") {
        acessoPermitido("Bem-vindo,", "Sr(a) " + nomeAchado);
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

void acessoPermitido(String linha1, String linha2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linha1);
  lcd.setCursor(0, 1);
  lcd.print(linha2);
  
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