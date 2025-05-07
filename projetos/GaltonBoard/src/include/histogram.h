#ifndef _histogram
#define _histogram

#include <stdio.h>
#include "ssd1306.h"

#define MAX_BINS 33

// Inicializa variáveis necessárias para geração do histograma
void init_histogram(int _n_bins);

// Adiciona um valor ao histograma, atualiza o número total de bolas
void add_to_histogram(int value);

// Desenha o histograma na metade direita da tela e o número total de bolas
void draw_histogram(ssd1306_t *disp);

#endif
