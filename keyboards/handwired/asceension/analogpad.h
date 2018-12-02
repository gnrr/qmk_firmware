#pragma once

#include "asceension.h"

#define  N_HIST 100  // should be <= 65538 (= UINT32_MAX / UINT16_MAX)

typedef struct {
    int16_t hist[N_HIST];
    int16_t cnt;
} ad_hist_t;

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t adc_mux_x;
    uint8_t adc_mux_y;
    ad_hist_t hist_x;
    ad_hist_t hist_y;
} analogpad_pointer_t;

typedef enum {SWST_IDLE = 0, SWST_ON, SWST_OFF} swst_t;

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t adc_mux_x;
    uint8_t adc_mux_y;
    swst_t state_x;
    swst_t state_y;
    ad_hist_t hist_x;
    ad_hist_t hist_y;
} analogpad_scroll_t;


void init_analogpad_pointer(analogpad_pointer_t* joy, uint8_t mux_x, uint8_t mux_y);
void read_analogpad_pointer(analogpad_pointer_t *joy, uint8_t max);

void init_analogpad_scroll(analogpad_scroll_t* pad, uint8_t mux_x, uint8_t mux_y);
void read_analogpad_scroll(analogpad_scroll_t* pad, uint8_t max);
