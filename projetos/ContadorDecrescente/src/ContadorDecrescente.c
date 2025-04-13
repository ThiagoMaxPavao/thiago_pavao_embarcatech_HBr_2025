#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"

// --------------------------- Pinagem ---------------------------

// Botões A e B
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// --------------------------- Lógica de Debounce dos botões ---------------------------

// Tempo para ignorar oscilações do sinal
#define DEBOUNCE_TIME 1

// Tempo da última borda detectada no sinal da porta.
// Para que a borda seja considerada válida a diferença deve
// ser maior do que DEBOUNCE_TIME.
volatile int64_t button_A_last_edge_time = 0;
volatile int64_t button_B_last_edge_time = 0;

void process_button_A();
void process_button_B();

// Rotina de interrupção para os botões
// Realiza debounce e chama a função de callback apenas na borda de descida (pressionamento do botão)
void gpio_callback(uint gpio, uint32_t events) {
    int64_t now = to_ms_since_boot(get_absolute_time());

    if(gpio == BUTTON_A_PIN) {
        if(events & GPIO_IRQ_EDGE_FALL && !(events & GPIO_IRQ_EDGE_RISE) && now - button_A_last_edge_time > DEBOUNCE_TIME) {
            process_button_A();
        }
        button_A_last_edge_time = now;
    }
    else if(gpio == BUTTON_B_PIN) {
        if(events & GPIO_IRQ_EDGE_FALL && !(events & GPIO_IRQ_EDGE_RISE) && now - button_B_last_edge_time > DEBOUNCE_TIME) {
            process_button_B();
        }
        button_B_last_edge_time = now;
    }
}

void buttons_init() {
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
}

// --------------------------- Display OLED ---------------------------

ssd1306_t disp;

void draw_display(int tempo_restante, int contagem_pressionamentos) {
    char buffer[20];
    static int tempo_restante_atual = -1;
    static int contagem_pressionamentos_atual = -1;

    // Atualiza o display apenas se os valores mudaram
    if (tempo_restante == tempo_restante_atual && contagem_pressionamentos == contagem_pressionamentos_atual) {
        return;
    }

    // Atualiza os valores atuais
    tempo_restante_atual = tempo_restante;
    contagem_pressionamentos_atual = contagem_pressionamentos;

    // Limpa o display e desenha os novos valores
    ssd1306_clear(&disp);

    ssd1306_draw_string_by_center(&disp, 64, 10, 2, "Contagem");

    sprintf(buffer, "- %d -", contagem_pressionamentos);
    ssd1306_draw_string_by_center(&disp, 64, 30, 2, buffer);

    sprintf(buffer, "Tempo: %d", tempo_restante);
    ssd1306_draw_string_by_center(&disp, 64, 55, 2, buffer);

    ssd1306_show(&disp);
}

// --------------------------- Lógica principal ---------------------------

// Variáveis globais
volatile bool contando = false; // False se está congelado, esperando pressionamento do botão A. True se está contando.
volatile int contagem_pressionamentos = 0;
int tempo_restante = 0;
volatile int64_t tempo_ultima_atualizacao_tempo = 0;

void process_button_A() {
    contagem_pressionamentos = 0;
    tempo_restante = 9;
    contando = true;
    tempo_ultima_atualizacao_tempo = to_ms_since_boot(get_absolute_time());
}

void process_button_B() {
    if(contando)
        contagem_pressionamentos++;
}

int main() {
    // Inicializa periféricos
    stdio_init_all();
    buttons_init();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    // Loop principal - controle do tempo restante e atualização dos displays
    while (true) {
        int64_t now = to_ms_since_boot(get_absolute_time());

        if(now - tempo_ultima_atualizacao_tempo > 1000 && contando) {
            tempo_restante--;
            tempo_ultima_atualizacao_tempo = now;

            if(tempo_restante <= 0) {
                contando = false;
            }
        }

        draw_display(tempo_restante, contagem_pressionamentos);

        sleep_ms(10);
    }
}
