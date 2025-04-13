#include "joystick.h"

void joystick_init(uint X_pin, uint Y_pin) {
    adc_init();
    adc_gpio_init(X_pin);
    adc_gpio_init(Y_pin);
}

void joystick_get_XY(float *x, float *y) {
    adc_select_input(0);
    uint adc_y_raw = adc_read();
    adc_select_input(1);
    uint adc_x_raw = adc_read();

    // Convert raw ADC values to joystick coordinates
    // X: 0 - 4095 -> -1 - 1
    // Y: 0 - 4095 -> -1 - 1
    *x = adc_x_raw / 2047.5 - 1;
    *y = adc_y_raw / 2047.5 - 1;
}

void joystick_get_RA(float *r, float *angle) {
    float x, y;
    joystick_get_XY(&x, &y);

    // Convert joystick coordinates to polar coordinates
    *r = sqrt(x * x + y * y);
    *angle = atan2(y, x);
}
