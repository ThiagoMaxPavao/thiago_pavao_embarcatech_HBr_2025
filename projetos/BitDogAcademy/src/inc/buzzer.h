#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define BUZZER_VOLUME 0.5f

// Inicializa pino GPIO em modo PWM
// Salva internamente o pino GPIO para uso posterior
void buzzer_init(uint pin);

// Inicia um beep bloqueante com a frequência e duração especificadas
void buzzer_beep(uint frequency, uint duration_ms);

// Inicia um beep não-bloqueante utilizando um alarme para parar o toque
void buzzer_start(uint frequency, uint duration_ms);

// Duplo beep - som de sucesso (não-bloqueante)
void success_sound();

// Som simples de sucesso, para ações mais frequentes (não bloqueante)
void simple_success_sound();

// Beep unico e longo - som de erro (não bloqueante)
void error_sound();

// Musica da tela de inicio - 3 notas (não bloqueante)
void start_song();

#endif // BUZZER_H
