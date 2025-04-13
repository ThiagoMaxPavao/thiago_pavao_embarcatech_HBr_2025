#include "buzzer.h"
#include "pico/time.h"

// Struct para definição de uma nota, contendo frequência e duração
// Possibilitando passagem de parâmetros pelo alarme.
typedef struct {
    uint frequency;
    uint duration_ms;
} note;

static uint pin;
static alarm_id_t buzzer_alarm_id = -1;

// Inicializa pino GPIO em modo PWM
// Salva internamente o pino GPIO para uso posterior
void buzzer_init(uint gpio_pin) {
    pin = gpio_pin;
    gpio_set_function(pin, GPIO_FUNC_PWM); // Set pin as PWM output
}

// Inicia um beep bloqueante com a frequência e duração especificadas
void buzzer_beep(uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin); // Get PWM slice for the pin

    // Configure PWM settings
    pwm_config config = pwm_get_default_config();
    float clock_div = (float)clock_get_hz(clk_sys) / (frequency * 4096);
    pwm_config_set_clkdiv(&config, clock_div);
    
    pwm_init(slice_num, &config, true); // Initialize PWM with config
    pwm_set_gpio_level(pin, BUZZER_VOLUME * 2048); // Set duty cycle to 50% * BUZZER_VOLUME

    sleep_ms(duration_ms); // Wait for duration

    pwm_set_gpio_level(pin, 0); // Turn off the buzzer
}

// Para o toque do buzzer
// Função chamada por um alarme
int64_t buzzer_stop(alarm_id_t id, void *user_data) {
    pwm_set_gpio_level(pin, 0);
    buzzer_alarm_id = -1;
    return 0; // Não repetir o alarme
}

// Inicia um beep não-bloqueante utilizando um alarme para parar o toque
void buzzer_start(uint frequency, uint duration_ms) {
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configure PWM settings
    pwm_config config = pwm_get_default_config();
    float clock_div = (float)clock_get_hz(clk_sys) / (frequency * 4096);
    pwm_config_set_clkdiv(&config, clock_div);

    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(pin, BUZZER_VOLUME * 2048); // Set duty cycle to 50% * BUZZER_VOLUME

    // Schedule alarm to stop the buzzer
    buzzer_alarm_id = add_alarm_in_ms(duration_ms, buzzer_stop, NULL, false);
}

// Função chamada por um alarme para iniciar um beep não-bloqueante
int64_t buzzer_start_alarm(alarm_id_t id, void *user_data) {
    note *n = (note *)user_data;
    buzzer_start(n->frequency, n->duration_ms);
    return 0;
}

// Duplo beep - som de sucesso (não-bloqueante)
void success_sound() {
    static note success_beep = {8000, 100};
    buzzer_start(success_beep.frequency, success_beep.duration_ms);
    add_alarm_in_ms(150, buzzer_start_alarm, &success_beep, false);
}

// Beep unico e longo - som de erro (não bloqueante)
void error_sound() {
    buzzer_start(3000, 150);
}

// Musica da tela de inicio - 3 notas (não bloqueante)
void start_song() {
    static note note1 = {4000, 100};
    static note note2 = {6000, 100};
    static note note3 = {10000, 200};

    buzzer_start(note1.frequency, note1.duration_ms);
    add_alarm_in_ms(150, buzzer_start_alarm, &note2, false);
    add_alarm_in_ms(300, buzzer_start_alarm, &note3, false);
}

// Som simples de sucesso, para ações mais frequentes (não bloqueante)
void simple_success_sound() {
    buzzer_start(8000, 100);
}
