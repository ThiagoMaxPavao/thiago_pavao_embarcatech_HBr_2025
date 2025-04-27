#include "led_embutido.h"
#include "pico/cyw43_arch.h"

int led_embutido_init() {
    if(cyw43_arch_init()) {
        return -1;
    }
    return 0;
}

void led_embutido_on() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
}

void led_embutido_off() {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
}
