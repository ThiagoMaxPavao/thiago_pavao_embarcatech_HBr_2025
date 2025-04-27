#include "convert_temperature.h"

// Função para converter o valor lido do ADC para temperatura em graus Celsius
float adc_to_celsius(uint16_t adc_val) {
    const float conversion_factor = 3.3f / (1 << 12);           // Conversão de 12 bits (0-4095) para 0-3.3V
    float voltage = adc_val * conversion_factor;              // Converte o valor ADC para tensão
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f; // Equação fornecida para conversão
    return temperature;
}
