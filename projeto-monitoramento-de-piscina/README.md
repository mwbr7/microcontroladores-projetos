# 🎥 **Monitoramento de Altura com ESP32 + MediaPipe**

Este projeto integra um **ESP32-CAM** com a biblioteca **MediaPipe** para realizar o monitoramento de altura de uma pessoa em tempo real. Ele calcula a altura da pessoa com base nas coordenadas dos pontos-chave do corpo (como cabeça e pés) e envia um alerta HTTP para o ESP32 sempre que a altura medida for abaixo de um valor mínimo predefinido.

## 🚀 Funcionalidades

- **Captura de vídeo em tempo real**: Transmite imagens do ESP32-CAM para o Python.
- **Detecção de pose humana**: Usando o **MediaPipe**, detecta a pose humana em tempo real e calcula a altura da pessoa.
- **Monitoramento de altura**: Calcula a altura com base na distância entre a cabeça e os pés.
- **Envio de alertas**: Se a altura da pessoa for inferior a um limite pré-configurado, um alerta HTTP é enviado ao ESP32 para realizar alguma ação (como acionar um alarme ou outro dispositivo conectado).

## 📸 Como Funciona

1. O **ESP32-CAM** envia um stream de vídeo via HTTP.
2. O script Python captura as imagens do stream e as processa com a biblioteca **OpenCV**.
3. **MediaPipe** detecta os pontos de referência do corpo humano, como cabeça, ombros, joelhos e tornozelos.
4. A altura da pessoa é calculada com base na distância entre o topo da cabeça e os tornozelos (pés).
5. Se a altura medida for abaixo do valor mínimo definido (`MIN_HEIGHT_PX`), um alerta HTTP é enviado para o ESP32.

## ⚙️ Requisitos

Antes de rodar o projeto, certifique-se de ter os seguintes requisitos:

- **Python 3.x** instalado.
- **ESP32-CAM** configurado para transmitir vídeo via HTTP.

## 🛠️ Como Rodar

### 1. Clone o Repositório

Primeiro, clone o repositório para sua máquina local:

```bash
git clone https://github.com/seu-usuario/esp32-mediapipe-pose.git
cd esp32-mediapipe-pose
