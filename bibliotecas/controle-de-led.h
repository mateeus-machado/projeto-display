#ifndef CONTROLE_LEDS_H
#define CONTROLE_LEDS_H

#include "hardware/pio.h"
#include "pico/stdlib.h"

#define TOTAL_LEDS 25

// Converte valores RGB (0 a 1) em um valor de 24 bits
static inline uint32_t get_color(double r, double g, double b) {
    uint8_t R = (uint8_t)(r * 255);
    uint8_t G = (uint8_t)(g * 255);
    uint8_t B = (uint8_t)(b * 255);
    return (G << 24) | (R << 16) | (B << 8);
}

// Atualiza a matriz de LEDs WS2812 com base nos dados fornecidos
static inline void atualizar_matriz(uint *art, double *red, double *green, double *blue, PIO pio, uint sm) {
    for (int i = 0; i < TOTAL_LEDS; i++) {
        uint32_t valor_led = get_color(red[TOTAL_LEDS - 1 - i], green[TOTAL_LEDS - 1 - i], blue[TOTAL_LEDS - 1 - i]);
        pio_sm_put_blocking(pio, sm, art[TOTAL_LEDS - 1 - i] ? valor_led : 0);
    }
}

// Exibe um número na matriz chamando exibir_numero (definida em numeros.h)
static inline void print_number(uint numero, PIO pio, uint sm) {
    extern void exibir_numero(uint numero);
    exibir_numero(numero);
}

#endif  // CONTROLE_LEDS_H
