#include "../includes/botoes.h"
#include "../includes/led_rgb.h"
#include "../includes/ssd1306.h"
#include <stdio.h>

// Estados dos LEDs
static bool estado_led_verde = false;
static bool estado_led_azul = false;
static volatile uint32_t ultimo_tempo_press = 0; // Para debounce

// Configura os botões e ativa a interrupção
void configurar_botoes(void) {
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &tratar_interrupcao_botao);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &tratar_interrupcao_botao);
}

// Função de debounce
bool debounce(uint32_t agora) {
    if (agora - ultimo_tempo_press < 200000) {
        return false;
    }
    ultimo_tempo_press = agora;
    return true;
}

// Atualiza o display OLED com o estado dos LEDs
void atualiza_display_oled(ssd1306_t* ssd, const char* linha1, const char* linha2) {
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, 3, 3, 122, 58, true, false);
    ssd1306_draw_string(ssd, linha1, 15, 10);
    ssd1306_draw_string(ssd, linha2, 30, 30);
    ssd1306_send_data(ssd);
}

// Função de interrupção dos botões
void tratar_interrupcao_botao(uint gpio, uint32_t eventos) {
    extern ssd1306_t ssd; // Permite acesso ao display global

    uint32_t agora = time_us_32();
    if (!debounce(agora)) return;

    if (gpio == BOTAO_A) { // Se pressionou o botão A
        estado_led_verde = !estado_led_verde;
        if (estado_led_verde) estado_led_azul = false;
    } else if (gpio == BOTAO_B) { // Se pressionou o botão B
        estado_led_azul = !estado_led_azul;
        if (estado_led_azul) estado_led_verde = false;
    }

    // Atualiza o LED RGB com os novos estados
    set_led_rgb(false, estado_led_verde, estado_led_azul);

    // Exibe mensagem no Serial Monitor
    printf("Botão %c pressionado -> LED Verde: %s | LED Azul: %s\n",
        (gpio == BOTAO_A) ? 'A' : 'B',
        estado_led_verde ? "LIGADO" : "DESLIGADO",
        estado_led_azul ? "LIGADO" : "DESLIGADO"
    );

    // Atualiza o display OLED
    if (estado_led_verde) {
        atualiza_display_oled(&ssd, "LED Verde", "LIGADO");
    } else if (estado_led_azul) {
        atualiza_display_oled(&ssd, "LED Azul", "LIGADO");
    } else {
        atualiza_display_oled(&ssd, "Todos LEDs", "DESLIGADOS");
    }
}