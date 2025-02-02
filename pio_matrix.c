/**
                          Código desenvolvido por: Gilmaiane Porto Silva
                                    Funcionalidades do Projeto
1. O LED vermelho do LED RGB deve piscar continuamente 5 vezes por segundo.
2. O botão A deve incrementar o número exibido na matriz de LEDs cada vez que for pressionado.
3. O botão B deve decrementar o número exibido na matriz de LEDs cada vez que for pressionado.
4. Os LEDs WS2812 devem ser usados para criar efeitos visuais representando números de 0 a 9.
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "led_matrix.h"
#include "hardware/i2c.h"
#include <string.h>
#include "ssd1306.h"
#include <stdlib.h>
#include <math.h>
#include <teclado.h>
#include <stdlib.h>
// Pino de saída
#define WS2812_PIN 7
uint sm;  // Variável para armazenar o número da máquina de estados

// Configuração dos pinos
const uint PIN_LED_VERMELHO = 13; //Red => GPIO13
#define tempo 100 

const uint PIN_BOTAO_A = 5;  // Botão A
const uint PIN_BOTAO_B = 6;  // Botão B

const uint I2C_SDA_PIN = 14;
const uint I2C_SCL_PIN = 15;

//Definições buzzer
#define BUZZER 10
// Pino de saída e frequência do buzzer
#define FREQUENCY 50 //Toms mais graves ou agudos


// Número de LEDs
#define NUM_PIXELS 25
#define NUM_FRAMES 10

// Variáveis globais
static volatile int contador = 0;  // Variável para armazenar o número (0 a 9)
static volatile uint32_t last_time_A = 0;  // Armazena o tempo do último evento para o botão A
static volatile uint32_t last_time_B = 0;  // Armazena o tempo do último evento para o botão B
volatile uint32_t last_interrupt_time = 0;
float r = 0.0, g = 1.0, b = 0.001; // Inicialização das variáveis RGB
// Declaração da função (antes de usá-la)


// Prototipação da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);


void piscar_led_vermelho () {
   // Lógica para o LED piscar
        gpio_put(PIN_LED_VERMELHO, 1); // Liga o LED
        sleep_ms(tempo);            // Mantém ligado por "tempo" ms
        gpio_put(PIN_LED_VERMELHO, 0); // Desliga o LED
        sleep_ms(tempo);            // Mantém ligado por "tempo" ms

    }


    //Inicializações
void inicializacoes(){
    // Configura o GPIO do LED vermelho
    gpio_init(PIN_LED_VERMELHO);
    gpio_set_dir(PIN_LED_VERMELHO, GPIO_OUT);

    // Configura os botões como entrada com pull-up interno
    gpio_init(PIN_BOTAO_A);
    gpio_set_dir(PIN_BOTAO_A, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_A);

    gpio_init(PIN_BOTAO_B);
    gpio_set_dir(PIN_BOTAO_B, GPIO_IN);
    gpio_pull_up(PIN_BOTAO_B);

    // Configura a interrupção com callback para os botões A e B
    gpio_set_irq_enabled_with_callback(PIN_BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(PIN_BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

}

    
int main() {
    stdio_init_all();  // Inicializa a comunicação serial
    setup_led_matrix();  // Configura a matriz de LEDs
    inicializacoes();

    //Animação PADRÃO
    animacao(1,5,xadrez,sm); // Chama a função de animação

 // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // run through the complete initialization process
    SSD1306_init();

    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area frame_area = {
        start_col : 0,
        end_col : SSD1306_WIDTH - 1,
        start_page : 0,
        end_page : SSD1306_NUM_PAGES - 1
    };

    calc_render_area_buflen(&frame_area);

    // zero the entire display
    uint8_t buf[SSD1306_BUF_LEN];
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

restart:

    SSD1306_scroll(true);
    sleep_ms(5000);
    SSD1306_scroll(false);

    char *text[] = {
        "   Bem-Vindo ",
        " ao EmbarcaTech ",
        "      2024 ",
        "  SOFTEX/MCTI "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++)
    {
        WriteString(buf, 5, y, text[i]);
        y += 8;
    }
    render(buf, &frame_area);

    while (true) {
        piscar_led_vermelho();
    }

    return 0;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    
    // Se o evento for do botão A
    if (gpio == PIN_BOTAO_A) {
        // Verifica se passou tempo suficiente (200ms) para o debounce
        if (current_time - last_time_A > 200000) {
            last_time_A = current_time;  // Atualiza o tempo do último evento
            if (contador  < 9 || contador == 9) {
             // Exibe o número correspondente na matriz de LEDs
            switch (contador) {
                case 0: desenho_pio1(desenho0, pio, sm, r, g, b); break;
                case 1: desenho_pio1(desenho1, pio, sm, r, g, b); break;
                case 2: desenho_pio1(desenho2, pio, sm, r, g, b); break;
                case 3: desenho_pio1(desenho3, pio, sm, r, g, b); break;
                case 4: desenho_pio1(desenho4, pio, sm, r, g, b); break;
                case 5: desenho_pio1(desenho5, pio, sm, r, g, b); break;
                case 6: desenho_pio1(desenho6, pio, sm, r, g, b); break;
                case 7: desenho_pio1(desenho7, pio, sm, r, g, b); break;
                case 8: desenho_pio1(desenho8, pio, sm, r, g, b); break;
                case 9: desenho_pio1(desenho9, pio, sm, r, g, b); break;
                default:
                break;
            }
            contador++;  // Primeiro incrementa
            printf("CONTADOR: %d\n", contador);

            }
        }
    }
    
    // Se o evento for do botão B
    if (gpio == PIN_BOTAO_B) {
        // Verifica se passou tempo suficiente (200ms) para o debounce
        if (current_time - last_time_B > 200000) {
            last_time_B = current_time;  // Atualiza o tempo do último evento
            if (contador > 0) {
                // Exibe o número correspondente na matriz de LEDs
            switch (contador) {
                case 2: desenho_pio1(desenho0, pio, sm, r, g, b); break;
                case 3: desenho_pio1(desenho1, pio, sm, r, g, b); break;
                case 4: desenho_pio1(desenho2, pio, sm, r, g, b); break;
                case 5: desenho_pio1(desenho3, pio, sm, r, g, b); break;
                case 6: desenho_pio1(desenho4, pio, sm, r, g, b); break;
                case 7: desenho_pio1(desenho5, pio, sm, r, g, b); break;
                case 8: desenho_pio1(desenho6, pio, sm, r, g, b); break;
                case 9: desenho_pio1(desenho7, pio, sm, r, g, b); break;
                case 10: desenho_pio1(desenho8, pio, sm, r, g, b); break;
                default:
                break;
            }
            printf("CONTADOR: %d\n", contador);
            contador --;


            }
        }
    }
}


