// #include <stdbool.h>
#include <stdlib.h>     // abs
#include "analog.h"      // adc_read
#include "analogpad.h"

#include "debug.h" // dprintf

#define ZERO_THRESH     30
#define CALIBR_THRESH   50
#define POS_MAX         10

// MAX/MIN macro w/ type safe
#define MAX(a,b)                                \
    ({ __typeof__ (a) _a = (a);                 \
        __typeof__ (b) _b = (b);                \
        _a > _b ? _a : _b; })

#define MIN(a,b)                                \
    ({ __typeof__ (a) _a = (a);                 \
        __typeof__ (b) _b = (b);                \
        _a < _b ? _a : _b; })



// from arduino
static long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

}

static int16_t average(int16_t arr[], uint32_t sz)
{
    int32_t sum = 0;

    for(uint32_t i=0; i<sz; i++) {
        sum += arr[i];
    }
  
    return (int16_t)(sum / sz);
}

#if 0
static int16_t deviation(int16_t arr[], uint32_t sz)
{
    int32_t sum = 0;
    int16_t max = INT16_MAX;
    int16_t min = INT16_MIN;
    int16_t av;

    for(uint32_t i=0; i<sz; i++) {
        sum += arr[i];

        max = MIN(max, arr[i]);
        min = MAX(min, arr[i]);
    }

    av = (int16_t)(sum / sz);                      // average
  
    return MAX(abs(max - av), abs(min - av));      // maximum deviation
}
#endif

static int16_t zero_adjust(int16_t ad, ad_hist_t* ph)
{
    // static bool calibrated = false;
    int16_t ad_av;
    int16_t pos;

    int16_t pos_sgn, pos_abs, sign;

    // if(calibrated == false) {
    //     calibrated = true; 

    //     for(uint32_t i=0; i<N_HIST; i++) {
    //         ad_hist[i] = ad;                    // initialize history array
    //     }
    // }

    ad_av = average(ph->hist, N_HIST);

    if(abs(ad - ad_av) < ZERO_THRESH) {
        ph->hist[ph->cnt] = ad;                    // add to history array
        ph->cnt = (ph->cnt + 1) % N_HIST;
        pos = 0;
        pos_abs = 0;
    }
    else {
        pos_sgn = ad - ad_av;                      // signed
        pos_abs  = abs(pos_sgn);
        sign = (pos_sgn < 0)? -1:1;
        pos = (pos_abs - ZERO_THRESH) * sign;
    }

#if 0 // dbg
    {
        enum {rx=0, ry, lx, ly};

        static uint8_t n = 0;

        if(n++ % 4 == rx) {
            dprintf("ad:%4d, cnt:%4d, av:%4d, abs:%4d pos:%4d \n", ad, *pcnt, ad_av, pos_abs, pos);
            // dprintf("ad:%4d, cnt:%4d, av:%4d, sway:%4d pos:%4d cal:%d\n", ad, *pcnt, ad_av, ad_sway, pos, cnt_calibr);
            // dprintf("ad:%4d  sway:%4d  av:%4d  pos_sgn:%4d  pos:%4d\n", ad, ad_sway, ad_av, pos_sgn, pos);
        }
    }
#endif
    
    return pos;
}

void init_ad_hist(int16_t ad, ad_hist_t* ph)
{
    for(uint32_t i=0; i<N_HIST; i++) {
        ph->hist[i] = ad;
    }

    ph->cnt = 0;
}

void init_analogpad_pointer(analogpad_pointer_t* pad, uint8_t mux_x, uint8_t mux_y)
{
    int16_t  ad_x;
    int16_t  ad_y;

    pad->x = 0;
    pad->y = 0;
    pad->adc_mux_x = mux_x;
    pad->adc_mux_y = mux_y;

    adc_read(pad->adc_mux_x);  // dummy read
    adc_read(pad->adc_mux_y);  // dummy read

    ad_x = adc_read(pad->adc_mux_x);
    ad_y = adc_read(pad->adc_mux_y);

    init_ad_hist(ad_x, &(pad->hist_x));
    init_ad_hist(ad_y, &(pad->hist_y));
}

void read_analogpad_pointer(analogpad_pointer_t* pad, uint8_t max)
{
    int16_t* pjx = &(pad->x);
    int16_t* pjy = &(pad->y);
    int16_t  ad_x;
    int16_t  ad_y;

    ad_x = adc_read(pad->adc_mux_x);
    ad_y = adc_read(pad->adc_mux_y);

    // *pjx = ad_x;
    // *pjy = ad_y;
    *pjx = map(zero_adjust(ad_x, &(pad->hist_x)), -510, 510, max, -(max+1));
    *pjy = map(zero_adjust(ad_y, &(pad->hist_y)), -510, 510, max, -(max+1));

    // *pjx = (*pjx * 9 + zero_adjust(ad_x, pcnt_x)) / 10;
    // *pjy = (*pjy * 9 + zero_adjust(ad_y, pcnt_y)) / 10;
}


void init_analogpad_scroll(analogpad_scroll_t* pad, uint8_t mux_x, uint8_t mux_y)
{
    int16_t  ad_x;
    int16_t  ad_y;

    pad->adc_mux_x = mux_x;
    pad->adc_mux_y = mux_y;
    pad->state_x = SWST_IDLE;
    pad->state_y = SWST_IDLE;

    ad_x = adc_read(pad->adc_mux_x);  // dummy read
    ad_y = adc_read(pad->adc_mux_y);  // dummy read

    ad_x = adc_read(pad->adc_mux_x);
    ad_y = adc_read(pad->adc_mux_y);

    init_ad_hist(ad_x, &(pad->hist_x));
    init_ad_hist(ad_y, &(pad->hist_y));
}

static int16_t adjust_scroll(int16_t ad, ad_hist_t* ph, swst_t* state)
{
    const int16_t AD_THR_HI = 450;
    // const int16_t AD_THR_LO = 40;
    int16_t pos = 0;
    int16_t ad_av;
    int16_t ad_abs, sign;

    ad_av = average(ph->hist, N_HIST);

    ad_abs = abs(ad - ad_av);

    switch(*state) {
        case SWST_IDLE:
            if(ad_abs > AD_THR_HI) *state = SWST_ON;
            break;
        case SWST_ON:
#if 0
            if(ad_abs < AD_THR_LO) *state = SWST_OFF;
#else
            sign = (ad - ad_av < 0)? -1:1;
            pos = 1 * sign;
            *state = SWST_IDLE;
#endif
            break;
        case SWST_OFF:
            sign = (ad - ad_av < 0)? -1:1;
            pos = 1 * sign;
            *state = SWST_IDLE;
            break;
        default:
            break;
    }

#if 0 // dbg
    if(pos != 0)
        dprintf("%4d %4d %4d %4d\n", ad, ad_abs, *state, pos);
#endif
    
    return pos;
}
    
void read_analogpad_scroll(analogpad_scroll_t* pad, uint8_t max)
{
    int16_t* pjx = &(pad->x);
    int16_t* pjy = &(pad->y);
    int16_t  ad_x;
    int16_t  ad_y;

    ad_x = adc_read(pad->adc_mux_x);
    ad_y = adc_read(pad->adc_mux_y);

    *pjx = adjust_scroll(ad_x, &(pad->hist_x), &(pad->state_x)) * max;
    *pjy = adjust_scroll(ad_y, &(pad->hist_y), &(pad->state_y)) * max;
    
#if 0 // dbg
    // dprintf("x %4d\n", ad_x);
    // dprintf("x:y %4d:%4d\n", ad_x, ad_y);
    dprintf("x:y %4d:%4d  ON/OFF %2d:%2d\n", *pjx, *pjy, state_x, state_y);
#endif
}

