#include "report.h"         // report_mouse_t
#include "host.h"           // host_mouse_send
#include "asceension.h"

bool i2c_initialized = false;
uint8_t mcp23018_status = 0x20;

// ADC mux for analog joypad (ATMEGA32U4)
#define ADC_MUX_PAD_LY 0x07             // PF7/ADC7
#define ADC_MUX_PAD_LX 0x06             // PF6/ADC6
#define ADC_MUX_PAD_RY 0x05             // PF5/ADC5
#define ADC_MUX_PAD_RX 0x04             // PF4/ADC4

static report_mouse_t mouseReport = {};

#define MAP(x, in_min, in_max, out_min, out_max) \
    ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

analogpad_pointer_t pad_r;
analogpad_scroll_t  pad_l;
        
void pointing_device_init(void)
{
    //initialize device, if that needs to be done.
    debug_enable = true;

    init_analogpad_pointer(&pad_r, ADC_MUX_PAD_RX, ADC_MUX_PAD_RY);
    init_analogpad_scroll(&pad_l, ADC_MUX_PAD_LX, ADC_MUX_PAD_LY);
}

void pointing_device_send(void){
    //If you need to do other things, like debugging, this is the place to do it.
    host_mouse_send(&mouseReport);
	//send it and 0 it out except for buttons, so those stay until they are explicity over-ridden using update_pointing_device
	mouseReport.x = 0;
	mouseReport.y = 0;
	mouseReport.v = 0;
	mouseReport.h = 0;
}

void pointing_device_task(void) {
	report_mouse_t currentReport = {};

    read_analogpad_pointer(&pad_r, 6);
    read_analogpad_scroll(&pad_l, 1);

#if 0
    if(pad_l.x || pad_l.y)
        dprintf("h,v: %d, %d\n", pad_l.x, pad_l.y);
#endif

    currentReport = mouseReport;

    currentReport.x = pad_r.x;           // pointer -127 .. 127
    currentReport.y = pad_r.y;           // pointer -127 .. 127
    currentReport.h = pad_l.x;           // scroll  -127 .. 127
    currentReport.v = pad_l.y;           // scroll  -127 .. 127
 
    mouseReport = currentReport;
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
