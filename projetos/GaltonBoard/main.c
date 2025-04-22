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

void draw_board(ssd1306_t *disp, int scale, int n_lines) {
    int x_offset = scale;
    int y_offset = (disp->height - scale)/2;

    for(int line = 0; line < n_lines; line++) {
        int n_pins = line + 1;
        for(int column = 0; column < n_pins; column++) {
            int x = x_offset + 2*scale*line;
            int y = y_offset - scale*line + 2*scale*column;
            ssd1306_draw_square(disp, x, y, scale, scale);
        }
        n_pins++;
    }
    ssd1306_show(disp);
}

void draw_ball(ssd1306_t *disp, int scale, int x, int y) {
    int y_offset = (disp->height - scale)/2;

    ssd1306_draw_square(disp, x*scale, y*scale + y_offset, scale, scale);
    ssd1306_show(disp);
}

void clear_ball(ssd1306_t *disp, int scale, int x, int y) {
    int y_offset = (disp->height - scale)/2;

    ssd1306_clear_square(disp, x*scale, y*scale + y_offset, scale, scale);
    ssd1306_show(disp);
}

// --------------------------- Main ---------------------------

ssd1306_t disp;

int main() {
    stdio_init_all();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    ssd1306_clear(&disp);
    draw_board(&disp, 5, 5);

    int x_ball = 0;
    int y_ball = 0;

    while (true) {
        draw_ball(&disp, 5, x_ball, y_ball);
        sleep_ms(200);
        clear_ball(&disp, 5, x_ball, y_ball);
        sleep_ms(300);

        x_ball += 1;
        if(x_ball % 2 == 1) y_ball += get_random_direction(50);
    }
}
