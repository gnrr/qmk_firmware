#include <stdbool.h>
#include <stdlib.h>     // abs
#include "analog.h"      // adc_read
#include "analog_joypad.h"

#include "debug.h"

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


#define  N_HIST 100  // should be <= 65538 (= UINT32_MAX / UINT16_MAX)
int16_t  ad_hist[N_HIST];

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

    av = (int16_t)(sum / sz);                   // average
  
    return MAX(abs(max - av), abs(min - av));   // maximum deviation
}
#endif

int16_t zero_adjust(int16_t ad, uint16_t* pcnt)
{
    static bool calibrated = false;
    int16_t ad_av;
    int16_t pos;

    int16_t pos_sgn, pos_abs, sign;

    if(calibrated == false) {
        calibrated = true; 

        for(uint32_t i=0; i<N_HIST; i++) {
            ad_hist[i] = ad;                    // initialize history array
        }
    }

    ad_av = average(ad_hist, N_HIST);

    if(abs(ad - ad_av) < ZERO_THRESH) {
        ad_hist[*pcnt] = ad;                    // add to history array
        *pcnt = (*pcnt + 1) % N_HIST;
        pos = 0;
        pos_abs = 0;
    }
    else {
        pos_sgn = ad - ad_av;                   // signed
        pos_abs  = abs(pos_sgn);
        sign = (pos_sgn < 0)? -1:1;
        pos = (pos_abs - ZERO_THRESH) * sign;
    }

    // dbg
    {
        enum {rx=0, ry, lx, ly};

        static uint8_t n = 0;

        if(n++ % 4 == rx) {
            dprintf("ad:%4d, cnt:%4d, av:%4d, abs:%4d pos:%4d \n", ad, *pcnt, ad_av, pos_abs, pos);
            // dprintf("ad:%4d, cnt:%4d, av:%4d, sway:%4d pos:%4d cal:%d\n", ad, *pcnt, ad_av, ad_sway, pos, cnt_calibr);
            // dprintf("ad:%4d  sway:%4d  av:%4d  pos_sgn:%4d  pos:%4d\n", ad, ad_sway, ad_av, pos_sgn, pos);
        }
    }
    return pos;
}  

void init_joypad(joypad_t* joy, uint8_t mux_x, uint8_t mux_y)
{
    joy->x = 0;
    joy->y = 0;
    joy->adc_mux_x = mux_x;
    joy->adc_mux_y = mux_y;
    joy->cnt_x = 0;
    joy->cnt_y = 0;
}

void read_joypad(joypad_t* joy, uint8_t max)
{
    int16_t* pjx = &(joy->x);
    int16_t* pjy = &(joy->y);
    uint16_t* pcnt_x = &(joy->cnt_x);
    uint16_t* pcnt_y = &(joy->cnt_y);
    int16_t  adc_x;
    int16_t  adc_y;

    adc_x = adc_read(joy->adc_mux_x);
    adc_y = adc_read(joy->adc_mux_y);

    // *pjx = adc_x;
    // *pjy = adc_y;
    *pjx = map(zero_adjust(adc_x, pcnt_x), -510, 510, max, -(max+1));
    *pjy = map(zero_adjust(adc_y, pcnt_y), -510, 510, max, -(max+1));

    // *pjx = (*pjx * 9 + zero_adjust(adc_x, pcnt_x)) / 10;
    // *pjy = (*pjy * 9 + zero_adjust(adc_y, pcnt_y)) / 10;
}
