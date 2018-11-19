#include "i2cmaster.h"
#include "quantum/pointing_device.h"
#include "analog_joypad.h"
#include "asceension.h"

#include "debug.h"

bool i2c_initialized = false;
uint8_t mcp23018_status = 0x20;

// ADC mux for analog joypad (ATMEGA32U4)
#define ADC_MUX_JOY_LY 0x07             // PF7/ADC7
#define ADC_MUX_JOY_LX 0x06             // PF6/ADC6
#define ADC_MUX_JOY_RY 0x05             // PF5/ADC5
#define ADC_MUX_JOY_RX 0x04             // PF4/ADC4

#define MAP(x, in_min, in_max, out_min, out_max) \
    ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

joypad_t jpad_l, jpad_r;
        
void pointing_device_init(void)
{
    //initialize device, if that needs to be done.
    debug_enable = true;

    init_joypad(&jpad_l, ADC_MUX_JOY_LX, ADC_MUX_JOY_LY);
    init_joypad(&jpad_r, ADC_MUX_JOY_RX, ADC_MUX_JOY_RY);
}

void pointing_device_task(void) {
    static uint8_t cnt = 0;

    if(cnt == 0)
        read_joypad(&jpad_l, 1);
    cnt = (cnt + 1) % 100;

    read_joypad(&jpad_r, 9);

	report_mouse_t currentReport = {};
    currentReport = pointing_device_get_report();

#if 0 // dbg
    // dprintf("LX,LY:%5d, %5d     RX,RY:%5d, %5d\n", jpad_l.x, jpad_l.y, jpad_r.x, jpad_r.y);
#else
    currentReport.x = jpad_r.x;           // 127 max -127 min
    currentReport.y = jpad_r.y;           // 127 max -127 min
    currentReport.h = jpad_l.x;           // 127 max -127 min (scroll horizontal)
    currentReport.v = jpad_l.y;           // 127 max -127 min (scroll vertical)
#endif
 
    pointing_device_set_report(currentReport);

    pointing_device_send();
}

uint8_t init_mcp23018(void)
{
    mcp23018_status = 0x20;

    /* I2C subsystem */

    if (!i2c_initialized) {
        i2c_init();  // on pins D(1,0)
        i2c_initialized = true;
        _delay_ms(100);
    }

    /* MCP2318 PORT A/B Pins are set all input /w pullup as Col */

    // pullup
    mcp23018_status = i2c_start(I2C_ADDR, I2C_WRITE); if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(GPPUA);               if (mcp23018_status) goto out;
    mcp23018_status = i2c_write(0b11111111);          if (mcp23018_status) goto out;
    /* Now write to GPPUB */
    mcp23018_status = i2c_write(0b11111111);          if (mcp23018_status) goto out;

out:
    i2c_stop();

    return mcp23018_status;
}
