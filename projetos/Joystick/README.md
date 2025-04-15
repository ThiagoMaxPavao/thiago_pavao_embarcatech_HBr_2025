# Joystick

## 🎯 Objetivo do Projeto

Este projeto consiste em um sistema de leitura e exibição em tempo real das coordenadas de um joystick analógico. Utilizando o conversor analógico-digital (ADC) do Raspberry Pi Pico W, o sistema captura continuamente os valores dos eixos X e Y do joystick, normaliza os dados para a faixa de -1 a 1, e os exibe em um display OLED na faixa de -100 a 100.

## 🔧 Componentes Utilizados

- Raspberry Pi Pico W
- Joystick analógico (2 eixos)
- Display OLED 0,96” (via I2C)
- Bateria recarregável

## 💾 Como Compilar e Executar o Código

1. Abra o projeto no VSCode, utilizando um ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto (Ctrl+Shift+B no VSCode ou via terminal com `cmake` e `make`);
3. Conecte a Raspberry Pi Pico via cabo USB, pressionando o botão **BOOTSEL** ao conectar para ativar o modo de boot;
4. Copie o arquivo `.uf2` gerado para a unidade de armazenamento "RPI-RP2" que aparecerá;
5. A Pico reiniciará automaticamente e iniciará a execução do programa;
6. O display OLED passará a mostrar as coordenadas em tempo real conforme o movimento do joystick.

🔗 Extensão recomendada para VSCode: [Raspberry Pi Pico SDK Extension](https://github.com/raspberrypi/pico-vscode)

## ⚡ Pinagem dos Dispositivos Utilizados

| Pino | Função        | Periférico     |
|------|---------------|----------------|
| 26   | ADC0 - Eixo Y | Joystick       |
| 27   | ADC1 - Eixo X | Joystick       |
| 14   | I2C1 - SDA    | Display OLED   |
| 15   | I2C1 - SCL    | Display OLED   |

## 🖼️ Imagens e Vídeos

- **Vídeo do programa em execução:** https://youtube.com/shorts/Gfij5uwhnuE

## 🧠 Lógica de Funcionamento

1. **Inicialização:**
   - O sistema inicia configurando os pinos ADC correspondentes aos eixos do joystick;
   - O display OLED é inicializado via I2C;

2. **Leitura do joystick:**
   - A cada 10 ms, o programa realiza uma leitura dos valores brutos dos canais ADC conectados aos eixos X e Y;
   - Esses valores são normalizados para uma faixa de -1 a 1, representando o deslocamento do joystick em cada eixo;

3. **Exibição dos dados:**
   - As coordenadas normalizadas são convertidas para porcentagem (de -100 a 100);
   - O display OLED mostra os valores de X e Y em tempo real com atualização contínua.

## 📂 Estrutura de Arquivos

O código-fonte se encontra na pasta `src`, destacam-se os arquivos:

- `Joystick.c`: Implementação da lógica de leitura dos eixos X e Y e visualização no display OLED;
- `inc/ssd1306.c` e `inc/ssd1306.h`: Biblioteca para comunicação com o display OLED via I2C.

## 📊 Resultados

- Leitura estável e em tempo real dos valores analógicos do joystick;
- Interface visual clara com atualização fluida das coordenadas;
- Projeto simples e eficaz para testes de movimentação e controle com joystick.
