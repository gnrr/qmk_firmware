#ifndef DME_H
#define DME_H

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

void init_ergodox(void);

#if 1
inline void ergodox_right_led_1_off(void) { DDRB &= ~(1<<PB5); PORTB &= ~(1<<PB5); }
inline void ergodox_right_led_1_on(void) { DDRB |= (1<<PB5); PORTB |= (1<<PB5); }
inline void ergodox_right_led_2_off(void) { DDRB &= ~(1<<PB6); PORTB &= ~(1<<PB6); }
inline void ergodox_right_led_2_on(void) { DDRB |= (1<<PB6); PORTB |= (1<<PB6); }
inline void ergodox_right_led_3_off(void) { DDRB &= ~(1<<PB3); PORTB &= ~(1<<PB3); }
inline void ergodox_right_led_3_on(void) { DDRB |= (1<<PB3); PORTB |= (1<<PB3); }
inline void ergodox_right_led_on(uint8_t l) {
    switch (l) {
         case 1:
             ergodox_right_led_1_on();
             break;
         case 2:
             ergodox_right_led_2_on();
             break;
         case 3:
             ergodox_right_led_3_on();
             break;
         default:
             break;
        }
}

inline void ergodox_right_led_off(uint8_t l) {
    switch (l) {
         case 1:
             ergodox_right_led_1_off();
             break;
         case 2:
             ergodox_right_led_2_off();
             break;
         case 3:
             ergodox_right_led_3_off();
             break;
         default:
             break;
        }
}
inline void ergodox_board_led_off(void) { DDRB &= ~(1<<PB0); PORTB |= (1<<PB0); }
inline void ergodox_board_led_on(void) { DDRB |= (1<<PB0); PORTB &= ~(1<<PB0); }
inline void ergodox_led_all_on(void) {
    ergodox_right_led_1_on();
    ergodox_right_led_2_on();
    ergodox_right_led_3_on();
    ergodox_board_led_on();
}
inline void ergodox_led_all_off(void) {
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    ergodox_board_led_off();
}
inline void ergodox_right_led_1_set(uint8_t n)          {}
inline void ergodox_right_led_2_set(uint8_t n)          {}
inline void ergodox_right_led_3_set(uint8_t n)          {}
inline void ergodox_right_led_set(uint8_t l, uint8_t n) {}
inline void ergodox_led_all_set(uint8_t n)              {}
#endif

#define LAYOUT_4x5( \
    L00, L01, L02, L03, L04,                      R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,                      R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,                      R20, R21, R22, R23, R24, \
         L31, L32,                                          R32, R33,      \
                   L33, L34,                      R30, R31,                \
                             L44, L43,  R41, R40,                          \
                             L42, L41,  R43, R42                           \
    ) \
    { \
        { L00, L01, L02, L03, L04 }, \
        { L10, L11, L12, L13, L14 }, \
        { L20, L21, L22, L23, L24 }, \
        { KC_NO, L31, L32, L33, L34 }, \
        { KC_NO, L41, L42, L43, L44 }, \
\
        { R04, R03, R02, R01, R00 },    \
        { R14, R13, R12, R11, R10 },    \
        { R24, R23, R22, R21, R20 },    \
        { KC_NO, R33, R32, R31, R30 },  \
        { KC_NO, R43, R42, R41, R40 }  \
    }



// #define LAYOUT LAYOUT_4x5


#endif // DME_H
