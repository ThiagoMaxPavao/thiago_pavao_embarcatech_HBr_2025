#include <stdio.h>
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "hardware/adc.h"

// --------------------------- Pinagem ---------------------------

// Canal do ADC para o sensor de temperatura interno
#define ADC_TEMPERATURE_CHANNEL 4

// I2C display OLED
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15

// --------------------------- Display OLED ---------------------------

ssd1306_t disp;

// Desenha temperatura no centro do display com símbolo de graus Celsius
void draw_temp(float celsius) {
    char buffer[32];

    ssd1306_clear(&disp);
    snprintf(buffer, sizeof(buffer), "%.2f\x7F""C", celsius);
    ssd1306_draw_string_by_center(&disp, 64, 32, 2, buffer);
    ssd1306_show(&disp);
}

// --------------------------- Conversão de Temperatura ---------------------------

// Função para converter o valor lido do ADC para temperatura em graus Celsius
float adc_to_temperature(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);           // Conversão de 12 bits (0-4095) para 0-3.3V
    float voltage = adc_value * conversion_factor;              // Converte o valor ADC para tensão
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f; // Equação fornecida para conversão
    return temperature;
}

// --------------------------- Lógica Principal ---------------------------

int main() {
    // Inicializa os periféricos
    stdio_init_all();

    adc_init();

    // Seleciona o canal 4 do ADC (sensor de temperatura interno)
    adc_set_temp_sensor_enabled(true);          // Habilita o sensor de temperatura interno
    adc_select_input(ADC_TEMPERATURE_CHANNEL);  // Seleciona o canal do sensor de temperatura

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, I2C_PORT, I2C_SDA, I2C_SCL);

    // Loop principal Lê o canal selecionado do ADC, converte para celsius e exibe no display

    while (true) {
        uint16_t adc_value = adc_read();
        float temperature = adc_to_temperature(adc_value);
        draw_temp(temperature);

        sleep_ms(1000);
    }
}
