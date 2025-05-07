#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pico/stdlib.h"

// Retorna +1 ou -1, para a chance dada de +1 (parâmetro de 0 a 100)
int get_random_direction(int chance);

// Atualiza o estado do joystick, retorna -1 ou +1 dependendo da direção de rotação (horária ou anti-horária)
// Recebe o tempo atual como parâmetro para impedir uma taxa de atualização desnecessariamente alta
int update_joystick(absolute_time_t now);

// Callback para desligar o buzzer com um alarme
int64_t buzzer_off_alarm_callback(alarm_id_t id, void *user_data);

#endif
