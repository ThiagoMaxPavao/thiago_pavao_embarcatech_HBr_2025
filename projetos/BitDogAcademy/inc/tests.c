#include "tests.h"

// Fontes para o teste de display
#include "acme_5_outlines_font.h"
#include "bubblesstandard_font.h"
#include "crackers_font.h"
#include "BMSPA_font.h"

void test_ssd1306(ssd1306_t *disp) {
    const uint8_t *fonts[4]= {acme_font, bubblesstandard_font, crackers_font, BMSPA_font};
    char buffer[20]; // Buffer para a string formatada

    for(int i = 0; i < 5; i++) {
        ssd1306_clear(disp);

        sprintf(buffer, "%d:Teste", i);

        if(i == 0)
            ssd1306_draw_string(disp, 8, 24, 2, buffer);
        else
            ssd1306_draw_string_with_font(disp, 8, 24, 2, fonts[i-1], buffer);

        ssd1306_show(disp);
        sleep_ms(2000);
    }

    ssd1306_clear(disp);
    ssd1306_draw_string(disp, 8, 24, 2, "FIM");
    ssd1306_show(disp);
}

void test_np() {
    uint a = 0;

    while(a < 360*2) {
        for(uint x = 0; x < 5; x++) {
            for(uint y = 0; y < 5; y++) {
                int shift = 25*(x-y);
                np_set_led_hsl_xy(x, y, ((a+shift) % 360) / 360.0, 1, 0.5);
                np_write();
            }
        }

        a += 10;
        sleep_ms(30);
    }

    np_clear();
    np_write();
}

void test_buzzer() {
    buzzer_beep(8000, 100);
    sleep_ms(50);
    buzzer_beep(8000, 100);
    sleep_ms(150);
    buzzer_beep(8000, 100);
    sleep_ms(50);
    buzzer_beep(10000, 100);
}

void test_joystick() {
    float x, y, r, angle;

    while(1) {
        joystick_get_XY(&x, &y);
        joystick_get_RA(&r, &angle);

        printf("X: %.2f, Y: %.2f, R: %.2f, angle: %.2f\n", x, y, r, angle);
        sleep_ms(100);
    }
}

void print_button_A_pressed() {
    printf("Button A pressed\n");
}

void print_button_B_pressed() {
    printf("Button B pressed\n");
}

void print_button_joystick_pressed() {
    printf("Button joystick pressed\n");
}

void test_buttons() {
    printf("Attaching callback function for each button, waiting for 10s\n");

    set_button_A_callback(print_button_A_pressed);
    set_button_B_callback(print_button_B_pressed);
    set_button_joystick_callback(print_button_joystick_pressed);

    sleep_ms(10000);

    printf("End! Detaching callback function for each button\n");

    set_button_A_callback(NULL);
    set_button_B_callback(NULL);
    set_button_joystick_callback(NULL);
}
