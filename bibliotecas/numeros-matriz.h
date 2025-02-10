#ifndef NUMEROS_H
#define NUMEROS_H

#include "controle-de-led.h"
#include "hardware/pio.h"

// Define o valor usado para limpar a matriz
#define CLEAN 11

// Variáveis globais definidas em main.c
extern PIO pio_global;
extern uint sm_global;
extern bool estado_led_azul;
extern bool estado_led_verde;
extern uint8_t valor;

#define TOTAL_LEDS 25 // Define o número total de LEDs na matriz

// Definição das cores dos LEDs
// Níveis de brilho para ativado e desativado
const double BRILHO_ATIVO = 0.3;
const double BRILHO_DESATIVADO = 0.0;

double color_red_ativa[TOTAL_LEDS] = {BRILHO_ATIVO};
double color_green_ativa[TOTAL_LEDS] = {BRILHO_ATIVO};
double color_blue_ativa[TOTAL_LEDS] = {BRILHO_ATIVO};

double color_red_desativa[TOTAL_LEDS] = {BRILHO_DESATIVADO};
double color_green_desativa[TOTAL_LEDS] = {BRILHO_DESATIVADO};
double color_blue_desativa[TOTAL_LEDS] = {BRILHO_DESATIVADO};

// Definição dos números em uma matriz 5x5
uint numeros[10][TOTAL_LEDS] = {
    {0,1,1,1,0, 0,1,0,1,0, 0,1,0,1,0, 0,1,0,1,0, 0,1,1,1,0}, // 0
    {0,0,1,0,0, 0,0,1,1,0, 1,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0}, // 1
    {0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0}, // 2
    {0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0}, // 3
    {0,1,0,1,0, 0,1,0,1,0, 0,1,1,1,0, 0,1,0,0,0, 0,0,0,1,0}, // 4
    {0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0}, // 5
    {0,1,1,1,0, 0,0,0,1,0, 0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0}, // 6
    {0,1,1,1,0, 0,1,0,0,0, 0,0,0,1,0, 0,1,0,0,0, 0,0,0,1,0}, // 7
    {0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0}, // 8
    {0,1,1,1,0, 0,1,0,1,0, 0,1,1,1,0, 0,1,0,0,0, 0,1,1,1,0}  // 9
};

// Exibe o número na matriz de LEDs
void exibir_numero(uint valor) {
    // Se o valor for CLEAN, limpa a matriz
    if (valor == CLEAN) {
        uint clear[TOTAL_LEDS] = {0};
        atualizar_matriz(clear, color_red_desativa, color_green_desativa, color_blue_desativa, pio_global, sm_global);
        return;
    }

    // Definição das cores de exibição com base no estado dos LEDs
    double *cor_verde = estado_led_verde ? color_green_ativa : color_green_desativa;
    double *cor_azul = estado_led_azul ? color_blue_ativa : color_blue_desativa;
    double *cor_vermelho = (!estado_led_azul && !estado_led_verde) ? color_red_ativa : color_red_desativa;

    // Atualiza a matriz de LEDs com o número correspondente
    if (valor < 10) {
        atualizar_matriz(numeros[valor], cor_vermelho, cor_verde, cor_azul, pio_global, sm_global);
    }
}

#endif
