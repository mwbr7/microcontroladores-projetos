# 🏊‍♂️ **Monitoramento de Piscina para Prevenção de Afogamento com ESP32 + MediaPipe**

Este projeto utiliza um **ESP32-CAM** para monitorar a piscina em tempo real e ajudar na **prevenção de afogamentos** de crianças. Ele calcula a altura da pessoa detectada com base nas coordenadas da cabeça e dos pés e envia um alerta caso a altura seja suspeita, indicando que uma criança pode estar em risco (submersa, caída ou em uma posição perigosa na piscina).

## 🚀 Funcionalidades

- **Captura de vídeo em tempo real**: O **ESP32-CAM** transmite vídeo ao vivo via HTTP, permitindo monitorar a piscina continuamente.
- **Detecção de pose humana**: Usando a biblioteca **MediaPipe**, o sistema detecta a pose da pessoa na piscina e calcula sua altura em tempo real.
- **Cálculo de altura**: A altura da pessoa é calculada com base na distância entre a cabeça e os tornozelos, ajudando a identificar situações de risco, como uma criança submersa.
- **Envio de alerta**: Se a altura da pessoa for inferior ao valor mínimo definido (indicando que a pessoa pode estar submersa ou caída), o sistema envia um alerta HTTP para o ESP32, que pode disparar um alarme ou notificar um responsável.

## 🏊‍♀️ Como Funciona

1. O **ESP32-CAM** transmite um stream de vídeo ao vivo da piscina.
2. O código Python captura as imagens do stream e as processa utilizando **MediaPipe** para detectar a posição da pessoa.
3. A altura da pessoa é calculada com base nos pontos de referência da pose (cabeça e tornozelos).
4. Se a altura calculada for abaixo de um valor mínimo pré-definido, o sistema interpreta isso como uma possível situação de afogamento (como a pessoa estar submersa) e envia um alerta ao ESP32.
5. O ESP32 pode então tomar ações, como disparar um alarme sonoro ou notificar um responsável para que a ação imediata seja tomada.

## 📸 Requisitos

Para rodar o projeto, você precisará dos seguintes componentes:

- **Python 3.x** instalado.
- **ESP32-CAM** configurado para transmitir vídeo via HTTP.
- **Bibliotecas Python** necessárias (veja abaixo).

## 🛠️ Como Rodar

### 1. Clonar o Repositório

Clone o repositório para sua máquina local:

```bash
git clone https://github.com/seu-usuario/monitoramento-piscina-afogamento.git
cd monitoramento-piscina-afogamento
