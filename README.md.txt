# Bidirectional People Counter – Arduino

🇺🇸 English version below  
🇧🇷 Versão em português abaixo

---

# 🇺🇸 English Version

## Overview

This project implements a **bidirectional people counting system** for closed environments with two entrances: a **front door** and a **back door**.

Infrared sensors installed at each door detect the movement of people entering or leaving the environment. The system determines the direction of movement and updates the internal counter accordingly.

The number of people inside the environment is displayed using **two multiplexed 7-segment displays**. When the room is empty, the LED indicator remains off. When at least one person enters, the LED turns on.

This project was developed as part of an **academic study for an Embedded Systems course**, focusing on sensor interfacing, real-time detection, and microcontroller programming.

---

## Prototype

Below are example images of the prototype hardware. Place your prototype photos inside the **prototype** folder.

![5 People Back Entrance](prototype/5_people_back.jpg)  
![5 People Front Door Entrance](prototype/5_people_front.jpg)  
![Exit 4 People Back Door](prototype/exit_4_back.jpg)  
![Exit 6 People Front Door](prototype/exit_6_front.jpg)  
![LED Off](prototype/led_off.jpg)  
![On Hold](prototype/on_hold.jpg)  
![Reset](prototype/reset.jpg)  
![7 People Entered](prototype/7_people_entered.jpg)  

---

## Demonstration Video

Project demonstration video:

[![Watch the demo](https://img.youtube.com/vi/Kw_Jyvw40fI/0.jpg)](https://www.youtube.com/watch?v=Kw_Jyvw40fI)

> Click the image above to watch the demonstration video on YouTube.

---

## Project Presentation

The complete project presentation (PowerPoint) can be found here:

`project_presentation/Sistema_de_Contagem_Bidirecional_de_Pessoas.pptx`

This presentation contains:

* Project explanation  
* System operation  
* Circuit description  
* Demonstration images

---

## System Features

* Bidirectional people detection  
* Entry and exit monitoring through two doors  
* Real-time people counting  
* Multiplexed 7-segment display control  
* LED occupancy indicator  
* Manual reset button  
* Direction detection using sensor activation sequence

---

## Hardware Components

* Arduino Uno  
* 2 × Seven Segment Displays  
* 3 × Infrared Sensors  
* 1 × LDR (Light Dependent Resistor)  
* 2 × NPN Transistors (BC547) for display multiplexing  
* 1 × LED indicator  
* 1 × Reset push button  
* Resistors  
* Protoboard  
* Jumper wires

---

## Pin Configuration

### Seven Segment Displays

| Segment | Arduino Pin |
| ------- | ----------- |
| a       | 7           |
| b       | 8           |
| c       | 4           |
| d       | 3           |
| e       | 2           |
| f       | 6           |
| g       | 5           |

### Display Control

| Display   | Function | Arduino Pin |
| --------- | -------- | ----------- |
| Display 1 | Units    | 9           |
| Display 2 | Tens     | 11          |

### Front Door Sensors

| Sensor     | Function        | Arduino Pin |
| ---------- | --------------- | ----------- |
| EXT_FRENTE | External sensor | A5          |
| INT_FRENTE | Internal sensor | A3          |

### Back Door Sensors

| Sensor          | Function        | Arduino Pin |
| --------------- | --------------- | ----------- |
| EXT_FUNDO       | External sensor | A4          |
| INT_FUNDO (LDR) | Internal sensor | A1          |

### LED Indicator

| Component | Arduino Pin |
| --------- | ----------- |
| LED       | A0          |

### Reset Button

| Component    | Arduino Pin |
| ------------ | ----------- |
| Reset Button | 13          |

---

## System Operation

The system determines if a person is **entering or leaving** by analyzing the order of sensor activation.

### Entry Detection

1. External sensor is activated  
2. Internal sensor is activated shortly after  
3. The system increments the people counter

### Exit Detection

1. Internal sensor is activated  
2. External sensor is activated shortly after  
3. The system decrements the people counter

This logic is implemented using a **state machine algorithm** in the firmware.

---

## Display Operation

The number of people inside the environment is displayed using **two multiplexed seven-segment displays**.  
Counting range: 0 – 99 people.

---

## Occupancy Indicator

* LED OFF → Room empty  
* LED ON → At least one person inside

---

## Reset Function

* Pressing the reset button resets the people counter to zero.  
* The LED blinks indicating the reset operation.

---

## Project Structure

people-counter-arduino/
│
├── lib/
├── src/
│ └── Sistema_de_Contagem_Bidirecional_3636800.ino
├── prototype/
│ ├── 5_people_back.jpg
│ ├── 5_people_front.jpg
│ ├── exit_4_back.jpg
│ ├── exit_6_front.jpg
│ ├── led_off.jpg
│ ├── on_hold.jpg
│ ├── reset.jpg
│ └── 7_people_entered.jpg
├── project_presentation/
│ └── Sistema_de_Contagem_Bidirecional_de_Pessoas.pptx
└── README.md

---

## Possible Applications

* Smart room monitoring  
* Building occupancy control  
* Energy-efficient lighting systems  
* Access monitoring systems  
* Smart building automation

---

# 🇧🇷 Versão em Português

## Visão Geral

Este projeto implementa um **sistema de contagem bidirecional de pessoas** para ambientes fechados com duas entradas: **porta da frente** e **porta dos fundos**.

Sensores infravermelhos detectam pessoas entrando ou saindo, e o sistema identifica a direção e atualiza o contador interno.

O número de pessoas é exibido em **dois displays de 7 segmentos multiplexados**. Quando a sala está vazia, o LED indicador permanece desligado; quando alguém entra, o LED acende.

---

## Protótipo

Exemplo de imagens do protótipo. Coloque suas fotos dentro da pasta **prototype**.

![Entrada Porta de Trás](prototype/5_people_back.jpg)  
![Entrada Porta da Frente](prototype/5_people_front.jpg)  
![Saída 4 Pessoas Porta de Trás](prototype/exit_4_back.jpg)  
![Saída 6 Pessoas Porta da Frente](prototype/exit_6_front.jpg)  
![LED Desligado](prototype/led_off.jpg)  
![Em Espera](prototype/on_hold.jpg)  
![Reset](prototype/reset.jpg)  
![Entraram 7 Pessoas](prototype/7_people_entered.jpg)

---

## Vídeo de Demonstração

- **YouTube**: [Vídeo de Demonstração](https://www.youtube.com/watch?v=Kw_Jyvw40fI&t=13s)

> O vídeo está hospedado no YouTube. Nenhuma cópia local é incluída.

---

## Apresentação do Projeto

Apresentação em PowerPoint:

`project_presentation/Sistema_de_Contagem_Bidirecional_de_Pessoas.pptx`

Contém: explicação do projeto, funcionamento, descrição do hardware e imagens do protótipo.

---

## Características do Sistema

* Detecção bidirecional de pessoas  
* Monitoramento de entrada e saída por duas portas  
* Contagem em tempo real  
* Exibição em displays de 7 segmentos  
* LED indicador de ocupação  
* Botão de reset manual  
* Detecção de direção baseada na sequência de sensores

---

## Componentes de Hardware

* Arduino Uno  
* 2 × Displays de 7 segmentos  
* 3 × Sensores infravermelhos  
* 1 × LDR  
* 2 × Transistores NPN (BC547) para multiplexação  
* 1 × LED indicador  
* 1 × Botão de reset  
* Resistores  
* Protoboard  
* Jumpers

---

## Funcionamento do Sistema

- Entrada: Sensor externo → Sensor interno → Incrementa contador  
- Saída: Sensor interno → Sensor externo → Decrementa contador

Implementado com **máquina de estados** no firmware.

---

## Funcionamento dos Displays

Exibição de pessoas em dois displays de 7 segmentos multiplexados.  
Faixa de contagem: 0 – 99 pessoas.

---

## Indicador de Ocupação

* LED OFF → Sala vazia  
* LED ON → Pelo menos uma pessoa dentro

---

## Função Reset

* Botão de reset reinicia o contador para zero.  
* LED pisca indicando o reset.

---

## Estrutura do Projeto

people-counter-arduino/
│
├── lib/
├── src/
│ └── Sistema_de_Contagem_Bidirecional_3636800.ino
├── prototype/
│ ├── 5_people_back.jpg
│ ├── 5_people_front.jpg
│ ├── exit_4_back.jpg
│ ├── exit_6_front.jpg
│ ├── led_off.jpg
│ ├── on_hold.jpg
│ ├── reset.jpg
│ └── 7_people_entered.jpg
├── project_presentation/
│ └── Sistema_de_Contagem_Bidirecional_de_Pessoas.pptx
└── README.md

---

## Possíveis Aplicações

* Monitoramento de ocupação de salas  
* Controle de fluxo de pessoas  
* Sistemas de iluminação inteligente  
* Monitoramento de acesso  
* Automação predial
