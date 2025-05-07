#ifndef _histogram
#define _histogram

#include <stdio.h>
#include "ssd1306.h"

#define MAX_BINS 33

void init_histogram(int _n_bins);
void add_to_histogram(int value);
void draw_histogram(ssd1306_t *disp);

#endif
