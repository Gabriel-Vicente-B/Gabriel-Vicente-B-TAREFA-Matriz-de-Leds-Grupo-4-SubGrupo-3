#include <stdio.h>
#include "pico/stdlib.h"
#include "pio_matrix.pio.h"
#include "hardware/pio.h"

// Definição do número de linhas e colunas do teclado matricial
#define row 4
#define column 4
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



int main()
{
    stdio_init_all();
    inicializar_pinos();

        while (true) {
        char tecla = ler_teclado_matricial();
        switch (tecla){
            case '1':
                animacao_1();
                break;
            case '2':
                animacao_2();
                break;
            case '3':
                animacao_3();
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
                desligar_leds();
                break;
            case 'B':
                all_led_azul_100();
                break;
            case 'C':
                all_led_vermelho_80();
                break;
            case 'D':
                all_led_verde_50();
                break;
            case '#':
                all_led_branco_20();
                break;
            default:
                break;
        }

}

}
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b){

    for (int16_t i = 0; i < NUM_PIXELS; i++) {
        if (i%2==0)
        {
            valor_led = matrix_rgb(desenho[24-i], r=0.0, g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);

        }else{
            valor_led = matrix_rgb(b=0.0, desenho[24-i], g=0.0);
            pio_sm_put_blocking(pio, sm, valor_led);
        }
    }
    imprimir_binario(valor_led);
}
uint32_t matrix_rgb(double b, double r, double g)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
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



void animacao_1(){



}
void animacao_2(){


    
}
void animacao_3(){


    
}
void animacao_4(){


    
}
void animacao_5(){


    
}
void animacao_6(){


    
}
void animacao_7(){


    
}
void desligar_leds(){


    
}
void all_led_azul_100(){


    
}
void all_led_vermelho_80(){


    
}
void all_led_verde_50(){


    
}
void all_led_branco_20();