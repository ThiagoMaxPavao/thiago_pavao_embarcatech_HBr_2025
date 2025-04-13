#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_A_PIN 5

void gpio_callback(uint gpio, uint32_t event_mask) {
    printf("Button A pressed\n");
}

int main() {
    stdio_init_all();

    gpio_init(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);

    while (true) {
        sleep_ms(1000);
    }
}
