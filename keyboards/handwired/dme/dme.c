#include "dme.h"

#if 0
extern inline void ergodox_board_led_on(void);
extern inline void ergodox_right_led_1_on(void);
extern inline void ergodox_right_led_2_on(void);
extern inline void ergodox_right_led_3_on(void);
extern inline void ergodox_right_led_on(uint8_t led);

extern inline void ergodox_board_led_off(void);
extern inline void ergodox_right_led_1_off(void);
extern inline void ergodox_right_led_2_off(void);
extern inline void ergodox_right_led_3_off(void);
extern inline void ergodox_right_led_off(uint8_t led);

extern inline void ergodox_led_all_off(void);

void ergodox_led_init(void);
void ergodox_blink_all_leds(void);

void matrix_init_kb(void) {
  ergodox_led_init();
  ergodox_blink_all_leds();
  matrix_init_user();
}

void ergodox_led_init(void)
 {
     DDRB  |=  (1<<PB5 | 1<<PB6 | 1<<PB3);
     PORTB &= ~(1<<PB5 | 1<<PB6 | 1<<PB3);
     DDRB  |=  (1<<PB0);
     PORTB |=  (1<<PB0);
     DDRD  |=  (1<<PB5);
     PORTD |=  (1<<PB5);
}

void ergodox_blink_all_leds(void)
{
    ergodox_led_all_off();
    ergodox_led_all_set(LED_BRIGHTNESS_HI);
    ergodox_right_led_1_on();
    _delay_ms(50);
    ergodox_right_led_2_on();
    _delay_ms(50);
    ergodox_right_led_3_on();
    _delay_ms(50);
    ergodox_right_led_1_off();
    _delay_ms(50);
    ergodox_right_led_2_off();
    _delay_ms(50);
    ergodox_right_led_3_off();
    //ergodox_led_all_on();
    //_delay_ms(333);
    ergodox_led_all_off();
}
#endif

// thx to - http://graphics.stanford.edu/~seander/bithacks.html#ReverseByteWith32Bits
inline
uint8_t reverse_bits(uint8_t b)
{
    return (uint8_t)(((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16); 
}

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
