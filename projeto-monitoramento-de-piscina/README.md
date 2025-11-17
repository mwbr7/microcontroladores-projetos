# 🏊‍♂️ **Monitoramento de Crianças Antes de Entrar na Piscina com ESP32 + MediaPipe**

Este projeto utiliza um **ESP32-CAM** para monitorar a área ao redor da piscina, ajudando a **prevenir o afogamento de crianças** antes mesmo que elas entrem na água. O sistema calcula a altura da pessoa (com base na detecção de pose utilizando a biblioteca **MediaPipe**) para identificar se a pessoa é uma criança, e envia um alerta caso a criança esteja se aproximando ou entrando em uma área perigosa (como o beiral da piscina) sem supervisão.

## 🚀 Funcionalidades

- **Captura de vídeo em tempo real**: O **ESP32-CAM** transmite vídeo ao vivo via HTTP, monitorando a área ao redor da piscina.
- **Detecção de pose humana**: Usando a biblioteca **MediaPipe**, o sistema detecta a presença de pessoas nas imediações da piscina e calcula sua altura em tempo real.
- **Identificação de crianças**: A altura calculada permite determinar se a pessoa é uma criança (com base em um valor mínimo configurado para altura).
- **Envio de alerta**: Se uma criança for detectada se aproximando da piscina ou entrando em uma área de risco, o sistema envia um alerta ao ESP32, que pode disparar um alarme ou notificar os responsáveis.

## 🏊‍♀️ Como Funciona

1. O **ESP32-CAM** transmite um stream de vídeo ao vivo da área ao redor da piscina.
2. O código Python captura as imagens do stream e as processa utilizando **MediaPipe** para detectar a pose da pessoa.
3. A altura da pessoa é calculada com base nos pontos de referência do corpo (cabeça e tornozelos), permitindo identificar se a pessoa é uma criança.
4. Caso uma criança (baseada na altura) seja detectada perto da piscina, o sistema envia um alerta para o ESP32.
5. O ESP32 pode então disparar um alarme sonoro ou enviar uma notificação para os responsáveis, indicando que uma criança pode estar entrando na piscina sem supervisão.

## 📸 Requisitos

Para rodar o projeto, você precisará dos seguintes componentes:

- **Python 3.x** instalado.
- **ESP32-CAM** configurado para transmitir vídeo via HTTP.
- **Bibliotecas Python** necessárias (veja abaixo).

## 🛠️ Como Rodar

### 1. Clonar o Repositório

Clone o repositório para sua máquina local:

```bash
git clone https://github.com/seu-usuario/monitoramento-criancas-piscina.git
cd monitoramento-criancas-piscina
