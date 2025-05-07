#ifndef _ball
#define _ball

#include "ssd1306.h"

#define MAX_BALLS 32

typedef struct {
    int x;
    int y;
    bool force_fall;
} ball_t;

// Funções de desenho no display OLED
void draw_balls(ssd1306_t *disp);
void clear_balls(ssd1306_t *disp);
void draw_board(ssd1306_t *disp);

// Executa um tick da simulação
void run_simulation_tick();

// Atualiza a frequência de simulação
bool update_simulation_frequency(int update);

// Mudança de escala de desenho, também reinicia a simulação
void scale_down();
void scale_up();

#endif
