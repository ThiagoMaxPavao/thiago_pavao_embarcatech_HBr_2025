#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <stdint.h>

// Bibliotecas para os periféricos
#include "ssd1306.h"
#include "neopixel.h"
#include "buzzer.h"
#include "joystick.h"
#include "buttons.h"

// Função de teste do display OLED
void test_ssd1306(ssd1306_t *disp);

// Função de teste da matriz de LEDs 5x5
void test_np();

// Função de teste do buzzer
void test_buzzer();

// Função de teste do joystick (imprime no terminal a leitura)
void test_joystick();

// Função de teste dos botões (imprime no terminal os pressionamentos detectados)
void test_buttons();

#endif // TESTS_H
