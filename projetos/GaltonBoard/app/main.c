#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pico/stdlib.h"
#include "ssd1306.h"
#include "joystick.h"
#include "buttons.h"

#include "ball.h"
#include "histogram.h"

// --------------------------- Configuração ---------------------------

#define JOYSTICK_N_BINS 10
#define JOYSTICK_EXTEND_THRESHOLD 0.9f
#define JOYSTICK_RETRACT_THRESHOLD 0.8f

#define MAX_FPS 60

#define MAX_SIMULATION_UPDATE_FREQUENCY MAX_FPS*0.7
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

// Buttons
#define BUTTON_A 5
#define BUTTON_B 6
#define BUTTON_JOYSTICK 22

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

bool update_simulation_frequency(int update) {
    static float current_simulation_frequency = 2;
    bool changed = false;

    if(update == 1) { // want higher frequency
        if(current_simulation_frequency < MAX_SIMULATION_UPDATE_FREQUENCY) { // allowed to get higher
            current_simulation_frequency *= 1.3;
            changed = true;
        }
        else if(n_updates < 4) { 
            n_updates++;          // higher number of updates in each simulation tick
            changed = true;
        }
    }
    else if(update == -1) { // want lower frequency
        if(n_updates > 1) {
            n_updates--;
            changed = true;
        }
        else if(current_simulation_frequency > MIN_SIMULATION_UPDATE_FREQUENCY){
            current_simulation_frequency *= 0.7;
            changed = true;
        }
    }

    simulation_delay_tick_us = 1000000.0 / current_simulation_frequency;

    return changed;
}

// --------------------------- Joystick logic ---------------------------

int64_t buzzer_off_callback(alarm_id_t id, void *user_data) {
    gpio_put(BUZZER_PIN, false);
    return 0; // one-shot
}

int update_joystick(absolute_time_t now) {
    static bool extended = false;
    static int current_bin = -1, last_bin = -1;
    static absolute_time_t last_read_time = 0;

    float r, teta;
    int update = 0;

    if(absolute_time_diff_us(last_read_time, now) > 10000) {
        last_read_time = now;

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
    
                if (diff == 1) update = -1;
                else if (diff == JOYSTICK_N_BINS - 1) update = +1;
            }
    
            last_bin = current_bin;
        }
    }

    return update;
}

// --------------------------- Main ---------------------------

volatile bool do_reset_simulation = false;

void set_scale_and_n_lines(int new_scale) {
    scale = new_scale;
    
    switch(scale)
    {
    case 4:
        n_lines = 7;
        break;
    case 2:
        n_lines = 15;
        break;
    case 1:
        n_lines = 31;
        break;
    default:
        break;
    }
}

void scale_down() {
    if(scale == 4) {
        set_scale_and_n_lines(2);
    }
    else if(scale == 2) {
        set_scale_and_n_lines(1);
    }

    do_reset_simulation = true;
}

void scale_up() {
    if(scale == 2) {
        set_scale_and_n_lines(4);
    }
    else if(scale == 1) {
        set_scale_and_n_lines(2);
    }

    do_reset_simulation = true;
}

int main() {
    stdio_init_all();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    joystick_init(JOY_X_PIN, JOY_Y_PIN);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, true);

    ssd1306_clear(&disp);
    draw_board(&disp, scale, n_lines);

    buttons_init(BUTTON_A, BUTTON_B, BUTTON_JOYSTICK);

    set_button_A_callback(scale_down);
    set_button_B_callback(scale_up);

    init_histogram(n_lines + 1);


    absolute_time_t last_redraw_time = get_absolute_time();
    absolute_time_t last_simulation_tick_time = get_absolute_time();

    while (true) {
        absolute_time_t now = get_absolute_time();

        int update = update_joystick(now);

        bool frequency_changed = update_simulation_frequency(update);

        if(frequency_changed) { // ring buzzer
            gpio_put(BUZZER_PIN, true);
            add_alarm_in_ms(2, buzzer_off_callback, NULL, false);
        }

        if(redraw && absolute_time_diff_us(last_redraw_time, now) > 1000000.0 / MAX_FPS) {
            redraw = false;
            last_redraw_time = now;

            draw_balls(&disp, scale);
            draw_histogram(&disp);
            ssd1306_show(&disp);
            clear_balls(&disp, scale);
        }

        if(absolute_time_diff_us(last_simulation_tick_time, now) > simulation_delay_tick_us) {
            last_simulation_tick_time = now;

            for (int i = 0; i < n_updates; i++) {
                int exit_position = update_balls(n_lines); // from 0 to n_lines, n_lines + 1 options

                if (exit_position != -1) {
                    add_to_histogram(exit_position);
                }
            }

            // update display
            redraw = true;
        }

        if(do_reset_simulation) {
            do_reset_simulation = false;

            reset_balls();
            init_histogram(n_lines + 1);

            ssd1306_clear(&disp);
            draw_board(&disp, scale, n_lines);

            // update display
            redraw = true;
        }

    }
}
