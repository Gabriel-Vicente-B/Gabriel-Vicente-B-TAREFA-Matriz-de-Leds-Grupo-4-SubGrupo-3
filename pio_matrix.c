#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"

#define row 4
#define column 4
#define NUM_PIXELS 25
#define OUT_PIN 7
#define pino_buzzer 21


const uint rowPin[row] = {8, 9, 6, 5};
const uint columnPin[column] = {4, 3, 2, 1};

const char TecladoMatricial[row][column] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

void inicializar_pinos();
char ler_teclado_matricial();
void animacao_1();
void animacao_2();
void animacao_3();
void animacao_4();
void animacao_5();
void animacao_6();
void animacao_7();
void desligar_leds();
void all_led_azul_100();
void all_led_vermelho_80();
void all_led_verde_50();
void all_led_branco_20();
void Resetar();

void imprimir_binario(int num)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        (num & (1 << i)) ? printf("1") : printf("0");
    }
}

uint32_t matrix_rgb(double r, double b, double g)
{
    unsigned char R, G, B;
    R = r * 255;
    G = g * 255;
    B = b * 255;
    return (G << 24) | (R << 16) | (B << 8);
}

int main()
{
    int x;
    PIO pio = pio0;
    bool ok;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;

    inicializar_pinos();
    stdio_init_all();

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);

    pio_matrix_program_init(pio, sm, offset, OUT_PIN);
    while (true)
    {
        char tecla = ler_teclado_matricial();
        switch (tecla)
        {
        case '1':
            animacao_1(valor_led, pio, sm, r, g, b);
            break;
        case '2':
            animacao_2(valor_led, pio, sm);
            break;
        case '3':
            animacao_3(valor_led, pio, sm, r, g, b);
            break;
        case '4':
            animacao_4(valor_led, pio, sm, r, g, b);
            break;
        case '5':
            animacao_5(valor_led, pio, sm, r, g, b);
            break;
        case '6':
            animacao_6(valor_led, pio, sm);
            break;
        case '7':
            animacao_7(valor_led, pio, sm, r, g, b);
            break;
        case 'A':
            desligar_leds(valor_led, pio, sm, r, g, b);
            break;
        case 'B':
            all_led_azul_100(valor_led, pio, sm, r, g, b);
            break;
        case 'C':
            all_led_vermelho_80(valor_led, pio, sm, r, g, b);
            break;
        case 'D':
            all_led_verde_50(valor_led, pio, sm, r, g, b);
            break;
        case '#':
            all_led_branco_20(valor_led, pio, sm, r, g, b);
            break;
        case '*':
            Resetar();
            break;
        default:
            break;
        }
    }
}

void inicializar_pinos()
{
    for (int i = 0; i < row; i++)
    {
        gpio_init(rowPin[i]);
        gpio_set_dir(rowPin[i], GPIO_OUT);
        gpio_put(rowPin[i], 0);
    }
    for (int j = 0; j < column; j++)
    {
        gpio_init(columnPin[j]);
        gpio_set_dir(columnPin[j], GPIO_IN);
        gpio_pull_up(columnPin[j]);
    }
    gpio_init(pino_buzzer);
    gpio_set_dir(pino_buzzer, GPIO_OUT);
    gpio_put(pino_buzzer, 0);
}

char ler_teclado_matricial()
{
    for (int i = 0; i < row; i++)
    {
        gpio_put(rowPin[i], 0);
        for (int j = 0; j < column; j++)
        {
            if (!gpio_get(columnPin[j]))
            {
                sleep_ms(50);
                if (!gpio_get(columnPin[j]))
                {
                    while (!gpio_get(columnPin[j]))
                    {
                    }
                    gpio_put(rowPin[i], 1);
                    return TecladoMatricial[i][j];
                }
            }
        }
        gpio_put(rowPin[i], 1);
    }

    return '\0';
}

void animacao_1(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    gpio_put(pino_buzzer, true);
    double letras[7][25] = {
        {1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0},

        {1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 0.0, 1.0, 1.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 1.0, 1.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0}};

    for (int letra = 0; letra < 7; letra++)
    {
        for (int16_t i = 0; i < 25; i++)
        {
            valor_led = matrix_rgb(letras[letra][24 - i], r = 0, g = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
            
        }
        imprimir_binario(valor_led);
        sleep_ms(2000);
    }
}

void animacao_2(uint32_t led_bin, PIO pio, uint sm){
    
    // As matrizes devem ser vistas como conjuntos de 3 (RGB), e a animação tem 5 frames, o que totaliza 15 matrizes
    double animacao[15][NUM_PIXELS] = {

    // Frame 1 - Apenas o solo
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.7, 0.7, 0.7, 0.7, 0.7},

    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.4, 0.4, 0.4, 0.4, 0.4},

    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.2, 0.2, 0.2, 0.2, 0.2},

    // Frame 2 - Tronco aparece
    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.7, 0.7, 0.7, 0.7, 0.7},

    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.3, 0.0, 0.0,
     0.0, 0.0, 0.3, 0.0, 0.0,
     0.4, 0.4, 0.4, 0.4, 0.4},

    {0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.0, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.2, 0.2, 0.3, 0.2, 0.2},

    // Frame 3 - Galhos começam a crescer
    {0.0, 0.5, 0.0, 0.0, 0.5,
     0.0, 0.5, 0.0, 0.5, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.5,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.7, 0.7, 0.7, 0.7, 0.7},

    {0.0, 0.3, 0.0, 0.0, 0.3,
     0.0, 0.3, 0.1, 0.3, 0.0,
     0.0, 0.1, 0.3, 0.1, 0.3,
     0.0, 0.0, 0.3, 0.0, 0.0,
     0.4, 0.4, 0.4, 0.4, 0.4},

    {0.0, 0.1, 0.0, 0.0, 0.1,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.1,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.2, 0.2, 0.3, 0.2, 0.2},

    // Frame 4 - Folhas surgem
    {0.3, 0.5, 0.2, 0.0, 0.5,
     0.0, 0.5, 0.0, 0.5, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.7, 0.7, 0.7, 0.7, 0.7},

    {0.6, 0.7, 0.8, 0.8, 0.7,
     0.8, 0.7, 0.8, 0.7, 0.6,
     0.3, 0.3, 0.3, 0.3, 0.7,
     0.0, 0.0, 0.3, 0.0, 0.0,
     0.4, 0.4, 0.4, 0.4, 0.4},

    {0.0, 0.1, 0.2, 0.0, 0.1,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.3,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.2, 0.2, 0.3, 0.2, 0.2},

    // Frame 5 - Frutos aparecem
    {0.8, 0.3, 0.6, 0.0, 0.3,
     0.0, 0.3, 0.0, 0.3, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.0, 0.0, 0.5, 0.0, 0.0,
     0.7, 0.7, 0.7, 0.7, 0.7},

    {0.0, 0.7, 0.0, 0.8, 0.7,
     0.9, 0.7, 1.0, 0.7, 0.8,
     0.5, 0.8, 0.3, 0.8, 0.3,
     0.0, 0.0, 0.3, 0.0, 0.0,
     0.4, 0.4, 0.4, 0.4, 0.4},

    {0.0, 0.1, 0.5, 0.0, 0.1,
     0.0, 0.1, 0.0, 0.1, 0.0,
     0.0, 0.0, 0.1, 0.0, 0.7,
     0.0, 0.0, 0.1, 0.0, 0.0,
     0.2, 0.2, 0.3, 0.2, 0.2}};



    for (int frame = 0; frame < 15; frame += 3){    // Iteração de frames
        for (int16_t i = 0; i < NUM_PIXELS; i++){   // Iteração de pixels

            led_bin = matrix_rgb(animacao[frame][24 - i], animacao[frame + 2][24 - i], animacao[frame + 1][24 - i]);
            pio_sm_put_blocking(pio, sm, led_bin);
            
        }
        imprimir_binario(led_bin);
        sleep_ms(1000);     // 1 frame por segundo
    }
}

void animacao_3(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double letras[5][25] = {
        {1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 1.0, 1.0, 1.0, 0.0},

        {1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},

        {0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0}};

    for (int letra = 0; letra < 5; letra++)
    {
        for (int16_t i = 0; i < 25; i++)
        {
            valor_led = matrix_rgb(letras[letra][24 - i], r = 0, g = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        imprimir_binario(valor_led);
        sleep_ms(2000);
    }
}

void animacao_4(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double letras[5][25] = {
        {1, 1, 1, 1, 1,
         0, 0, 0, 0, 1,
         1, 1, 1, 1, 1,
         1, 0, 0, 0,0,
         1, 1, 1, 1, 1},

        {1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        0, 0, 0, 0, 1},

        {1,1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        1, 1, 1, 1, 1},

        {1, 1, 1, 1, 1,
        1, 0, 0, 0, 0,
        0, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1},

        {0, 0, 1, 0, 0,
        0, 0, 1, 1, 0,
        1, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        1, 1, 1, 1, 1}};

    for (int letra = 0; letra < 5; letra++)
    {
        for (int16_t i = 0; i < 25; i++)
        {
            valor_led = matrix_rgb(letras[letra][24 - i], r = 0, g = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        imprimir_binario(valor_led);
        sleep_ms(2000);
    }
}
void animacao_5(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double animacaos[5][5][5][3] = {
        {{{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}},
        {{{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}},
        {{{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}},
        {{{255, 0, 0}, {255, 0, 0}, {255, 255, 0}, {255, 0, 0}, {255, 0, 0}},
         {{255, 0, 0}, {255, 255, 0}, {255, 0, 0}, {255, 255, 0}, {255, 0, 0}},
         {{255, 0, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 0, 0}},
         {{255, 255, 0}, {255, 0, 0}, {255, 255, 0}, {255, 0, 0}, {255, 255, 0}},
         {{255, 255, 0}, {255, 255, 0}, {255, 0, 0}, {255, 255, 0}, {255, 255, 0}}},

        {{{255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {255, 0, 0}},
         {{255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}},
         {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
         {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
         {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}}};

    for (int animacao = 0; animacao < 5; animacao++)
    {
        for (int coluna = 0; coluna < 5; coluna++)
        {
            for (int linha = 0; linha < 5; linha++)
            {
                valor_led = matrix_rgb(
                    animacaos[animacao][linha][coluna][0] / 255.0,
                    animacaos[animacao][linha][coluna][1] / 255.0,
                    animacaos[animacao][linha][coluna][2] / 255.0);
                pio_sm_put_blocking(pio, sm, valor_led);
            }
        }
        imprimir_binario(valor_led);
        sleep_ms(2000);
    }
}

void animacao_6(uint32_t valor_led, PIO pio, uint sm)
{
    double fantasma[25] = {
        0.0, 1.0, 1.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 1.0,
        1.0, 0.0, 1.0, 0.0, 1.0};

    // Exibir fantasma amarelo
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(fantasma[24 - i], fantasma[24 - i], 0.0); // Amarelo
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(2000);

    // Exibir fantasma vermelho
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(fantasma[24 - i], 0.0, 0.0); // Vermelho
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(2000);

    // Exibir fantasma azul
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(0.0, 0.0, fantasma[24 - i]); // Azul
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(2000);

    // Exibir fantasma verde
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(0.0, fantasma[24 - i], 0.0); // Verde
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(2000);

    // Exibir fantasma rosa
    for (int i = 0; i < 25; i++)
    {
        valor_led = matrix_rgb(fantasma[24 - i], fantasma[24 - i] * 0.5, fantasma[24 - i] * 0.5); // Rosa
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(2000);
}

void animacao_7(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double letras[6][25] = {
        {0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 1.0, 0.0, 0.0,
         0.0, 1.0, 1.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 1.0, 1.0, 0.0, 0.0,
         0.0, 1.0, 1.0, 0.0, 0.0},

        {0.0, 0.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 0.0},

        {0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 1.0, 1.0, 0.0, 0.0,
         0.0, 1.0, 1.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 1.0, 1.0, 0.0, 0.0,
         0.0, 1.0, 1.0, 0.0, 0.0},

        {0.0, 0.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 1.0, 0.0}};

    for (int letra = 0; letra < 6; letra++)
    {
        for (int16_t i = 0; i < 25; i++)
        {
            valor_led = matrix_rgb(letras[letra][24 - i], r = 0, g = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        imprimir_binario(valor_led);
        sleep_ms(2000);
    }
}
void desligar_leds(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(b = 0, r = 0, g = 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
}
void all_led_azul_100(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double desenho[25] = {1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(r=0, desenho[24 - i], g = 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(500);
}
void all_led_vermelho_80(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double desenho[25] = {0.8, 0.8, 0.8, 0.8, 0.8,
                          0.8, 0.8, 0.8, 0.8, 0.8,
                          0.8, 0.8, 0.8, 0.8, 0.8,
                          0.8, 0.8, 0.8, 0.8, 0.8,
                          0.8, 0.8, 0.8, 0.8, 0.8};

    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(desenho[24 - i], b=0, g = 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(500);
}
void all_led_verde_50(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double desenho[25] = {0.5, 0.5, 0.5, 0.5, 0.5,
                          0.5, 0.5, 0.5, 0.5, 0.5,
                          0.5, 0.5, 0.5, 0.5, 0.5,
                          0.5, 0.5, 0.5, 0.5, 0.5,
                          0.5, 0.5, 0.5, 0.5, 0.5};
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(r = 0.0, b = 00, desenho[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(500);
}
void all_led_branco_20(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double desenho[25] = {0.2, 0.2, 0.2, 0.2, 0.2,
                          0.2, 0.2, 0.2, 0.2, 0.2,
                          0.2, 0.2, 0.2, 0.2, 0.2,
                          0.2, 0.2, 0.2, 0.2, 0.2,
                          0.2, 0.2, 0.2, 0.2, 0.2};
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        valor_led = matrix_rgb(desenho[24 - i], desenho[24 - i], desenho[24 - i]);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(500);
}
void Resetar()
{
    reset_usb_boot(0, 0);
}