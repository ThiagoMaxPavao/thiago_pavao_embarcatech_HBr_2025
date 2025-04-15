#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/adc.h"

// --------------------------- Pinagem ---------------------------

// Joystick
#define JOY_X_PIN 26
#define JOY_Y_PIN 27

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// --------------------------- Joystick ---------------------------

// Inicializa ADC e pinos GPIO para o joystick
void joystick_init(uint X_pin, uint Y_pin) {
    adc_init();
    adc_gpio_init(X_pin);
    adc_gpio_init(Y_pin);
}

// Retorna as coordenadas do joystick em um intervalo de -1 a 1
void joystick_get_XY(float *x, float *y) {
    adc_select_input(0);
    uint adc_y_raw = adc_read();
    adc_select_input(1);
    uint adc_x_raw = adc_read();

    // Convert raw ADC values to joystick coordinates
    // X: 0 - 4095 -> -1 - 1
    // Y: 0 - 4095 -> -1 - 1
    *x = adc_x_raw / 2047.5 - 1;
    *y = adc_y_raw / 2047.5 - 1;
}

// --------------------------- Display OLED ---------------------------

ssd1306_t disp;

// Desenha as coordenadas do joystick na tela OLED, de -100 a 1000
void draw_coords(float x, float y) {
    char buffer[32];

    ssd1306_clear(&disp);
    snprintf(buffer, sizeof(buffer), "X: % d", (int) (100*x));
    ssd1306_draw_string(&disp, 10, 10, 2, buffer);
    snprintf(buffer, sizeof(buffer), "Y: % d", (int) (100*y));
    ssd1306_draw_string(&disp, 10, 30, 2, buffer);
    ssd1306_show(&disp);
}


// --------------------------- Lógica Principal ---------------------------

int main() {
    // Inicializa os periféricos
    stdio_init_all();
    joystick_init(JOY_X_PIN, JOY_Y_PIN);

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    // Lógica principal, lê coordenadas e atualiza o display a cada 10ms
    float x, y;

    while (true) {
        joystick_get_XY(&x, &y);
        draw_coords(x, y);
        sleep_ms(10);
    }
}
