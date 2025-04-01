#ifndef MATH_INTEGER_DIVISION_ACTIVITY_H
#define MATH_INTEGER_DIVISION_ACTIVITY_H

#include "ssd1306.h"
#include <math.h>
#include "util.h"

// Bibliotecas para os periféricos
#include "ssd1306.h"
#include "neopixel.h"
#include "buzzer.h"
#include "joystick.h"
#include "buttons.h"

// Fonte para o título de introdução da atividade
#include "acme_5_outlines_font.h"

// Configura estado inicial da atividade e funcoes de callback nos botoes
void math_integer_division_activity_setup();

// Atualiza o estado da atividade de acordo com o input do usuário
// Retorna true se a atividade terminou, e false caso a funcao ainda deva ser chamada em loop
bool math_integer_division_activity_loop();

// Desenha uma pagina de tutorial da atividade
// Retorna true se a pagina existe, e false caso a pagina nao exista, indicando o fim do tutorial
bool math_integer_division_draw_tutorial_page(int page_number);

#endif // MATH_INTEGER_DIVISION_ACTIVITY_H
