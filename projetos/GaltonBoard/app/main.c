#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pico/stdlib.h"
#include "ssd1306.h"
#include "joystick.h"

#include "ball.h"
#include "histogram.h"

// --------------------------- Configuração ---------------------------

#define JOYSTICK_N_BINS 10
#define JOYSTICK_EXTEND_THRESHOLD 0.9f
#define JOYSTICK_RETRACT_THRESHOLD 0.8f

#define MAX_FPS 60

#define MAX_SIMULATION_UPDATE_FREQUENCY MAX_FPS/2
#define MIN_SIMULATION_UPDATE_FREQUENCY 1

// --------------------------- Pinagem ---------------------------

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// Joystick
#define JOY_X_PIN 26
#define JOY_Y_PIN 27

// Buzzer
#define BUZZER_PIN 21

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

// --------------------------- Simulation Tick Callback ---------------------------

ssd1306_t disp;
volatile bool redraw = false;
int scale = 4;
int n_lines = 7;
int n_updates = 1;

int64_t simulation_delay_tick_us = 1<<19;

bool simulation_tick_callback(repeating_timer_t *rt) {
    clear_balls(&disp, scale);

    for (int i = 0; i < n_updates; i++) {
        int exit_position = update_balls(n_lines); // from 0 to n_lines, n_lines + 1 options

        if (exit_position != -1) {
            add_to_histogram(exit_position);
        }
    }

    draw_balls(&disp, scale);
    draw_histogram(&disp);

    // update display
    redraw = true;

    // update time
    rt->delay_us = simulation_delay_tick_us;

    return true; // Keep the timer running
}

// --------------------------- Joystick logic ---------------------------

int64_t buzzer_off_callback(alarm_id_t id, void *user_data) {
    gpio_put(BUZZER_PIN, false);
    return 0; // one-shot
}

int update_joystick() {
    static bool extended = false;
    static int current_bin = -1, last_bin = -1;

    float r, teta;
    int update = 0;

    joystick_get_RA(&r, &teta);

    // Update extension state
    if (r > JOYSTICK_EXTEND_THRESHOLD) {
        extended = true;
    } else if (r < JOYSTICK_RETRACT_THRESHOLD) {
        extended = false;
        last_bin = -1;
    }

    if (extended) {
        if (teta < 0) teta += 2 * M_PI;

        // Calculate bin index
        current_bin = (int)(teta * JOYSTICK_N_BINS / (2 * M_PI)) % JOYSTICK_N_BINS;

        if (last_bin != -1) {
            int diff = (current_bin - last_bin + JOYSTICK_N_BINS) % JOYSTICK_N_BINS;

            if(diff != 0) {
                gpio_put(BUZZER_PIN, true);
                add_alarm_in_ms(2, buzzer_off_callback, NULL, false);
            }

            if (diff == 1) update = -1;
            else if (diff == JOYSTICK_N_BINS - 1) update = +1;
        }

        last_bin = current_bin;
    }

    return update;
}

// --------------------------- Main ---------------------------


int main() {
    stdio_init_all();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    joystick_init(JOY_X_PIN, JOY_Y_PIN);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, true);

    ssd1306_clear(&disp);
    draw_board(&disp, scale, n_lines);

    init_histogram(n_lines + 1);

    repeating_timer_t timer;
    add_repeating_timer_ms(1000, simulation_tick_callback, NULL, &timer);

    absolute_time_t last_redraw_time = get_absolute_time();

    float current_simulation_frequency = 2;


    while (true) {
        absolute_time_t now = get_absolute_time();

        int update = update_joystick();

        if(update == 1 && current_simulation_frequency < MAX_SIMULATION_UPDATE_FREQUENCY) {
            current_simulation_frequency *= 1.3;
        }
        else if(update == -1 && current_simulation_frequency > MIN_SIMULATION_UPDATE_FREQUENCY) {
            current_simulation_frequency *= 0.7;
        }

        simulation_delay_tick_us = 1000000.0 / current_simulation_frequency;

        if(redraw && absolute_time_diff_us(last_redraw_time, now) > 1000000.0 / MAX_FPS) {
            redraw = false;
            last_redraw_time = now;
            ssd1306_show(&disp);
        }

        sleep_ms(5);
    }
}
