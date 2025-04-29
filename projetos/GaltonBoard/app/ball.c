#include "ball.h"

ball_t balls_falling[MAX_BALLS];
int ball_falling_count = 0;
int ball_queued_count = MAX_BALLS;

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

void draw_ball(ssd1306_t *disp, int scale, int x, int y) {
    int y_offset = (disp->height - scale)/2;

    ssd1306_draw_empty_square(disp, x*scale, y*scale + y_offset, scale-1, scale-1);
}

void clear_ball(ssd1306_t *disp, int scale, int x, int y) {
    int y_offset = (disp->height - scale)/2;

    ssd1306_clear_square(disp, x*scale, y*scale + y_offset, scale, scale);
}

void draw_balls(ssd1306_t *disp, int scale) {
    for(int i = 0; i < ball_falling_count; i++) {
        draw_ball(disp, scale, balls_falling[i].x, balls_falling[i].y);
    }
}

void clear_balls(ssd1306_t *disp, int scale) {
    for(int i = 0; i < ball_falling_count; i++) {
        clear_ball(disp, scale, balls_falling[i].x, balls_falling[i].y);
    }
}

int update_balls(int n_lines) {
    static bool start_occupied = false;
    static int start_occupied_tick_count = 0;
    int exit_position = -1;

    if(balls_falling[0].x >= 2*n_lines) {
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
