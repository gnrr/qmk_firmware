#pragma once

#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t adc_mux_x;
    uint8_t adc_mux_y;
    uint16_t cnt_x;
    uint16_t cnt_y;
    uint8_t hist_x;
    uint8_t hist_y;
} joypad_t;

void init_joypad(joypad_t* joy, uint8_t mux_x, uint8_t mux_y);
void read_joypad(joypad_t *joy, uint8_t max);

