#include "config.h"         // USER_PRINT
// #include "quantum/pointing_device.h"         // report_mouse_t
// #include "quantum.h"        // IS_LAYER_ON
#include "action_layer.h"   // keyrecord_t 
#include "report.h"         // report_mouse_t
#include "host.h"           // host_mouse_send
#include "print.h"          // dprintf, print
#include "debug.h"
#include "quantum.h"        // pin_t
#include "trackball.h"
#include "deck.h"

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

// static report_mouse_t mouseReport = {};
static Trackball tb;


// Disable name mangling in C++ so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_init(void)
{
    // debug_enable = true;
    dprintf(">> %s\n", __PRETTY_FUNCTION__);

    bool result = tb.init(PIN_RESET, PIN_CS, PIN_OE);
    if(!result) {
        print("  Trackball error: cannot initialize ball sensor\n");
        dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return;                                     // abend
    }

    dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

#if 0
void pointing_device_send(void)
{
    dprintf(">> %s\n", __PRETTY_FUNCTION__);

    //If you need to do other things, like debugging, this is the place to do it.
    host_mouse_send(&mouseReport);
	//send it and 0 it out except for buttons, so those stay until they are explicity over-ridden using update_pointing_device
	mouseReport.x = 0;
	mouseReport.y = 0;
	mouseReport.v = 0;
	mouseReport.h = 0;

    dprintf("<< %s\n", __PRETTY_FUNCTION__);
}
#endif

extern "C" report_mouse_t pointing_device_get_report(void);
extern "C" void pointing_device_set_report(report_mouse_t newMouseReport);
extern "C" void pointing_device_send();

// Disable name mangling in C++ so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_task(void)
{
    dprintf(">> %s\n", __PRETTY_FUNCTION__);

	report_mouse_t currentReport = pointing_device_get_report();

    tb.update();

    currentReport.x = tb.get_dx();            // pointer x -127 .. 127
    currentReport.y = tb.get_dy();            // pointer y -127 .. 127
    currentReport.v = tb.get_scroll();        // scroll  v -127 .. 127
    currentReport.h = 0;                      // scroll  h -127 .. 127 

	pointing_device_set_report(currentReport);
    pointing_device_send();

    dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

// const uint16_t PROGMEM fn_actions[] = {
    // [0] = ACTION_FUNCTION(ID_MS_BTN1),
    // [1] = ACTION_FUNCTION(ID_MS_BTN2),
// };
#if 1
const uint16_t PROGMEM fn_actions[] = {
  [0] = ACTION_FUNCTION(0),  // Calls action_function()
  [1] = ACTION_FUNCTION(1),  // Calls action_function()
};
#endif

extern "C"
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
#if 1
    
    // dbg_hi(D3);
    // debug_enable = true;
    dprintf(">> %s\n", __PRETTY_FUNCTION__);

	report_mouse_t currentReport = pointing_device_get_report();

    switch (id) {
        case 0:
            if (record->event.pressed) {
                // dbg_hi(D4);
                currentReport.buttons |=  MOUSE_BTN1;
            } else {
                // dbg_lo(D4);
                currentReport.buttons &= ~MOUSE_BTN1;
            }
            break;

        case 1: 
            if(record->event.pressed) {
                // dbg_hi(D4);
                currentReport.buttons |=  MOUSE_BTN2;
            } else {
                // dbg_lo(D4);
                currentReport.buttons &= ~MOUSE_BTN2;
            }
            break;

        default:
            break;
    }
	pointing_device_set_report(currentReport);
    pointing_device_send();

    dprintf("<< %s\n", __PRETTY_FUNCTION__);
    // dbg_lo(D3);
#endif
}

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

