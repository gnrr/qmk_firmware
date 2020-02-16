// #include "config.h"         // USER_PRINT
// #include "quantum/pointing_device.h"         // report_mouse_t
// #include "quantum.h"        // IS_LAYER_ON
#include "action_layer.h"   // keyrecord_t 
#include "report.h"         // report_mouse_t
// #include "host.h"           // host_mouse_send
#include "print.h"          // dprintf, print
#include "debug.h"
#include "quantum.h"        // pin_t
#include "trackball.h"
#include "scroll_sensor.h"
#include "bootleg.h"

#if 0
extern inline void bootleg_board_led_on(void);
extern inline void bootleg_right_led_1_on(void);
extern inline void bootleg_right_led_2_on(void);
extern inline void bootleg_right_led_3_on(void);
extern inline void bootleg_right_led_on(uint8_t led);

extern inline void bootleg_board_led_off(void);
extern inline void bootleg_right_led_1_off(void);
extern inline void bootleg_right_led_2_off(void);
extern inline void bootleg_right_led_3_off(void);
extern inline void bootleg_right_led_off(uint8_t led);

extern inline void bootleg_led_all_off(void);

void bootleg_led_init(void);
void bootleg_blink_all_leds(void);

void matrix_init_kb(void) {
  bootleg_led_init();
  bootleg_blink_all_leds();
  matrix_init_user();
}

void bootleg_led_init(void)
 {
     DDRB  |=  (1<<PB5 | 1<<PB6 | 1<<PB3);
     PORTB &= ~(1<<PB5 | 1<<PB6 | 1<<PB3);
     DDRB  |=  (1<<PB0);
     PORTB |=  (1<<PB0);
     DDRD  |=  (1<<PB5);
     PORTD |=  (1<<PB5);
}

void bootleg_blink_all_leds(void)
{
    bootleg_led_all_off();
    bootleg_led_all_set(LED_BRIGHTNESS_HI);
    bootleg_right_led_1_on();
    _delay_ms(50);
    bootleg_right_led_2_on();
    _delay_ms(50);
    bootleg_right_led_3_on();
    _delay_ms(50);
    bootleg_right_led_1_off();
    _delay_ms(50);
    bootleg_right_led_2_off();
    _delay_ms(50);
    bootleg_right_led_3_off();
    //bootleg_led_all_on();
    //_delay_ms(333);
    bootleg_led_all_off();
}
#endif

bool enable_horizontal_scroll;           // true: horizontal scroll, false: vertical scroll

// static report_mouse_t mouseReport = {};
static Trackball tb;
static ScrollSensor scroll;

// Disable name mangling so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_init(void)
{
    // debug_enable = true;
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    bool result = scroll.init();
    if(!result) {
        print("  ScrollSensor error: cannot initialize scroll sensor\n");
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return;                                     // abend
    }

    result = tb.init(PIN_RESET, PIN_CS, PIN_OE);                   // todo pin defines --> this .c
    if(!result) {
        print("  Trackball error: cannot initialize ball sensor\n");
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return;                                     // abend
    }

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

#if 0
void pointing_device_send(void)
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    //If you need to do other things, like debugging, this is the place to do it.
    host_mouse_send(&mouseReport);
	//send it and 0 it out except for buttons, so those stay until they are explicity over-ridden using update_pointing_device
	mouseReport.x = 0;
	mouseReport.y = 0;
	mouseReport.v = 0;
	mouseReport.h = 0;

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}
#endif

extern "C" report_mouse_t pointing_device_get_report(void);
extern "C" void pointing_device_set_report(report_mouse_t newMouseReport);
extern "C" void pointing_device_send();

// Disable name mangling so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_task(void)
{
    // debug_enable = true;
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    tb.update();
    int8_t x = tb.get_dx();
    int8_t y = tb.get_dy();
    int8_t s;
    if((x == 0) && (y == 0)) {
        // dprintf("stat:%d\t", scroll.get_status());
        s = scroll.get();
        // dprintf("s:%d\n", s);
    }
    else {
        scroll.clear();     // not accept scrolling while moving pointer
        s = 0;
    }

	report_mouse_t currentReport = pointing_device_get_report();

    currentReport.x = x;                                // pointer x -127 .. 127
    currentReport.y = y;                                // pointer y -127 .. 127
    currentReport.v = (enable_horizontal_scroll)? 0:s;  // scroll  v -127 .. 127
    currentReport.h = (enable_horizontal_scroll)? s:0;  // scroll  h -127 .. 127
    debug_enable = true;
    // dprintf("s:%2d, h:%2d, -h:%2d\n", s, currentReport.h, -currentReport.h);
    debug_enable = false;
    enable_horizontal_scroll = false;

	pointing_device_set_report(currentReport);
    pointing_device_send();

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_FUNCTION(0),  // mouse_button_L
    [1] = ACTION_FUNCTION(1),  // mouse_button_R
};

extern "C"
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    // Just write mouse-buttons status into currentReport without sending mouse-report.
    // The mouse-report is sent with x, y data by pointing_device_task afterwards.
    // MOUSEKEY sends the mouse-report to the host at the time so it cannot be used in this case. 
    // Because using MOUSEKEY occurs mouse-report sending twice at a main-loop.   

    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

	report_mouse_t currentReport = pointing_device_get_report();

    switch (id) {
        case 0:
            if (record->event.pressed) {
                currentReport.buttons |=  MOUSE_BTN1;
            } else {
                currentReport.buttons &= ~MOUSE_BTN1;
            }
            break;

        case 1: 
            if(record->event.pressed) {
                currentReport.buttons |=  MOUSE_BTN2;
            } else {
                currentReport.buttons &= ~MOUSE_BTN2;
            }
            break;

        default:
            break;
    }
	pointing_device_set_report(currentReport);
    pointing_device_send();

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

#if 0
pin_t dbg_out_pins[] = {D2, D3};           // PD2, PD3 for dbg_out
#define DBG_OUT_PIN_SZ   sizeof(dbg_out_pins)/sizeof(dbg_out_pins[0])
void dbg_out_init(void)
{
    for(uint8_t i=0; i<DBG_OUT_PIN_SZ; i++) {
        pin_t pin = dbg_out_pins[i];
        setPinOutput(pin);              // output
        writePinLow(pin);               // --> LO
    }
}

void dbg_out(pin_t pin, uint8_t level)
{
    // assert(pin == D2 || pin == D3);

    if(level == 0) {
        writePinLow(pin);               // --> LO
    }
    else {
        writePinHigh(pin);              // --> HI
    }
}
#endif

