# Led Embutido

## 🎯 Objetivo do Projeto

Este é um projeto simples que apenas faz com que o LED embutido da Raspberry Pi Pico W pisque com período de 1 segundo. Ele foi desenvolvido para exercitar a organização de arquivos de um projeto seguindo uma arquitetura modular, com camada de _drivers_, _hal_ e _aplicação_.

## 🔧 Componentes Utilizados

- Raspberry Pi Pico W

## 💾 Como Compilar e Executar o Código

1. Abra o projeto no VSCode, utilizando um ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto (Ctrl+Shift+B no VSCode ou via terminal com `cmake` e `make`);
3. Conecte a Raspberry Pi Pico via cabo USB, pressionando o botão **BOOTSEL** ao conectar para ativar o modo de boot;
4. Copie o arquivo `.uf2` gerado para a unidade de armazenamento "RPI-RP2" que aparecerá;
5. A Pico reiniciará automaticamente e iniciará a execução do programa.

🔗 Extensão recomendada para VSCode: [Raspberry Pi Pico SDK Extension](https://github.com/raspberrypi/pico-vscode)

## ⚡ Pinagem dos Dispositivos Utilizados

O único periférico utilizado foi o LED embutido na placa.

## 📂 Estrutura de Arquivos

O código-fonte se encontra na pasta `src`, destacam-se os subdiretórios:

- `app`: Camada de aplicação, que utiliza do HAL para executar a lógica principal do programa.
- `drivers`: Camada de interação com o hardware, com funções de baixo nível. Neste caso, estão sendo utilizadas as APIs da SDK.
- `hal`: Camada de abstração de hardware, fornencedo funções de alto nível para a camada de aplicação.
