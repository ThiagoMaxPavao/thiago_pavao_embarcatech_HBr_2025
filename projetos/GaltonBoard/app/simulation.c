#include "simulation.h"
#include "config.h"
#include "util.h"
#include "histogram.h"

static ball_t balls_falling[MAX_BALLS];
static int ball_falling_count = 0;
static int ball_queued_count = MAX_BALLS;

// board and drawing parameters

static int scale = INITIAL_SCALE;
static int n_lines = INITIAL_N_LINES;

// simulation tick parameters

int64_t simulation_delay_tick_us = INITIAL_SIMULATION_TICK_US;

// notify

extern volatile bool redraw;
extern volatile bool redraw_board;

// --------------------------- Display OLED ---------------------------

void draw_ball(ssd1306_t *disp, int x, int y) {
    int y_offset = (disp->height - scale)/2;

    ssd1306_draw_empty_square(disp, x*scale, y*scale + y_offset, scale-1, scale-1);
}

void clear_ball(ssd1306_t *disp, int x, int y) {
    int y_offset = (disp->height - scale)/2;

    ssd1306_clear_square(disp, x*scale, y*scale + y_offset, scale, scale);
}

void draw_balls(ssd1306_t *disp) {
    for(int i = 0; i < ball_falling_count; i++) {
        draw_ball(disp, balls_falling[i].x, balls_falling[i].y);
    }
}

void clear_balls(ssd1306_t *disp) {
    for(int i = 0; i < ball_falling_count; i++) {
        clear_ball(disp, balls_falling[i].x, balls_falling[i].y);
    }
}

void draw_board(ssd1306_t *disp) {
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

// --------------------------- Ball Update logic ---------------------------

int update_balls() {
    static bool start_occupied = false;
    static int start_occupied_tick_count = 0;
    int exit_position = -1;

    if(ball_falling_count > 0 && balls_falling[0].x >= 2*n_lines) {
        ball_queued_count++;
        ball_falling_count--;
        exit_position = (balls_falling[0].y + n_lines)/2;
        for(int i = 0; i < ball_falling_count; i++) {
            balls_falling[i] = balls_falling[i+1];
        }
    }

    for(int i = 0; i < ball_falling_count; i++) {

        if(balls_falling[i].x % 2 == 0 && !balls_falling[i].force_fall) {
            balls_falling[i].y += get_random_direction(50);
            balls_falling[i].force_fall = true;
        }
        else {
            balls_falling[i].x += 1;
            balls_falling[i].force_fall = false;
        }
    }

    if(start_occupied) {
        start_occupied_tick_count++;
        if(start_occupied_tick_count > 4) {
            start_occupied = false;
        }
    }

    if(!start_occupied && ball_queued_count > 0 && ball_falling_count < MAX_BALLS) {
        balls_falling[ball_falling_count].x = 0;
        balls_falling[ball_falling_count].y = 0;
        balls_falling[ball_falling_count].force_fall = false;
        ball_falling_count++;
        
        start_occupied = true;
        start_occupied_tick_count = 0;
        ball_queued_count--;
    }

    return exit_position;
}

void reset_balls() {
    ball_falling_count = 0;
    ball_queued_count = MAX_BALLS;
}

// --------------------------- Simulation Tick Callback ---------------------------

void run_simulation_tick() {
    // Atualiza posição das bolas no tabuleiro
    int exit_position = update_balls(n_lines);

    // Atualiza histograma se alguma bola tiver saído do tabuleiro
    if (exit_position != -1) {
        add_to_histogram(exit_position);
    }
}

// --------------------------- Update Simulation Frequency Logic ---------------------------

bool update_simulation_frequency(int update) {
    static float current_simulation_frequency = INITIAL_SIMULATION_UPDATE_FREQUENCY;
    bool changed = false;

    if(update == 1) { // want higher frequency
        if(current_simulation_frequency < MAX_SIMULATION_UPDATE_FREQUENCY) { // allowed to get higher
            current_simulation_frequency *= 1.7;
            changed = true;
        }
    }
    else if(update == -1) { // want lower frequency
        if(current_simulation_frequency > MIN_SIMULATION_UPDATE_FREQUENCY){
            current_simulation_frequency /= 1.7;
            changed = true;
        }
    }

    simulation_delay_tick_us = 1000000.0 / current_simulation_frequency;

    return changed;
}

// --------------------------- Scale update ---------------------------

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
    default:
        n_lines = 31;
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

    reset_balls();
    init_histogram(n_lines + 1);
    redraw_board = true;
    redraw = true;
}

void scale_up() {
    if(scale == 2) {
        set_scale_and_n_lines(4);
    }
    else if(scale == 1) {
        set_scale_and_n_lines(2);
    }

    reset_balls();
    init_histogram(n_lines + 1);
    redraw_board = true;
    redraw = true;
}
