# 🏊‍♂️ **Monitoramento de Piscina com ESP32 + MediaPipe**

Este projeto utiliza um **ESP32-CAM** para capturar imagens em tempo real e a biblioteca **MediaPipe** para detectar a pose humana. A principal funcionalidade é calcular a altura da pessoa (com base nas coordenadas da cabeça e dos pés) e disparar um alerta caso a pessoa seja detectada em uma posição de perigo ou anormal (por exemplo, caindo ou fora da piscina).

## 🚀 Funcionalidades

- **Captura de vídeo em tempo real**: O **ESP32-CAM** transmite vídeo ao vivo via HTTP para o script Python.
- **Detecção de pose humana**: Usando a biblioteca **MediaPipe**, a posição da pessoa na piscina é analisada em tempo real.
- **Cálculo de altura**: A altura da pessoa é calculada com base nos pontos-chave do corpo humano (cabeça e tornozelos).
- **Envio de alerta**: Caso a altura medida seja abaixo de um valor mínimo configurado, um alerta é enviado para o ESP32 (indicando que a pessoa pode estar em perigo ou em uma posição crítica).

## 🏊‍♀️ Como Funciona

1. O **ESP32-CAM** transmite um stream de vídeo ao vivo.
2. O código Python captura as imagens do stream, converte-as para o formato adequado e processa-as utilizando o **MediaPipe**.
3. A **MediaPipe** detecta pontos-chave do corpo, como a cabeça e os tornozelos.
4. A altura da pessoa é calculada com base na diferença de pixels entre esses dois pontos.
5. Se a altura calculada for inferior a um valor mínimo definido (por exemplo, se a pessoa cair ou se estiver submersa), o sistema envia um alerta HTTP para o ESP32, que pode, por exemplo, disparar um alarme sonoro ou notificar um responsável.

## 📸 Requisitos

Para rodar o projeto, você precisa dos seguintes componentes:

- **Python 3.x** instalado.
- **ESP32-CAM** configurado para transmitir vídeo via HTTP.
- **Bibliotecas Python** necessárias (ver abaixo).

## 🛠️ Como Rodar

### 1. Clonar o Repositório

Clone o repositório do projeto para sua máquina local:

```bash
git clone https://github.com/seu-usuario/monitoramento-piscina-esp32-mediapipe.git
cd monitoramento-piscina-esp32-mediapipe
