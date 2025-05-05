#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pico/stdlib.h"
#include "ssd1306.h"
#include "joystick.h"
#include "buttons.h"

#include "simulation.h"
#include "histogram.h"
#include "util.h"

#include "config.h"

// --------------------------- Variáveis globais ---------------------------

ssd1306_t disp;
volatile bool redraw = false;
volatile bool redraw_board = false;
extern int64_t simulation_delay_tick_us;


// --------------------------- Main ---------------------------

int main() {
    stdio_init_all();

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    joystick_init(JOY_X_PIN, JOY_Y_PIN);

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, true);

    ssd1306_clear(&disp);
    draw_board(&disp);

    buttons_init(BUTTON_A, BUTTON_B, BUTTON_JOYSTICK);

    set_button_A_callback(scale_down);
    set_button_B_callback(scale_up);

    init_histogram(INITIAL_N_LINES + 1);


    absolute_time_t last_redraw_time = get_absolute_time();
    absolute_time_t last_simulation_tick_time = get_absolute_time();

    while (true) {
        absolute_time_t now = get_absolute_time();

        int update = update_joystick(now);

        bool frequency_changed = update_simulation_frequency(update);

        if(frequency_changed) { // ring buzzer
            gpio_put(BUZZER_PIN, true);
            add_alarm_in_ms(2, buzzer_off_alarm_callback, NULL, false);
        }

        if(redraw && absolute_time_diff_us(last_redraw_time, now) > 1000000.0 / MAX_FPS) {
            redraw = false;
            last_redraw_time = now;
            
            if(redraw_board) {
                redraw_board = false;
                ssd1306_clear(&disp);
                draw_board(&disp);
            }

            draw_balls(&disp);
            draw_histogram(&disp);
            ssd1306_show(&disp);
            clear_balls(&disp);
        }

        if(absolute_time_diff_us(last_simulation_tick_time, now) > simulation_delay_tick_us) {
            last_simulation_tick_time = now;

            run_simulation_tick();

            // update display
            redraw = true;
        }

    }
}
