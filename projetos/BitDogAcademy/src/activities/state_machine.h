#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

// Biblioteca padrão e da SDK
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include <math.h>

// Biblioteca para o display OLED
#include "ssd1306.h"

// Fontes
#include "acme_5_outlines_font.h"
#include "bubblesstandard_font.h"
#include "BMSPA_font.h"

// Biblioteca para o Neopixel - Matrix de LEDs RGB
#include "neopixel.h"

// Biblioteca para o Buzzer
#include "buzzer.h"

// Biblioteca para o Joystick
#include "joystick.h"

// Biblioteca para os botões
#include "buttons.h"

// Funcoes da atividade
#include "tests.h"

// Funcoes da atividade de matemática - divisão inteira
#include "math_integer_division.h"

// Maquina de estados
typedef enum {
    STATE_INIT,
    STATE_RUN_TESTS,
    STATE_START_SCREEN,
    STATE_START_SCREEN_WAIT,
    STATE_SHOW_USAGE_TUTORIAL,
    STATE_USAGE_TUTORIAL_WAIT,
    STATE_DRAW_SELECT_SUBJECT,
    STATE_SELECT_SUBJECT_WAIT,
    STATE_DRAW_SELECT_ACTIVITY,
    STATE_SELECT_ACTIVITY_WAIT,
    STATE_DRAW_ACTIVITY_TUTORIAL_PAGE,
    STATE_ACTIVITY_TUTORIAL_PAGE_WAIT,
    STATE_ACTIVITY_SETUP,
    STATE_ACTIVITY_LOOP
} state_t;

// Funcao de animacao da tela inicial
void state_start_screen_wait(bool reset);

// Executa a máquina de estados
void run_state_machine();

#endif // STATE_MACHINE_H
