#include "test.h"
#include "unity.h"
#include "convert_temperature.h"

void test_adc_to_celsius(void) {
    // Teste com um valor ADC conhecido
    uint16_t adc_val = 876;
    float expected_temperature = 27.0f; // Temperatura esperada
    float result = adc_to_celsius(adc_val);
    
    // Verifica se o resultado está dentro de uma margem de erro aceitável
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected_temperature, result);
}

int run_all_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius);
    return UNITY_END();
}
