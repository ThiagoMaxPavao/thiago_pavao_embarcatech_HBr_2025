// buttons.h
#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>
#include "pico/stdlib.h"

// Tempo para ignorar oscilações do sinal
#define DEBOUNCE_TIME 100

typedef void (*button_callback_t)(void);

// Inicializa os botões com pull up e configura as interrupções
void buttons_init(int button_A_pin_, int button_B_pin_, int button_joystick_pin_);

// Funções de configuração da função de callback para cada botão
void set_button_A_callback(button_callback_t callback);
void set_button_B_callback(button_callback_t callback);
void set_button_joystick_callback(button_callback_t callback);

#endif // BUTTONS_H
