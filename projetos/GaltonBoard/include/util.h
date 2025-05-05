#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "pico/stdlib.h"

int get_random_direction(int chance);
int64_t buzzer_off_alarm_callback(alarm_id_t id, void *user_data);
int update_joystick(absolute_time_t now);

#endif
