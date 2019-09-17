#include "report.h"         // report_mouse_t
#include "host.h"           // host_mouse_send
#include "print.h"          // dprintf, print
#include "debug.h"
#include "trackball.h"

#if 0
extern inline void deck_board_led_on(void);
extern inline void deck_right_led_1_on(void);
extern inline void deck_right_led_2_on(void);
extern inline void deck_right_led_3_on(void);
extern inline void deck_right_led_on(uint8_t led);

extern inline void deck_board_led_off(void);
extern inline void deck_right_led_1_off(void);
extern inline void deck_right_led_2_off(void);
extern inline void deck_right_led_3_off(void);
extern inline void deck_right_led_off(uint8_t led);

extern inline void deck_led_all_off(void);

void deck_led_init(void);
void deck_blink_all_leds(void);

void matrix_init_kb(void) {
  deck_led_init();
  deck_blink_all_leds();
  matrix_init_user();
}

void deck_led_init(void)
 {
     DDRB  |=  (1<<PB5 | 1<<PB6 | 1<<PB3);
     PORTB &= ~(1<<PB5 | 1<<PB6 | 1<<PB3);
     DDRB  |=  (1<<PB0);
     PORTB |=  (1<<PB0);
     DDRD  |=  (1<<PB5);
     PORTD |=  (1<<PB5);
}

void deck_blink_all_leds(void)
{
    deck_led_all_off();
    deck_led_all_set(LED_BRIGHTNESS_HI);
    deck_right_led_1_on();
    _delay_ms(50);
    deck_right_led_2_on();
    _delay_ms(50);
    deck_right_led_3_on();
    _delay_ms(50);
    deck_right_led_1_off();
    _delay_ms(50);
    deck_right_led_2_off();
    _delay_ms(50);
    deck_right_led_3_off();
    //deck_led_all_on();
    //_delay_ms(333);
    deck_led_all_off();
}
#endif


#define PIN_RESET   F1
#define PIN_NCS     C7
#define PIN_NOE     F0
 
static report_mouse_t mouseReport = {};
static Trackball tb;

// Disable name mangling in C++ so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_init(void)
{
    dprintf(">> %s\n", __func__);


    bool result = tb.init(PIN_RESET, PIN_NCS, PIN_NOE);
    if(!result) {
        dprintf("  Trackball error: cannot initialize ball sensor\n");
    }

    dprintf("<< %s\n", __func__);
}

void pointing_device_send(void)
{
    dprintf(">> %s\n", __func__);

    //If you need to do other things, like debugging, this is the place to do it.
    host_mouse_send(&mouseReport);
	//send it and 0 it out except for buttons, so those stay until they are explicity over-ridden using update_pointing_device
	mouseReport.x = 0;
	mouseReport.y = 0;
	mouseReport.v = 0;
	mouseReport.h = 0;

    dprintf("<< %s\n", __func__);
}

// Disable name mangling in C++ so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_task(void)
{
    dprintf(">> %s\n", __func__);

	report_mouse_t currentReport = {};

    tb.update();
    int8_t  x = tb.get_dx();
    int8_t  y = tb.get_dy();
    int16_t v = tb.get_scroll();

    dprintf("x,y:%04d,%04d, v:%04d\n", x, y, v);

    currentReport = mouseReport;

    currentReport.x = x;           // pointer -127 .. 127
    currentReport.y = y;           // pointer -127 .. 127
    currentReport.h = 0;
    currentReport.v = v;           // scroll  -127 .. 127
 
    mouseReport = currentReport;
    pointing_device_send();

    dprintf("<< %s\n", __func__);
}

#if 0
void dbg_out_init(void)
{
    //         76543210
    DDRD  |= 0b10011100;        // PD7, PD4, PD3, PD2 for dbg_out
    PORTD &= 0b01100011;        // initialize to 0 (LO)
}

void dbg_out(uint8_t pin, uint8_t level)
{
    assert(pin == 7 || pin == 4 || pin == 3 || pin == 2);

    if(level == 0) {
        PORTD &= ~_BV(pin & 0xF);        // out: 0 (LO)
    }
    else {
        PORTD |=  _BV(pin & 0xF);        // out: 1 (HI)
    }
}
#endif

