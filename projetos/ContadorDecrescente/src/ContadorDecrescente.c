#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6

// Tempo para ignorar oscilações do sinal
#define DEBOUNCE_TIME 50

// Tempo da última borda detectada no sinal da porta.
// Para que a borda seja considerada válida a diferença deve
// ser maior do que DEBOUNCE_TIME.
volatile int64_t button_A_last_edge_time = 0;
volatile int64_t button_B_last_edge_time = 0;

void process_button_A() {
    printf("Button A pressed\n");
}

void process_button_B() {
    printf("Button B pressed\n");
}

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

int main() {
    stdio_init_all();

    buttons_init();

    while (true) {
        sleep_ms(1000);
    }
}
