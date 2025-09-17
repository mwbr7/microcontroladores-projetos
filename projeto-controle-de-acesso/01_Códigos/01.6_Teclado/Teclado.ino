#include <Keypad.h>

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

void setup() {
  Serial.begin(9600);
  Serial.println("Teclado 4x3 - ESP32");
  Serial.println("Aguardando pressionamento das teclas...");
  Serial.println();
}

void loop() {
  char tecla_pressionada = meuteclado.getKey();

  if (tecla_pressionada) {
    Serial.print("Tecla pressionada: ");
    Serial.println(tecla_pressionada);
  }
}
