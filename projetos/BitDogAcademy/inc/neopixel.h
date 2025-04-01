#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdio.h>
#include <stdint.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include <math.h>
#include "pico/stdlib.h"

#define LED_COUNT 25 // Número de LEDs em sequência
#define MAX_BRIGHTNESS 30 // Brilho máximo dos LEDs, valor aplicado quando configura-se 255

// RGB values range from 0 to 255
// HSL values: Hue (0.0 - 1.0), Saturation (0.0 - 1.0), Lightness (0.0 - 1.0)

typedef struct {
    uint8_t G, R, B;
} npLED_t;

// Inicialização da maquina de estados PIO e da matriz apagada
void np_init(uint pin);

// Limpa a matriz de LEDs, apagando todos os LEDs
void np_clear();

// Envia as cores de todos os LEDs para a matriz
// utilizando PIO com o protocolo adequado implementado
void np_write();

// Configura um LED da matriz dado seu indice e uma cor RGB
void np_set_led_rgb_index(uint index, uint8_t r, uint8_t g, uint8_t b);

// Configura a cor de um LED da matriz a partir das coordenadas x e y e a cor em RGB
void np_set_led_rgb_xy(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);

// Configura a cor de um LED da matriz a partir do indice do LED e a cor em HSL
void np_set_led_hsl_index(uint index, double h, double s, double l);

// Configura a cor de um LED da matriz a partir das coordendas x e y e a cor em HSL
void np_set_led_hsl_xy(uint8_t x, uint8_t y, double h, double s, double l);

#endif // NEOPIXEL_H