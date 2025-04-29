#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>

// Inicializa módulo ADC e pinos GPIO com a função de ADC
void joystick_init(uint X_pin, uint Y_pin);

// Lê valores dos pinos do joystick e converte para coordenadas X e Y
// ranges de -1 a 1
void joystick_get_XY(float *x, float *y);

// Lê valores dos pinos do joystick e converte para raio e ângulo
// range raio 0 a 1, ângulo -PI a PI
void joystick_get_RA(float *r, float *angle);

#endif // JOYSTICK_H
