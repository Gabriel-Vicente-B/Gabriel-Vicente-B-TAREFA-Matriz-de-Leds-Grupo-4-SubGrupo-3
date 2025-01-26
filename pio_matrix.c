#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"

// Definição do número de linhas e colunas do teclado matricial
#define row 4
#define column 4
#define NUM_PIXELS 25
#define OUT_PIN 7
// Pinos GPIO dispostos segundo linhas e colunas de cima para baixo, esquerda para direita
// Linha 1 = GPIO_4, Linha 2 = GPIO_8...
// Coluna 1 = GPIO_17, Coluna 2 = GPIO_18...
const uint rowPin[row] = {9,8,6, 5};
const uint columnPin[column] = {4, 3, 2, 1};

// Matriz representando os botões do teclado matricial
// Exemplo: 'A' corresponde aos pinos 4 (Linha 1) e 20 (Coluna 4) sendo ligados
const char TecladoMatricial[row][column] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

void inicializar_pinos();
char ler_teclado_matricial();
void animacao_1(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void animacao_2(uint32_t led_bin, PIO pio, uint sm);
void animacao_3(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b);
void animacao_4();
void animacao_5();
void animacao_6();
void animacao_7();
void desligar_leds();
void all_led_azul_100();
void all_led_vermelho_80();
void all_led_verde_50();
void all_led_branco_20();
// vetor para criar imagem na matriz de led 5X5 - 2
void imprimir_binario(int num)
{
    int i;
    for (i = 31; i >= 0; i--)
    {
        (num & (1 << i)) ? printf("1") : printf("0");
    }
}

uint32_t matrix_rgb(double b, double r, double g)
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
    inicializar_pinos();
    PIO pio = pio0;
    bool ok;
    uint32_t valor_led;
    double r = 0.0, b = 0.0, g = 0.0;
    ok = set_sys_clock_khz(128000, false);
    stdio_init_all();
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

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
            animacao_4();
            break;
        case '5':
            animacao_5(valor_led, pio, sm, r, g, b);
            break;
        case '6':
            animacao_6();
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
        default:
            break;
        }
    }
}

void inicializar_pinos()
{
    // Inicializa os pinos GPIO das linhas
    for (int i = 0; i < row; i++)
    {
        gpio_init(rowPin[i]);
        gpio_set_dir(rowPin[i], GPIO_OUT);
        gpio_put(rowPin[i], 0); // Na inicialização, os pinos estarão em low
    }

    // Inicializa os pinos GPIO das colunas
    for (int j = 0; j < column; j++)
    {
        gpio_init(columnPin[j]);
        gpio_set_dir(columnPin[j], GPIO_IN);
        gpio_pull_up(columnPin[j]); // Ativa pull-up
    }
}

char ler_teclado_matricial()
{
    for (int i = 0; i < row; i++)
    {
        gpio_put(rowPin[i], 0); // Ativa a linha atual (nível baixo)

        for (int j = 0; j < column; j++)
        {
            // Verifica se o botão foi pressionado
            if (!gpio_get(columnPin[j]))
            {                 // Detecta nível baixo (tecla pressionada)
                sleep_ms(50); // Aguarda para debounce
                if (!gpio_get(columnPin[j]))
                { // Confirma a tecla pressionada
                    while (!gpio_get(columnPin[j]))
                    {
                        // Aguarda a tecla ser solta
                    }
                    gpio_put(rowPin[i], 1);        // Desativa a linha
                    return TecladoMatricial[i][j]; // Retorna o caractere pressionado
                }
            }
        }

        gpio_put(rowPin[i], 1); // Desativa a linha após a verificação
    }

    return '\0'; // Retorna '\0' se nenhuma tecla for pressionada
}

void animacao_1(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double letras[7][25] = {
        {1.0, 1.0, 1.0, 1.0, 1.0, // Letra E
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0},

        {1.0, 0.0, 0.0, 0.0, 1.0, // Letra M
         1.0, 1.0, 0.0, 1.0, 1.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 0.0, // Letra B
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0, // Letra A
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 0.0, // Letra R
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0, // Letra C
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 1.0, 1.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0, // Letra A (repetida)
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0}};

    for (int letra = 0; letra < 7; letra++)
    { // Itera sobre as letras
        for (int16_t i = 0; i < 25; i++)
        { // Itera sobre os pixels da matriz
            valor_led = matrix_rgb(letras[letra][24 - i], r = 0, g = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        imprimir_binario(valor_led);
        sleep_ms(2000); // Aguarda 5 segundos antes de mostrar a próxima letra
    }
}

void animacao_2(uint32_t led_bin, PIO pio, uint sm){

    // As matrizes devem ser vistas como conjuntos de 3 (RGB), e a animação tem 5 frames, o que totaliza 15 matrizes
    double animacao[15][NUM_PIXELS] = {
        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.4, 0.0, 0.0,
         0.0, 0.0, 0.7, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.1, 0.0, 0.0,
         0.0, 0.0, 0.2, 0.0, 0.0},
        //////////////////////////
        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.2, 0.0, 0.0,
         0.0, 0.2, 0.4, 0.2, 0.0,
         0.0, 0.0, 0.7, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.1, 0.0, 0.0,
         0.0, 0.0, 0.2, 0.0, 0.0,
         0.0, 0.0, 0.3, 0.0, 0.0},
        //////////////////////////
        {0.1, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.1,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0},

        {0.1, 0.0, 0.1, 0.0, 0.0,
         0.1, 0.0, 0.4, 0.0, 0.1,
         0.0, 0.4, 0.7, 0.4, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.1,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.1, 0.0, 0.0,
         0.0, 0.1, 0.2, 0.1, 0.0,
         0.0, 0.0, 0.3, 0.0, 0.0,
         0.0, 0.0, 0.3, 0.0, 0.0},
        //////////////////////////
        {0.3, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.3,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.0},

        {0.2, 0.0, 0.2, 0.0, 0.0,
         0.2, 0.0, 0.7, 0.0, 0.2,
         0.0, 0.7, 1.0, 0.7, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.1,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 0.0, 0.5, 0.0, 0.0,
         0.0, 0.0, 0.2, 0.0, 0.0,
         0.0, 0.2, 0.3, 0.2, 0.0,
         0.0, 0.0, 0.3, 0.0, 0.1,
         0.0, 0.0, 0.5, 0.0, 0.0},
        //////////////////////////
        {0.8, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.4,
         0.0, 0.0, 0.0, 0.0, 0.0,
         0.0, 0.0, 0.0, 0.0, 0.3,
         0.0, 0.0, 0.0, 0.0, 0.0},

        {0.2, 0.0, 0.2, 0.0, 0.0,
         0.4, 0.0, 0.7, 0.0, 0.6,
         0.0, 0.7, 1.0, 0.7, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.2,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {0.0, 0.0, 0.7, 0.0, 0.0,
         0.0, 0.0, 0.3, 0.0, 0.0,
         0.0, 0.3, 0.4, 0.3, 0.0,
         0.0, 0.0, 0.4, 0.0, 0.6,
         0.0, 0.0, 0.5, 0.0, 0.0}
    };

    for (int frame = 0; frame < 15; frame+3) {  // Iteração de frames
        for (int16_t i = 0; i < NUM_PIXELS; i++) {    // Iteração de pixels

            led_bin = matrix_rgb(animacao[frame][24 - i], animacao[frame + 1][24 - i], animacao[frame + 2][24 - i]);
            pio_sm_put_blocking(pio, sm, led_bin);

        }
        imprimir_binario(led_bin);
        sleep_ms(1000);  // 1 frame por segundo
    }
    sleep_ms(5000); // Mantém o frame final por 3 segundos
    led_bin = matrix_rgb(0.0, 0.0, 0.0); // Apaga a imagem
    pio_sm_put_blocking(pio, sm, led_bin);
}

void animacao_3(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{
    double letras[5][25] = {
        {1.0, 1.0, 1.0, 1.0, 1.0, // Letra T
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 0.0},

        {1.0, 1.0, 1.0, 1.0, 1.0, // Letra E
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0},

        {0.0, 1.0, 1.0, 1.0, 0.0, // Letra C
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 1.0, 1.0, 1.0, 0.0},

        {1.0, 0.0, 0.0, 0.0, 1.0, // Letra H
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},
         
        {0.0, 0.0, 1.0, 0.0, 0.0, // Letra +
         0.0, 0.0, 1.0, 0.0, 0.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 1.0, 0.0,0.0,
         0.0, 0.0, 1.0, 0.0, 0.0}};

    for (int letra = 0; letra < 5; letra++)
  {   // Itera sobre as letras
        for (int16_t i = 0; i < 25; i++)
        { // Itera sobre os pixels da matriz
            valor_led = matrix_rgb(letras[letra][24 - i], r = 0, g = 0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
        imprimir_binario(valor_led);
        sleep_ms(2000); // Aguarda 5 segundos antes de mostrar a próxima letra
    }
}

void animacao_4()
{
    
}
void animacao_5(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    double animacaos[5][5][5][3] = { // Corrigido para ter a dimensão correta
        { // Animação 1
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 0}}, 
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}
        },
        { // Animação 2
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}
        },
        { // Animação 3
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
            {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}
        }
        // Adicione as animações 4 e 5 aqui...
    };

    for (int animacao = 0; animacao < 5; animacao++) { // Corrigido para 5 animações
        for (int coluna = 0; coluna < 5; coluna++) { 
            for (int linha = 0; linha < 5; linha++) { 
                valor_led = matrix_rgb(
                    animacaos[animacao][linha][coluna][0] / 255.0, 
                    animacaos[animacao][linha][coluna][1] / 255.0, 
                    animacaos[animacao][linha][coluna][2] / 255.0
                );
                pio_sm_put_blocking(pio, sm, valor_led);
            }
        }

        imprimir_binario(valor_led); // Imprime o valor binário da animação exibida
        sleep_ms(2000); // Espera 2 segundos antes da próxima animação
    }
}
void animacao_6()
{
   
}
void animacao_7()
{
         
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

        valor_led = matrix_rgb(desenho[24 - i], r = 0, g = 0.0);
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

        valor_led = matrix_rgb(b = 0.0, desenho[24 - i], g = 0.0);
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

        valor_led = matrix_rgb(b = 0.0, r = 00, desenho[24 - i]);
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