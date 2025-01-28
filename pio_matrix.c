#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

// Definições de pinos
#define ROWS 4
#define COLS 4
#define NUM_LEDS 25

// Mapear as teclas do teclado matricial
char keypad[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Matrizes para cada frame da animação
const uint32_t frames[5][NUM_LEDS] = {
    {0xFF0000, 0x000000, 0x000000, 0x000000, 0xFF0000,  // Vermelho
     0x000000, 0xFF0000, 0x000000, 0xFF0000, 0x000000,
     0x000000, 0x000000, 0xFF0000, 0x000000, 0x000000,
     0x000000, 0xFF0000, 0x000000, 0xFF0000, 0x000000,
     0xFF0000, 0x000000, 0x000000, 0x000000, 0xFF0000},

    {0x00FF00, 0x000000, 0x000000, 0x000000, 0x00FF00,  // Verde
     0x000000, 0x00FF00, 0x000000, 0x00FF00, 0x000000,
     0x000000, 0x000000, 0x00FF00, 0x000000, 0x000000,
     0x000000, 0x00FF00, 0x000000, 0x00FF00, 0x000000,
     0x00FF00, 0x000000, 0x000000, 0x000000, 0x00FF00},

    {0x0000FF, 0x000000, 0x000000, 0x000000, 0x0000FF,  // Azul
     0x000000, 0x0000FF, 0x000000, 0x0000FF, 0x000000,
     0x000000, 0x000000, 0x0000FF, 0x000000, 0x000000,
     0x000000, 0x0000FF, 0x000000, 0x0000FF, 0x000000,
     0x0000FF, 0x000000, 0x000000, 0x000000, 0x0000FF},

    {0xFFFFFF, 0x000000, 0x000000, 0x000000, 0xFFFFFF,  // Branco
     0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF, 0x000000,
     0x000000, 0x000000, 0xFFFFFF, 0x000000, 0x000000,
     0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF, 0x000000,
     0xFFFFFF, 0x000000, 0x000000, 0x000000, 0xFFFFFF},

    {0xFFFF00, 0x000000, 0x000000, 0x000000, 0xFFFF00,  // Amarelo
     0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000,
     0x000000, 0x000000, 0xFFFF00, 0x000000, 0x000000,
     0x000000, 0xFFFF00, 0x000000, 0xFFFF00, 0x000000,
     0xFFFF00, 0x000000, 0x000000, 0x000000, 0xFFFF00}
};

// Função para inicializar os LEDs
void init_leds() {
    // Inicializar os pinos de controle dos LEDs
}

// Função para exibir um frame
void display_frame(const uint32_t *frame) {
    for (int i = 0; i < NUM_LEDS; i++) {
        // Exibir cor no LED correspondente
    }
}

// Função principal da animação
void play_animation() {
    for (int frame = 0; frame < 5; frame++) {
        display_frame(frames[frame]);
        sleep_ms(500);  // Aguardar 500ms entre os frames
    }
}

// Função principal
int main() {
    stdio_init_all();
    init_leds();

    while (true) {
        // Ler a tecla pressionada no teclado matricial
        char key = read_keypad();
        
        switch (key) {
            case '0':  // Animação ao pressionar a tecla '0'
                play_animation();
                break;
            case 'A':  // Desligar todos os LEDs
                clear_leds();
                break;
            case 'B':  // Ligar LEDs em azul
                set_all_leds(0x0000FF, 100);
                break;
            case 'C':  // Ligar LEDs em vermelho (80%)
                set_all_leds(0xFF0000, 80);
                break;
            case 'D':  // Ligar LEDs em verde (50%)
                set_all_leds(0x00FF00, 50);
                break;
            case '#':  // Ligar LEDs em branco (20%)
                set_all_leds(0xFFFFFF, 20);
                break;
            case '*':  // Modo de gravação
                reboot_device();
                break;
            default:
                break;
        }
    }
    return 0;
}
