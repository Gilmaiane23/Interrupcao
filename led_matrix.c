#include "led_matrix.h"
#include "pio_matrix.pio.h"

PIO pio = pio0;
#define FPS 500

//uint sm;  // Variável para armazenar o número da máquina de estados
//const uint botaoA_pin = 5;  //Botão = A, Botão = B
// Defina o número de pixels
#define NUM_PIXELS 25


// Função para converter brilho e cor em um valor 32 bits
uint32_t matrix_rgb(double brilho, double r, double g, double b) {
    unsigned char R = (unsigned char)(r * brilho * 255);
    unsigned char G = (unsigned char)(g * brilho * 255);
    unsigned char B = (unsigned char)(b * brilho * 255);
    return (G << 24) | (R << 16) | (B << 8);
}

// Inicializa a matriz de LEDs
void setup_led_matrix() {
    bool ok;
    // Configura o clock para 128 MHz
    ok = set_sys_clock_khz(128000, false);
    stdio_init_all();

    printf("Iniciando a transmissão PIO\n");
    if (ok) printf("Clock configurado para %ld Hz\n", clock_get_hz(clk_sys));

    // Configuração do PIO
    uint offset = pio_add_program(pio, &pio_matrix_program); //carrega o programa PIO para controle dos LEDs
    uint sm = pio_claim_unused_sm(pio, true);  // aloca uma máquina de estado disponível no PIO.
    pio_matrix_program_init(pio, sm, offset, OUT_PIN); //configura os pinos de saída (OUT_PIN).
}


void desenho_pio1(double *desenho, PIO pio, uint sm, double r, double g, double b)
{ 
    for (int16_t i = 0; i < NUM_PIXELS; i++)
    {
        uint32_t valor_led = matrix_rgb(desenho[24 - i], r, g, b);
        pio_sm_put_blocking(pio, sm, valor_led);
    }
}


uint8_t obter_index(uint8_t i) {
    uint8_t x = i % 5;  // Coluna
    uint8_t y = i / 5;  // Linha
    return (y % 2 == 0) ? (y * 5 + x) : (y * 5 + (4 - x));
}

void draw(double **desenhoRGB) {
    uint sm;  // Variável para armazenar o número da máquina de estados
    uint32_t led_value;
    for (int16_t i = 0; i < NUM_LEDS; i++) {
        led_value = matrix_rgb(
            1.0, // Aqui você define o brilho. Altere o valor conforme necessário.
            convertDrawingToMatrixIndex(desenhoRGB[0], NUM_LEDS - 1 - i),
            convertDrawingToMatrixIndex(desenhoRGB[1], NUM_LEDS - 1 - i),
            convertDrawingToMatrixIndex(desenhoRGB[2], NUM_LEDS - 1 - i)
        );
        pio_sm_put_blocking(pio0, sm, led_value);
    }
    bin_printf(led_value);
}


void anima(uint8_t num_frames, uint32_t desenho[num_frames][NUM_PIXELS], uint sm) {
    for (uint8_t i = 0; i < num_frames; i++) {
        for (uint8_t j = 0; j < NUM_PIXELS; j++) {
            uint8_t pos = obter_index(j);
            uint32_t valor_led = desenho[i][24 - pos];
            pio_sm_put_blocking(pio0, sm, valor_led);
        }
        sleep_ms(200);
    }
}

void animacao(uint8_t repeticoes,uint8_t num_frames,uint32_t desenho[num_frames][NUM_PIXELS],uint sm){
    for(uint8_t i=0;i<repeticoes;i++){
        anima(num_frames,desenho,sm);
    }
}



    //vetor para criar imagem na matriz de led - A
double desenho1[25] = {0.0, 0.0, 0.2, 0.0, 0.0,
                        0.0, 0.0, 0.2, 0.2, 0.0, 
                        0.0, 0.0, 0.2, 0.0, 0.0,
                        0.0, 0.0, 0.2, 0.0, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};

double desenho2[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.0, 0.0, 
                        0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.0, 0.0, 0.2, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};

double desenho3[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.0, 0.0, 
                        0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.0, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};

double desenho4[25] = {0.0, 0.2, 0.0, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.2, 0.0, 
                        0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.2, 0.0};

double desenho5[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.0, 0.0, 0.2, 0.0, 
                        0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.0, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};

double desenho6[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.0, 0.0, 0.2, 0.0, 
                        0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.2, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};


double desenho7[25] = { 0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.0, 0.0, 
                        0.0, 0.0, 0.2, 0.0, 0.0,
                        0.0, 0.0, 0.0, 0.2, 0.0,
                        0.2, 0.0, 0.0, 0.0, 0.0};

double desenho8[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.2, 0.0, 
                        0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.2, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};

double desenho9[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.2, 0.0, 
                        0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.0, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};

double desenho0[25] = {0.0, 0.2, 0.2, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.2, 0.0, 
                        0.0, 0.2, 0.0, 0.2, 0.0,
                        0.0, 0.2, 0.0, 0.2, 0.0,
                        0.0, 0.2, 0.2, 0.2, 0.0};

uint32_t xadrez[17][NUM_PIXELS] = {
{
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100
},
{
0x1010100, 0x1010100, 0x1010100, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100
},
{
0x1010100, 0x1010100, 0x190000, 0x19000000, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100
},
{
0x1010100, 0x190000, 0x19000000, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x190000, 0x19000000, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x1010100
},
{
0x190000, 0x19000000, 0x190000, 0x19000000, 0x190000, 
0x1010100, 0x190000, 0x19000000, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x190000, 0x19000000, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x1010100, 0x1010100, 0x190000
},
{
0x19000000, 0x190000, 0x19000000, 0x190000, 0x19000000, 
0x190000, 0x19000000, 0x190000, 0x19000000, 0x190000, 
0x1010100, 0x190000, 0x19000000, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x190000, 0x19000000, 0x190000, 
0x1010100, 0x1010100, 0x1010100, 0x190000, 0x19000000
},
{
0x190000, 0x19000000, 0x190000, 0x19000000, 0x190000, 
0x19000000, 0x190000, 0x19000000, 0x190000, 0x19000000, 
0x190000, 0x19000000, 0x190000, 0x19000000, 0x190000, 
0x1010100, 0x190000, 0x19000000, 0x190000, 0x19000000, 
0x1010100, 0x1010100, 0x190000, 0x19000000, 0x190000
},
};
