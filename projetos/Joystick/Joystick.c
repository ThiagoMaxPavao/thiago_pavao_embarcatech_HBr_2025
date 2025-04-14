#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"

// --------------------------- Pinagem ---------------------------

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// --------------------------- Display OLED ---------------------------

ssd1306_t disp;

int main()
{
    stdio_init_all();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    ssd1306_clear(&disp);

    ssd1306_draw_string_by_center(&disp, 64, 32, 2, "Teste");

    ssd1306_show(&disp);

    while (true) {
        sleep_ms(1000);
    }
}
