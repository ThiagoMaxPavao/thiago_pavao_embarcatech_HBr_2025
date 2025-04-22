#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "ssd1306.h"

// --------------------------- Pinagem ---------------------------

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// --------------------------- Random ---------------------------

// chance from 0 to 100
// returns +1 or -1, for the given chance of +1
int get_random_direction(int chance) {
    uint64_t randValue = get_rand_64();
    uint8_t randChance = (randValue % 100);
    if (randChance < chance) {
        return 1;
    } else {
        return -1;
    }
}

// --------------------------- Display OLED ---------------------------

ssd1306_t disp;

int main() {
    stdio_init_all();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    ssd1306_clear(&disp);
    ssd1306_draw_pixel(&disp, 0,32);
    ssd1306_show(&disp);

    while (true) {
        sleep_ms(1000);
    }
}
