# Sistema de Controle de Acesso

Este é um sistema de controle de acesso baseado em ESP32 que pode ser operado de três maneiras: utilizando um cartão RFID, um teclado matricial ou um aplicativo Blynk. Através do uso de um servo motor (ou relé) e buzzer, o sistema abre ou fecha a trava de acordo com a autenticação do usuário.

## Funcionalidades

- **Autenticação via RFID:** Acesso liberado para um cartão específico.
- **Autenticação via Teclado:** O usuário pode inserir uma senha através de um teclado matricial.
- **Autenticação via Blynk:** O acesso pode ser controlado diretamente pelo aplicativo Blynk.
- **Feedback sonoro e visual:** O sistema fornece feedback através de um buzzer e um display LCD.

## Componentes Utilizados

- **ESP32**: Microcontrolador que gerencia a comunicação Wi-Fi e controla os dispositivos.
- **MFRC522**: Leitor de RFID para autenticação via cartão.
- **Teclado Matricial (4x3)**: Para inserção manual de senha.
- **Relé (ou Servo Motor)**: Para controlar a abertura e o fechamento da trava.
- **LCD 20x4**: Display para feedback visual ao usuário.
- **Buzzer**: Para alertas sonoros.

## Conexões

- **MFRC522 (Leitor RFID):**
  - SS_PIN (Chip Select) = GPIO 5
  - RST_PIN (Reset) = GPIO 16
- **LCD 20x4 (I2C):**
  - SDA = GPIO 21
  - SCL = GPIO 22
- **Teclado Matricial (4x3):**
  - Linhas conectadas a GPIOs 12, 33, 25, 27
  - Colunas conectadas a GPIOs 14, 13, 26
- **Relé (ou Servo Motor):**
  - Controlado via GPIO 17
- **Buzzer:**
  - Controlado via GPIO 4

## Bibliotecas Utilizadas

- **WiFi.h**: Para gerenciar a conexão Wi-Fi com a rede.
- **BlynkSimpleEsp32.h**: Biblioteca para integração com o aplicativo Blynk.
- **Wire.h**: Comunicação I2C para o LCD.
- **LiquidCrystal_I2C.h**: Biblioteca para controlar o LCD via I2C.
- **SPI.h**: Para comunicação SPI com o leitor RFID.
- **MFRC522.h**: Biblioteca para ler dados do cartão RFID.
- **Keypad.h**: Para gerenciar o teclado matricial.

## Configuração Inicial

1. **Configuração Wi-Fi:**
   No código, insira os dados de sua rede Wi-Fi nas variáveis `ssid` e `pass`:

   ```cpp
   char ssid[] = "SEU_SSID";
   char pass[] = "SUA_SENHA";

2. **Configuração do Blynk:**
   ```cpp
   #define BLYNK_AUTH_TOKEN "SEU_AUTH_TOKEN"
   
3. **Cartão RFID:**
   O código está configurado para permitir o acesso com um cartão com o ID "04:AD:6A:A7". Para adicionar novos cartões,   basta alterar o código conforme a necessidade, modificando a verificação if (strID.indexOf("04:AD:6A:A7") >= 0).

***Uso***
- Acesso via RFID: Aproximar o cartão RFID autorizado para liberar o acesso.
- Acesso via Teclado: Digite a senha correta no teclado matricial. Pressione # para confirmar e * para apagar a senha digitada.
- Acesso via Blynk: Controle a trava diretamente pelo aplicativo Blynk, abrindo ou fechando a trava.


