#include "util.h"

#include "pico/rand.h"
#include "joystick.h"
#include "config.h"

// --------------------------- Random ---------------------------

// chance from 0 to 100
// returns +1 or -1, for the given chance of +1
int get_random_direction(int chance) {
    uint64_t randValue = get_rand_64();
    uint8_t randChance = (randValue % 100);
    if (randChance < chance) {
        return 1;
    } else {
        return -1;
    }
}

// --------------------------- Joystick logic ---------------------------

int update_joystick(absolute_time_t now) {
    static bool extended = false;
    static int current_bin = -1, last_bin = -1;
    static absolute_time_t last_read_time = 0;

    float r, teta;
    int update = 0;

    if(absolute_time_diff_us(last_read_time, now) > 10000) {
        last_read_time = now;

        joystick_get_RA(&r, &teta);

        // Update extension state
        if (r > JOYSTICK_EXTEND_THRESHOLD) {
            extended = true;
        } else if (r < JOYSTICK_RETRACT_THRESHOLD) {
            extended = false;
            last_bin = -1;
        }
    
        if (extended) {
            if (teta < 0) teta += 2 * M_PI;
    
            // Calculate bin index
            current_bin = (int)(teta * JOYSTICK_N_BINS / (2 * M_PI)) % JOYSTICK_N_BINS;
    
            if (last_bin != -1) {
                int diff = (current_bin - last_bin + JOYSTICK_N_BINS) % JOYSTICK_N_BINS;
    
                if (diff == 1) update = -1;
                else if (diff == JOYSTICK_N_BINS - 1) update = +1;
            }
    
            last_bin = current_bin;
        }
    }

    return update;
}

// --------------------------- Buzzer off alarm ---------------------------

int64_t buzzer_off_alarm_callback(alarm_id_t id, void *user_data) {
    gpio_put(BUZZER_PIN, false);
    return 0; // one-shot
}
