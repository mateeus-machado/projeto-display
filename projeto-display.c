#include <stdio.h>
#include <math.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "bibliotecas/ssd1306.h"
#include "bibliotecas/fonte.h"
#include "bibliotecas/matriz_led.h"
#include "bibliotecas/animacao.h"
#include "bibliotecas/led_rgb.h"
#include "bibliotecas/botoes.h"
#include "projeto-display.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO_OLED 0x3C
#define BUFFER_SIZE 20  // Limite máximo da frase no display

// Define o display como global para ser acessado pelos botões
ssd1306_t ssd;

int main() {
    stdio_init_all();

    // Inicializa o I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display SSD1306
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO_OLED, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
    ssd1306_send_data(&ssd);

    // Inicializa a Matriz de LEDs WS2812
    PIO pio = pio0;
    set_sys_clock_khz(128000, false);
    uint offset = pio_add_program(pio, &comunicacao_serial_com_rp2040_program);
    uint sm = pio_claim_unused_sm(pio, true);
    comunicacao_serial_com_rp2040_program_init(pio, sm, offset, PINO_LEDS);
    sleep_ms(100);

    // Configuração dos botões e LEDs
    configurar_botoes();
    configurar_led_rgb();

    // Array de funções de animação
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

            ssd1306_fill(&ssd, false);
            ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
            ssd1306_draw_string(&ssd, "Numero ", 10, 10);
            char num_str[2] = {c, '\0'};
            ssd1306_draw_string(&ssd, num_str, 60, 10);
            ssd1306_send_data(&ssd);

            animacao[num](pio, sm);
            posicao = 0;
            frase[0] = '\0';
        } else if (c == '\b' || c == 127) {  
            if (posicao > 0) {
                posicao--;
                frase[posicao] = '\0';
                printf("\rFrase atual: %s   ", frase);
                fflush(stdout);

                ssd1306_fill(&ssd, false);
                ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
                ssd1306_draw_string(&ssd, frase, 8, 10);
                ssd1306_send_data(&ssd);
            }
        } else if (c >= 32 && c <= 126 && posicao < BUFFER_SIZE) {
            frase[posicao++] = c;
            frase[posicao] = '\0';
            
            printf("\rFrase atual: %s ", frase);
            fflush(stdout);

            ssd1306_fill(&ssd, false);
            ssd1306_rect(&ssd, 3, 3, 122, 58, true, false);
            ssd1306_draw_string(&ssd, frase, 8, 10);
            ssd1306_send_data(&ssd);
        }
    }
}
