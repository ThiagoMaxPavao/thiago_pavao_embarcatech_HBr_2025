# GaltonBoard

## 🎯 Objetivo do Projeto

O Galton Board é um dispositivo composto por uma série de pinos dispostos em linhas e uma sequência de canaletas abaixo da última linha de pinos. Esferas são liberadas do topo, acima de um pino central, e descem pelo dispositivo mudando de direção conforme colidem com os pinos, até serem depositadas em uma das canaletas. O dispositivo tem como objetivo demonstrar a Lei dos Grandes Números, evidenciando que, quando o número de esferas que passam pelo experimento é suficientemente grande, a distribuição entre as canaletas segue uma Gaussiana, algo previsível em meio à aleatoriedade das colisões com cada linha de pinos.

## 🔧 Componentes Usados
- Raspberry Pi Pico W
- Display OLED 0,96”
- Buzzer passivo
- Joystick
- 2x Push Buttons

## 💾 Como Compilar e Executar o Código

1. Abra o projeto no VSCode, usando o ambiente com suporte ao SDK do Raspberry Pi Pico (CMake + compilador ARM);
2. Compile o projeto normalmente (Ctrl+Shift+B no VSCode ou via terminal com cmake e make);
3. Conecte sua BitDogLab via cabo USB e coloque a Pico no modo de boot (pressione o botão BOOTSEL e conecte o cabo);
4. Copie o arquivo .uf2 gerado para a unidade de armazenamento que aparece (RPI-RP2);
5. A Pico reiniciará automaticamente e começará a executar o código.

🔗 Extensão do VSCode: [Raspberry Pi Pico SDK Extension](https://github.com/raspberrypi/pico-vscode)

## ▶️ Como Usar

Quando em funcionamento, o projeto inicia automaticamente a simulação, liberando uma esfera após a outra. O sistema também realiza a contagem de esferas em cada canaleta e monta um histograma para representar visualmente essa contagem. Quando a barra do histograma de alguma canaleta atinge o limite da tela, o sistema ajusta a escala do desenho, fazendo com que essa barra ocupe toda a altura da tela e as demais sejam redimensionadas proporcionalmente, de acordo com o número de esferas depositadas em relação à maior.

No canto inferior esquerdo da tela é possível visualizar o número total de esferas que já foram depositadas em alguma canaleta.

Para interagir:
- **Pressionar os botões A e B**: altera a escala e o número de linhas desenhadas, consequentemente modificando a quantidade de canaletas. Isso permite tanto observar uma simulação mais ampla com menos linhas quanto uma mais detalhada. Ao pressionar qualquer um dos botões, a escala é modificada e a simulação reinicia, zerando o histograma e a contagem.
- **Girar o joystick**: ajusta a velocidade da simulação, possibilitando "acelerar" ou desacelerar o andamento. Para isso, basta levar o joystick até o fim de curso em qualquer direção e, mantendo-o pressionado contra o limite, deslizar no sentido horário ou anti-horário.

## ⚡ Pinagem dos Dispositivos Utilizados
| Pino | Função | Periférico |
|------|--------|------------|
| 5  | GPIO + Interrupção | Botão A |
| 6  | GPIO + Interrupção | Botão B |
| 14 | I2C1 - SDA | Display OLED |
| 15 | I2C1 - SCL | Display OLED |
| 21 | GPIO | Buzzer |
| 26 | ADC | Joystick - X |
| 27 | ADC | Joystick - Y |

## 🖼️ Imagens e Vídeos

...

## 🛠️ Desenvolvimento

Primeiramente, foi desenvolvida uma função que retorna +1 ou -1 de forma aleatória, a partir de uma probabilidade de +1 recebida como parâmetro. A função foi implementada utilizando a biblioteca `pico/rand.h` do SDK. Em seguida, realizou-se um experimento configurando a chance para 50%, com o objetivo de verificar se a função era capaz de gerar números aleatórios de forma balanceada.

O experimento demonstrou que, para uma grande quantidade de sorteios, a função de fato gera valores equilibrados, como pode ser observado na saída do programa exibida abaixo:

```
Count positive/negative: 49880/50120
Count positive/negative: 50092/49908
Count positive/negative: 49943/50057
Count positive/negative: 50258/49742
Count positive/negative: 49867/50133
Count positive/negative: 50176/49824
Count positive/negative: 50400/49600
Count positive/negative: 49784/50216
```

## 🧠 Lógica de Funcionamento

A lógica principal do programa está no loop da função `main`, responsável por:
- Monitorar a posição do joystick e atualizar a frequência da simulação de acordo;
- Atualizar o display OLED, redesenhando os pinos, as esferas, o histograma e o contador. A atualização ocorre, no máximo, a uma taxa definida por uma diretiva de pré-processador presente no arquivo de configuração (`MAX_FPS`);
- Atualizar a posição das esferas na simulação e contabilizar aquelas que chegam ao final do percurso no histograma.

A alteração da escala de desenho e do número de linhas de pinos é realizada via interrupção nos pinos GPIO dos botões A e B.

Todas essas responsabilidades são executadas por meio de chamadas às funções dos módulos do programa, mantendo o loop principal enxuto e organizado.

## 📂 Estrutura de Arquivos

O código-fonte está organizado dentro da pasta `src`, estruturado da seguinte forma:

Existem três diretórios principais:
- `app/` - contém os arquivos com a lógica de negócio da aplicação:
  - `main.c`: responsável pela inicialização do sistema e pelo loop principal de execução.
  - `simulation.c`: implementa toda a lógica da simulação das esferas caindo pelos pinos, além do desenho desses componentes na tela.
  - `histogram.c`: cuida da lógica de gerenciamento e desenho do histograma, assim como da contagem total de esferas exibida.
  - `util.c`: reúne funções auxiliares para o funcionamento geral, como a geração de direções aleatórias (-1/+1).
- `hal/` - implementa facilidades para interação com os periféricos utilizados:
  - `button.c`: lógica de interrupção para os botões A e B.
  - `joystick.c`: leitura e conversão dos valores do ADC conectados ao joystick.
  - `ssd1306.c`: lógica de comunicação e desenho no display OLED via I2C.
- `include/` - contém os arquivos de cabeçalho dos módulos e bibliotecas:
  - `config.h`: define parâmetros de configuração do programa, como a pinagem de cada componente e os limites de frequência mínima e máxima da simulação.
  - Outros arquivos de cabeçalho expõem as funcionalidades de cada um dos módulos do programa.

## 📈 Resultados

O projeto cumpriu seu objetivo de ilustrar a distribuição Gaussiana conforme previsto pela Lei dos Grandes Números. À medida que mais esferas são liberadas, o histograma exibido na tela forma uma curva simétrica, confirmando o comportamento estatístico esperado da simulação.
