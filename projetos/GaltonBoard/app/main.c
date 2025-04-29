#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"

#include "ball.h"
#include "histogram.h"

// --------------------------- Pinagem ---------------------------

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

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

// --------------------------- Main ---------------------------

ssd1306_t disp;

int main() {
    stdio_init_all();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    int scale = 4;
    int n_lines = 7;
    int n_updates = 1;

    ssd1306_clear(&disp);
    draw_board(&disp, scale, n_lines);

    init_histogram(n_lines+1);

    while (true) {
        draw_balls(&disp, scale);
        sleep_ms(200);
        clear_balls(&disp, scale);

        for(int i = 0; i < n_updates; i++) {
            int exit_position = update_balls(n_lines); // from 0 to n_lines, n_lines + 1 options
    
            if(exit_position != -1) {
                add_to_histogram(exit_position);
            }
        }

        draw_histogram(&disp);
    }
}
