#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/adc.h"
#include "pico/bootrom.h"
#include "pio_matrix.pio.h"
#include "ws2818b.pio.h"

// Definição do número de linhas e colunas do teclado matricial
#define row 4
#define column 4
#define NUM_PIXELS 25
#define OUT_PIN 7

// Defininção do número de LEDs
#define NUM_PIXELS 25

// Pinos GPIO dispostos segundo linhas e colunas de cima para baixo, esquerda para direita
// Linha 1 = GPIO_4, Linha 2 = GPIO_8...
// Coluna 1 = GPIO_17, Coluna 2 = GPIO_18...
const uint rowPin[row] = {8, 7, 6, 5};
const uint columnPin[column] = {4, 3, 2, 1};

// Matriz representando os botões do teclado matricial
// Exemplo: 'A' corresponde aos pinos 4 (Linha 1) e 20 (Coluna 4) sendo ligados
const char TecladoMatricial[row][column] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


void inicializar_pinos();
char ler_teclado_matricial();
void animacao_1(uint32_t valor_led, PIO pio,uint sm,double r,double g,double b);
void animacao_2();
void animacao_3();
void animacao_4();
void animacao_5(uint8_t G, uint8_t R, uint8_t B, PIO np_pio, uint sm);
void animacao_6();
void animacao_7();
void desligar_leds();
void all_led_azul_100();
void all_led_vermelho_80();
void all_led_verde_50();
void all_led_branco_20();




//vetor para criar imagem na matriz de led 5X5 - 2



void imprimir_binario(int num) {
 int i;
 for (i = 31; i >= 0; i--) {
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
};

void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }
  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }
}

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds[index].R = r;
  leds[index].G = g;
  leds[index].B = b;
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

int getIndex(int x, int y) {
    // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
    // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
    if (y % 2 == 0) {
        return 24-(y * 5 + x); // Linha par (esquerda para direita).
    } else {
        return 24-(y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
    }
}

int main()
{
    int x;
    inicializar_pinos();
    PIO pio = pio0; 
    bool ok;    
    uint32_t valor_led;
    double r = 0.0, b = 0.0 , g = 0.0;
    ok = set_sys_clock_khz(128000, false);
    stdio_init_all();
    if (ok) printf("clock set to %ld\n", clock_get_hz(clk_sys));

    uint offset = pio_add_program(pio, &pio_matrix_program);
    uint sm = pio_claim_unused_sm(pio, true);
    pio_matrix_program_init(pio, sm, offset, OUT_PIN);
    
    all_led_azul_100(valor_led,pio,sm,r,g,b);

        while (true) {
        char tecla = ler_teclado_matricial();
        switch (tecla){
            case '1':
                animacao_1(valor_led,pio,sm,r,g,b);
                break;
            case '2':
                animacao_2();
                break;
            case '3':
                animacao_3(valor_led, pio, sm, r, g, b);
                break;
            case '4':
                animacao_4();
                break;
            case '5':
                animacao_5();
                break;
            case '6':
                animacao_6();
                break;
            case '7':
                animacao_7();
                break;
            case 'A':
                desligar_leds(valor_led,pio,sm,r,g,b);
                break;
            case 'B':
                all_led_azul_100(valor_led,pio,sm,r,g,b);
                break;
            case 'C':
                all_led_vermelho_80(valor_led,pio,sm,r,g,b);
                break;
            case 'D':
                all_led_verde_50(valor_led,pio,sm,r,g,b);
                break;
            case '#':
                all_led_branco_20(valor_led,pio,sm,r,g,b);
                break;
            default:
                break;
        }

}

}

void inicializar_pinos(){
    // Inicializa os pinos GPIO das linhas
    for (int i = 0; i < row; i++) {
        gpio_init(rowPin[i]);
        gpio_set_dir(rowPin[i], GPIO_OUT);
        gpio_put(rowPin[i], 0); // Na inicialização, os pinos estarão em low
    }

    // Inicializa os pinos GPIO das colunas
    for (int j = 0; j < column; j++) {
    gpio_init(columnPin[j]);
    gpio_set_dir(columnPin[j], GPIO_IN);
    gpio_pull_up(columnPin[j]);  // Ativa pull-up
    }
}

char ler_teclado_matricial() {
    for (int i = 0; i < row; i++) {
        gpio_put(rowPin[i], 0);  // Ativa a linha atual (nível baixo)

        for (int j = 0; j < column; j++) {
            // Verifica se o botão foi pressionado
            if (!gpio_get(columnPin[j])) {  // Detecta nível baixo (tecla pressionada)
                sleep_ms(50);  // Aguarda para debounce
                if (!gpio_get(columnPin[j])) {  // Confirma a tecla pressionada
                    while (!gpio_get(columnPin[j])) {
                        // Aguarda a tecla ser solta
                    }
                    gpio_put(rowPin[i], 1);  // Desativa a linha
                    return TecladoMatricial[i][j];  // Retorna o caractere pressionado
                }
            }
        }

        gpio_put(rowPin[i], 1);  // Desativa a linha após a verificação
    }

    return '\0';  // Retorna '\0' se nenhuma tecla for pressionada
}



void animacao_1(uint32_t valor_led, PIO pio,uint sm,double r,double g,double b){
double letras[7][25] = {
    {1.0, 1.0, 1.0, 1.0, 1.0,  // Letra E
     0.0, 0.0, 0.0, 0.0, 1.0, 
     1.0, 1.0, 1.0, 1.0, 1.0,
     0.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0},

    {1.0, 0.0, 0.0, 0.0, 1.0,  // Letra M
     1.0, 1.0, 0.0, 1.0, 1.0,
     1.0, 0.0, 1.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0},

    {1.0, 1.0, 1.0, 1.0, 0.0,  // Letra B
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 0.0},

    {0.0, 1.0, 1.0, 1.0, 0.0,  // Letra A
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0},

    {1.0, 1.0, 1.0, 1.0, 0.0,  // Letra R
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 0.0,
     0.0, 0.0, 1.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 1.0, 0.0},

    {0.0, 1.0, 1.0, 1.0, 0.0,  // Letra C
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 0.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     0.0, 1.0, 1.0, 1.0, 0.0},

    {0.0, 1.0, 1.0, 1.0, 0.0,  // Letra A (repetida)
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0,
     1.0, 0.0, 0.0, 0.0, 1.0}
};

for (int letra = 0; letra < 7; letra++) {  // Itera sobre as letras
    for (int16_t i = 0; i < 25; i++) {    // Itera sobre os pixels da matriz
        valor_led = matrix_rgb(letras[letra][24 - i], r = 0, g = 0.0);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    sleep_ms(2000);  // Aguarda 5 segundos antes de mostrar a próxima letra
}

}
void animacao_2(){


    
}
void animacao_3(uint32_t valor_led, PIO pio, uint sm, double r, double g, double b) {
    double letras[7][25] = {
        {1.0, 1.0, 1.0, 1.0, 1.0,  // Letra E
         0.0, 0.0, 0.0, 0.0, 1.0, 
         1.0, 1.0, 1.0, 1.0, 1.0,
         0.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0},

        {1.0, 0.0, 0.0, 0.0, 1.0,  // Letra M
         1.0, 1.0, 0.0, 1.0, 1.0,
         1.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 0.0,  // Letra B
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,  // Letra A
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0},

        {1.0, 1.0, 1.0, 1.0, 0.0,  // Letra R
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 0.0,
         0.0, 0.0, 1.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,  // Letra C
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 0.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         0.0, 1.0, 1.0, 1.0, 0.0},

        {0.0, 1.0, 1.0, 1.0, 0.0,  // Letra A (repetida)
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 1.0, 1.0, 1.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0,
         1.0, 0.0, 0.0, 0.0, 1.0}
    };

    for (int letra = 0; letra < 7; letra++) { // Itera sobre as letras
        for (int coluna = 0; coluna < 5; coluna++) { // Colunas de cada letra
            for (int linha = 0; linha < 5; linha++) { // Linhas de cada letra
                valor_led = matrix_rgb(
                    letras[letra][linha * 5 + coluna] * b, 
                    letras[letra][linha * 5 + coluna] * r, 
                    letras[letra][linha * 5 + coluna] * g
                );
                pio_sm_put_blocking(pio, sm, valor_led);
            }
        }

        // Imprime o valor binário da letra exibida
        imprimir_binario(valor_led);

        // Espera 2 segundos antes de mostrar a próxima letra
        sleep_ms(2000);
    }
}

void animacao_4(){

int numeros[5][5][5] = {
    
    // Número 5
    {
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    }, 

    // Número 4
    {
        {1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },

    // Número 3
    {
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1}
    },

    // Número 2
    {
        {1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1}
    },

    // Número 1
    {
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {1, 1, 1, 1, 1}
    }

};

    for (int n = 0; n < 5; n++) {
        for(int i = 0; i < 5; i++){
            for (int j = 0; j < 5; j++){
                if (numeros[n][i][j] == 1){
                    ligar_led(i, j);
                } else {
                    desligar_led(i, j);
                }
                
            }
        }
        sleep_ms(1000);
        limpar_matriz();

    }

    
}
void animacao_5(){
  npInit(LED_PIN);
  npClear();

  // Aqui, você desenha nos LEDs.

  npWrite(); // Escreve os dados nos LEDs.

  // Não faz mais nada. Loop infinito.
  while (true) {

    int imagem01 [5][5][3] = {
      {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
      {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
      {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
      {{255, 255, 0}, {255, 255, 255}, {255, 255, 255}, {255, 255, 255}, {255, 255, 0}},
      {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}};
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, imagem01[coluna][linha][0], imagem01[coluna][linha][1], imagem01[coluna][linha][2]);
    }}
    npWrite();
    sleep_ms(2000);
    npClear();
    int imagem02 [5][5][3] = {
    {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
    {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
    {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
    {{255, 255, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {255, 255, 0}},
    {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}};
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, imagem02[coluna][linha][0], imagem02[coluna][linha][1], imagem02[coluna][linha][2]);
    }}
    npWrite();
    sleep_ms(2000);
    npClear();
    int imagem03 [5][5][3] = {
    {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
    {{255, 255, 0}, {0, 0, 0}, {255, 255, 0}, {0, 0, 0}, {255, 255, 0}},
    {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
    {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}},
    {{255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}}};
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, imagem03[coluna][linha][0], imagem03[coluna][linha][1], imagem03[coluna][linha][2]);
    }}
    npWrite();
    sleep_ms(1000);
    npClear();

    int imagem04 [5][5][3] = {
    {{255, 0, 0}, {255, 0, 0}, {255, 255, 0}, {255, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {255, 255, 0}, {255, 0, 0}, {255, 255, 0}, {255, 0, 0}},
    {{255, 0, 0}, {255, 255, 0}, {255, 255, 0}, {255, 255, 0}, {255, 0, 0}},
    {{255, 255, 0}, {255, 0, 0}, {255, 255, 0}, {255, 0, 0}, {255, 255, 0}},
    {{255, 255, 0}, {255, 255, 0}, {255, 0, 0}, {255, 255, 0}, {255, 255, 0}}};
    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, imagem04[coluna][linha][0], imagem04[coluna][linha][1], imagem04[coluna][linha][2]);
    }}
    npWrite();
    sleep_ms(2000);
    npClear();
     int imagem05 [5][5][3] = {
        {
    {{255, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
    {{0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {255, 0, 0}, {0, 0, 0}, {0, 0, 0}}}};

    for(int linha = 0; linha < 5; linha++){
    for(int coluna = 0; coluna < 5; coluna++){
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, imagem05[coluna][linha][0], imagem05[coluna][linha][1], imagem05[coluna][linha][2]);
    }}
    npWrite();
    sleep_ms(2000);
    npClear();
}
void animacao_6(){


    
}
void animacao_7(){


    
}
void desligar_leds(uint32_t valor_led, PIO pio,uint sm,double r,double g,double b){
    for (int16_t i = 0; i < NUM_PIXELS; i++) {

            valor_led = matrix_rgb(b=0, r=0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);

    
}
void all_led_azul_100(uint32_t valor_led, PIO pio,uint sm,double r,double g,double b){

    double desenho[25] =   {1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0, 1.0, 1.0, 
                            1.0, 1.0, 1.0, 1.0, 1.0,
                            1.0, 1.0, 1.0, 1.0,1.0,
                            1.0,1.0, 1.0, 1.0, 1.0};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {

            valor_led = matrix_rgb(desenho[24-i], r=0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    
    sleep_ms(500);

    
}
void all_led_vermelho_80(uint32_t valor_led, PIO pio,uint sm,double r,double g,double b){

    double desenho[25] =   {0.8, 0.8, 0.8, 0.8, 0.8,
                            0.8, 0.8, 0.8, 0.8,0.8, 
                            0.8, 0.8, 0.8, 0.8, 0.8,
                            0.8, 0.8, 0.8, 0.8,0.8,
                            0.8,0.8, 0.8, 0.8, 0.8};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {

            valor_led = matrix_rgb(b=0.0,desenho[24-i], g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    
    sleep_ms(500);
    
}
void all_led_verde_50(uint32_t valor_led, PIO pio,uint sm,double r,double g,double b){
    double desenho[25] =   {0.5, 0.5,0.5, 0.5, 0.5,
                            0.5, 0.5, 0.5, 0.5,0.5, 
                            0.5, 0.5, 0.5, 0.5, 0.5,
                            0.5,0.5, 0.5, 0.5,0.5,
                            0.5,0.5, 0.5, 0.5, 0.5};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {

            valor_led = matrix_rgb(b=0.0,r=00, desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    
    sleep_ms(500);

    
}
void all_led_branco_20(uint32_t valor_led, PIO pio,uint sm,double r,double g,double b){
    double desenho[25] =   {0.2,0.2, 0.2,0.2,0.2,
                            0.2,0.2,0.2,0.2,0.2, 
                            0.2, 0.2,0.2,0.2, 0.2,
                            0.2,0.2,0.2,0.2,0.2,
                            0.2,0.2,0.2,0.2, 0.2};

    for (int16_t i = 0; i < NUM_PIXELS; i++) {

            valor_led = matrix_rgb(desenho[24-i],desenho[24-i],desenho[24-i]);
            pio_sm_put_blocking(pio, sm, valor_led);
    }
    imprimir_binario(valor_led);
    
    sleep_ms(500);
}