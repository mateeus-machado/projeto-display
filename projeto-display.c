#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "./includes/ssd1306.h"
#include "./includes/fonte.h"
#include "./includes/matriz_led.h"
#include "./includes/animacao.h"
#include "./includes/led_rgb.h"
#include "./includes/botoes.h"
#include "projeto_display.pio.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_OLED 0x3C
#define BUFFER_SIZE 20  // Limite máximo da frase no display

// Define o display como global para ser acessado pelos botões
ssd1306_t ssd;

void inicializa_i2c() {
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void inicializa_display() {
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO_OLED, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_send_data(&ssd);
}

void inicializa_matriz_led(PIO pio, uint sm, uint offset) {
    set_sys_clock_khz(128000, false);
    projeto_display_program_init(pio, sm, offset, PINO_LEDS);
    sleep_ms(100);
}

void atualiza_display(const char* texto) {
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_draw_string(&ssd, texto, 8, 10);
    ssd1306_send_data(&ssd);
}

int main() {
    stdio_init_all();

    inicializa_i2c();
    inicializa_display();

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &projeto_display_program);
    uint sm = pio_claim_unused_sm(pio, true);
    inicializa_matriz_led(pio, sm, offset);

    configurar_botoes();
    configurar_led_rgb();

    void (*animacao[]) (PIO, uint) = {
        animacao_numero_zero,
        animacao_numero_um,
        animacao_numero_dois,
        animacao_numero_tres,
        animacao_numero_quatro,
        animacao_numero_cinco,
        animacao_numero_seis,
        animacao_numero_sete,
        animacao_numero_oito,
        animacao_numero_nove
    };

    printf("Sistema pronto. Digite caracteres no Serial Monitor.\n");

    char frase[BUFFER_SIZE + 1] = "";
    int posicao = 0;

    while (true) {
        int c = getchar_timeout_us(100000);
        if (c == PICO_ERROR_TIMEOUT) continue;

        if (c >= '0' && c <= '9') {
            int num = c - '0';
            printf("Exibindo número %d na Matriz de LEDs\n", num);

            char num_str[2] = {c, '\0'};
            char texto[BUFFER_SIZE + 10];
            snprintf(texto, sizeof(texto), "Numero %s", num_str);
            atualiza_display(texto);

            animacao[num](pio, sm);
            posicao = 0;
            frase[0] = '\0';
        } else if (c == '\b' || c == 127) {
            if (posicao > 0) {
                posicao--;
                frase[posicao] = '\0';
                printf("\rFrase atual: %s   ", frase);
                fflush(stdout);
                atualiza_display(frase);
            }
        } else if (c >= 32 && c <= 126 && posicao < BUFFER_SIZE) {
            frase[posicao++] = c;
            frase[posicao] = '\0';
            printf("\rFrase atual: %s ", frase);
            fflush(stdout);
            atualiza_display(frase);
        }
    }
}