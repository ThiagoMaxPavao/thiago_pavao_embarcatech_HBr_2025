#include "histogram.h"

static uint histogram_values[MAX_BINS];
static int n_bins = 0;
static int max_value = 10;
static int total_balls = 0;

void init_histogram(int _n_bins) {
    for (int i = 0; i < MAX_BINS; i++) {
        histogram_values[i] = 0;
    }
    
    n_bins = _n_bins;
    max_value = n_bins;
    total_balls = 0;
}

void add_to_histogram(int value) {
    if (value < 0 || value >= n_bins) {
        return;
    }
    histogram_values[value]++;

    if(histogram_values[value] > max_value) {
        max_value = histogram_values[value];
    }

    total_balls++;
}

void draw_histogram(ssd1306_t *disp) {
    ssd1306_clear_square(disp, 64, 0, 64, 64);

    if (max_value == 0) {
        return;
    }

    for(int i = 0; i < n_bins; i++) {
        int bar_height = (histogram_values[i] * 64) / max_value;
        ssd1306_draw_square(disp, 64, 64*i/n_bins, bar_height, 64/n_bins);
    }
    
    char buffer[16];
    int len = snprintf(buffer, sizeof(buffer), "%d", total_balls);
    ssd1306_clear_square(disp, 0, 64-8-1, 6*len, 8);
    ssd1306_draw_string(disp, 0, 64-8-1, 1, buffer);
}
