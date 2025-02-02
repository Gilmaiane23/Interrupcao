#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"

// Configurações da matriz
#define COLUMNS 5
#define ROWS 5
#define NUM_LEDS 25
#define OUT_PIN 7
#define NUM_PIXELS 25

extern PIO pio;  // Declare como extern para ser definida em outro arquivo

void bin_printf(int num);
void animacao(uint8_t repeticoes, uint8_t num_frames, uint32_t desenho[num_frames][NUM_PIXELS], uint sm);
void setup_led_matrix();
void draw(double **desenhoRGB);
void anima(uint8_t num_frames, uint32_t desenho[num_frames][NUM_PIXELS], uint sm);


extern double desenho1[25];
extern double desenho2[25];
extern double desenho3[25];
extern double desenho4[25];
extern double desenho5[25];
extern double desenho6[25];
extern double desenho7[25];
extern double desenho8[25];
extern double desenho9[25];
extern double desenho0[25];
extern uint32_t xadrez[17][NUM_PIXELS];


//void anima(int numero, uint32_t* desenho, uint sm);
void desenho_pio1(double *desenho, PIO pio, uint sm, double r, double g, double b);

#endif // LED_MATRIX_H