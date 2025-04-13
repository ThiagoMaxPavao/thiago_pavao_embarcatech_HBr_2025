#include "neopixel.h"

static PIO np_pio;
static uint sm;
static npLED_t leds[LED_COUNT];

// Configuração da maquina de estados PIO para controle de LEDs WS2818B
void ws2818b_program_init(PIO pio, uint sm, uint offset, uint pin, float freq) {
    pio_gpio_init(pio, pin);

    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);

    // Program configuration.
    pio_sm_config c = ws2818b_program_get_default_config(offset);
    sm_config_set_sideset_pins(&c, pin); // Uses sideset pins.
    sm_config_set_out_shift(&c, true, true, 8); // 8 bit transfers, right-shift.
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX); // Use only TX FIFO.
    float prescaler = clock_get_hz(clk_sys) / (10.f * freq); // 10 cycles per transmission, freq is frequency of encoded bits.
    sm_config_set_clkdiv(&c, prescaler);

    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

void np_init(uint pin) {
    np_pio = pio0;
    uint offset = pio_add_program(np_pio, &ws2818b_program);
    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }
    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    np_clear();
    np_write();
}

// Inverte os bits de um byte
// Necessário porque a máquina PIO lê os bits na ordem contrária à necessária
uint8_t reverse_bits(uint8_t byte) {
    byte = (byte & 0xF0) >> 4 | (byte & 0x0F) << 4;
    byte = (byte & 0xCC) >> 2 | (byte & 0x33) << 2;
    byte = (byte & 0xAA) >> 1 | (byte & 0x55) << 1;
    return byte;
}

void np_set_led_rgb_index(uint index, uint8_t r, uint8_t g, uint8_t b) {
    if (index < LED_COUNT) {
        leds[index].R = reverse_bits(r * MAX_BRIGHTNESS / 255);
        leds[index].G = reverse_bits(g * MAX_BRIGHTNESS / 255);
        leds[index].B = reverse_bits(b * MAX_BRIGHTNESS / 255);
    }
}

void np_clear() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        np_set_led_rgb_index(i, 0, 0, 0);
    }
}

void np_write() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
    sleep_us(100);
}

// Retorna o índice do LED na matriz a partir das coordenadas x e y
int get_index(int x, int y) {
    return (y % 2 != 0) ? (y * 5 + x) : (y * 5 + (4 - x));
}

void np_set_led_rgb_xy(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
    int index = get_index(x, y);
    np_set_led_rgb_index(index, r, g, b);
}

// Para conversão entre HSL e RGB
void hue_to_rgb(double p, double q, double t, double *color) {
    if (t < 0) t += 1;
    if (t > 1) t -= 1;
    if (t < 1.0 / 6.0) *color = p + (q - p) * 6.0 * t;
    else if (t < 1.0 / 2.0) *color = q;
    else if (t < 2.0 / 3.0) *color = p + (q - p) * (2.0 / 3.0 - t) * 6.0;
    else *color = p;
}

void np_set_led_hsl_index(uint index, double h, double s, double l) {
    double r, g, b;
    if (s == 0) {
        r = g = b = l;
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        hue_to_rgb(p, q, h + 1.0 / 3.0, &r);
        hue_to_rgb(p, q, h, &g);
        hue_to_rgb(p, q, h - 1.0 / 3.0, &b);
    }
    np_set_led_rgb_index(index, round(r * 255), round(g * 255), round(b * 255));
}

void np_set_led_hsl_xy(uint8_t x, uint8_t y, double h, double s, double l) {
    int index = get_index(x, y);
    np_set_led_hsl_index(index, h, s, l);
}
