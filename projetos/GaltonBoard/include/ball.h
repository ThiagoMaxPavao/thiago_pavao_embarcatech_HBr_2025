#ifndef _ball
#define _ball

#include "pico/rand.h"
#include "ssd1306.h"

#define MAX_BALLS 32

typedef struct {
    int x;
    int y;
    bool force_fall;
} ball_t;

void draw_balls(ssd1306_t *disp, int scale);
void clear_balls(ssd1306_t *disp, int scale);
int update_balls(int n_lines);
void reset_balls();

#endif
