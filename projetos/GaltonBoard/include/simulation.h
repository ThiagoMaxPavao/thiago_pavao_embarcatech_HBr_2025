#ifndef _ball
#define _ball

#include "ssd1306.h"

#define MAX_BALLS 32

typedef struct {
    int x;
    int y;
    bool force_fall;
} ball_t;

void draw_balls(ssd1306_t *disp);
void clear_balls(ssd1306_t *disp);
int update_balls();
void reset_balls();

void draw_board(ssd1306_t *disp);
bool update_simulation_frequency(int update);

void run_simulation_tick();

void scale_down();
void scale_up();

#endif
