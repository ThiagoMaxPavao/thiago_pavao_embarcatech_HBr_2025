# Temperatura Interna

## 🎯 Objetivo do Projeto

Este projeto realiza a leitura da temperatura interna do microcontrolador Raspberry Pi Pico W utilizando seu sensor embutido. A temperatura é convertida e exibida em tempo real em um display OLED, atualizando o valor a cada segundo.

O objetivo principal é demonstrar a leitura de valores analógicos internos por meio do ADC e a conversão desses dados para uma representação em graus Celsius.

Durante os testes, foi observado que os valores de temperatura variam rapidamente, por exemplo, de 29°C para 37°C em apenas um segundo. Isso ocorre devido à baixa precisão e instabilidade térmica do sensor interno.

## 🔧 Componentes Utilizados

- Raspberry Pi Pico W
- Display OLED 0,96” (via I2C)

## 💾 Como Compilar e Executar o Código

1. Abra o projeto no VSCode, utilizando um ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto (Ctrl+Shift+B no VSCode ou via terminal com `cmake` e `make`);
3. Conecte a Raspberry Pi Pico via cabo USB, pressionando o botão **BOOTSEL** ao conectar para ativar o modo de boot;
4. Copie o arquivo `.uf2` gerado para a unidade de armazenamento "RPI-RP2" que aparecerá;
5. A Pico reiniciará automaticamente e iniciará a execução do programa;
6. O display OLED passará a mostrar as coordenadas em tempo real conforme o movimento do joystick.

🔗 Extensão recomendada para VSCode: [Raspberry Pi Pico SDK Extension](https://github.com/raspberrypi/pico-vscode)

## ⚡ Pinagem dos Dispositivos Utilizados

| Pino | Função     | Periférico     |
|------|------------|----------------|
| X    | ADC        | Sensor de temperatura |
| 14   | I2C1 - SDA | Display OLED   |
| 15   | I2C1 - SCL | Display OLED   |

O sensor de temperatura interno da Raspberry Pi Pico não está ligado à um pino GPIO da placa, por isso, nesta tabela o pino foi representado com um `X`.

## 🖼️ Imagens e Vídeos

- **Foto do programa em execução:** medindo aproximadamente 34 graus Celsius.

![foto do programa em execucao](assets/temperatura.jpeg)

## 🧠 Lógica de Funcionamento

1. **Inicialização:**
   - Ativa o sensor de temperatura interno e seleciona o canal ADC correspondente;
   - Inicializa o display OLED via I2C;

2. **Leitura e Conversão:**
   - A cada segundo, realiza uma leitura do ADC e converte para temperatura em Celsius;
   - A temperatura é formatada e exibida no centro do display OLED.

## 📂 Estrutura de Arquivos

- `TemperaturaInterna.c`: Lógica principal de leitura e exibição da temperatura;
- `inc/ssd1306.c` e `inc/ssd1306.h`: Biblioteca para controle do display OLED.

## 📊 Resultados

- Leitura funcional da temperatura interna;
- Variação significativa nos valores devido à baixa precisão do sensor embutido;
- Visualização clara e atualização automática no display OLED.
