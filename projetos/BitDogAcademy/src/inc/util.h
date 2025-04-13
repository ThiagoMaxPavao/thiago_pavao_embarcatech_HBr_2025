#ifndef UTIL_H
#define UTIL_H

#include <math.h>

// Calculo da diferença entre dois ângulos em radianos
// A diferença é sempre positiva e menor que PI
float angle_difference(float angle1, float angle2);

// Função para limitar um valor inteiro entre um intervalo
int clamp(int x, int lower, int upper);

// Função para limitar um valor de ponto flutuante entre um intervalo
float clampf(float x, float lower, float upper);

#endif // UTIL_H
