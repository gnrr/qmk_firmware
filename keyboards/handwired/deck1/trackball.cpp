#include "print.h"
#include "debug.h"
#include "LUFA/SPI.h"
#include "adns5050.h"
#include "trackball.h"

// static Adns5050 ball_sensor = Adns5050(PIN_RESET, PIN_NCS, PIN_NOE);

// return false:fail, true:success
bool Trackball::init(const uint8_t pin_reset, const uint8_t pin_ncs, const uint8_t pin_noe)
{
    dprintf(">> %s\n", __func__);

    // 500kHz(@fcpu=16MHz), MODE3
    uint8_t opts = SPI_SPEED_FCPU_DIV_32 | SPI_ORDER_MSB_FIRST | SPI_MODE_MASTER
                 | SPI_SCK_LEAD_FALLING | SPI_SAMPLE_TRAILING;

    _connected = _ball_sensor.init(pin_reset, pin_ncs, pin_noe, opts);

    if(_connected) {
        dprintf("  Trackball::init OK: connected\n");
    }
    else {
        dprintf("  Trackball::init error: can not connect to ball sensor\n");
        return false;
    }

    // _ball_sensor.power_down_mode(true);                   // power down mode

    // todo: initialize scroll sensor

    _dx = 0;
    _dy = 0;
    _scroll = 0;

    dprintf("<< %s\n", __func__);
}

void Trackball::update()
{
    dprintf(">> %s\n", __func__);

    if(!_connected) {
        dprintf("  Trackball error: can not connect to ball sensor\n");
        dprintf("<< %s\n", __func__);
        return;
    }

    // _ball_sensor.power_down_mode(false);                   // normal mode
  
    bool moving_p = _ball_sensor.read(REG_MOTION);
  
    if(moving_p) {
        _dx = _ball_sensor.read(REG_DELTA_X);
        _dy = _ball_sensor.read(REG_DELTA_Y);

        dprintf("  dx: %4d   dy: %4d\n", _dx, _dy);
    }
    // _ball_sensor.power_down_mode(true);                   // power down mode

    // todo: read scroll sensor

    dprintf("<< %s\n", __func__);
}

#if 0
void init_ad_hist(int16_t ad, ad_hist_t* ph)
{
    for(uint32_t i=0; i<N_HIST; i++) {
        ph->hist[i] = ad;
    }

    ph->cnt = 0;
}

void trackball_init(analogpad_pointer_t* pad, uint8_t mux_x, uint8_t mux_y)
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

void trackball_read_ball(analogpad_pointer_t* pad, uint8_t max)
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


void trackball_read_ring(analogpad_scroll_t* pad, uint8_t mux_x, uint8_t mux_y)
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

    *pjx = -adjust_scroll(ad_x, &(pad->hist_x), &(pad->state_x)) * max;
    *pjy =  adjust_scroll(ad_y, &(pad->hist_y), &(pad->state_y)) * max;
    
#if 0 // dbg
    // dprintf("x %4d\n", ad_x);
    // dprintf("x:y %4d:%4d\n", ad_x, ad_y);
    dprintf("x:y %4d:%4d  ON/OFF %2d:%2d\n", *pjx, *pjy, state_x, state_y);
#endif
}
#endif
