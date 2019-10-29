#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "quantum.h"
#include "config.h"             // USER_PRINT
#include "print.h"
#include "debug.h"              // dprint


#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

#if 0
void init_deck(void);

inline void deck_right_led_1_off(void) { DDRB &= ~(1<<PB5); PORTB &= ~(1<<PB5); }
inline void deck_right_led_1_on(void) { DDRB |= (1<<PB5); PORTB |= (1<<PB5); }
inline void deck_right_led_2_off(void) { DDRB &= ~(1<<PB6); PORTB &= ~(1<<PB6); }
inline void deck_right_led_2_on(void) { DDRB |= (1<<PB6); PORTB |= (1<<PB6); }
inline void deck_right_led_3_off(void) { DDRB &= ~(1<<PB3); PORTB &= ~(1<<PB3); }
inline void deck_right_led_3_on(void) { DDRB |= (1<<PB3); PORTB |= (1<<PB3); }
inline void deck_right_led_on(uint8_t l) {
    switch (l) {
         case 1:
             deck_right_led_1_on();
             break;
         case 2:
             deck_right_led_2_on();
             break;
         case 3:
             deck_right_led_3_on();
             break;
         default:
             break;
        }
}

inline void deck_right_led_off(uint8_t l) {
    switch (l) {
         case 1:
             deck_right_led_1_off();
             break;
         case 2:
             deck_right_led_2_off();
             break;
         case 3:
             deck_right_led_3_off();
             break;
         default:
             break;
        }
}
inline void deck_board_led_off(void) { DDRB &= ~(1<<PB0); PORTB |= (1<<PB0); }
inline void deck_board_led_on(void) { DDRB |= (1<<PB0); PORTB &= ~(1<<PB0); }
inline void deck_led_all_on(void) {
    deck_right_led_1_on();
    deck_right_led_2_on();
    deck_right_led_3_on();
    deck_board_led_on();
}
inline void deck_led_all_off(void) {
    deck_right_led_1_off();
    deck_right_led_2_off();
    deck_right_led_3_off();
    deck_board_led_off();
}
inline void deck_right_led_1_set(uint8_t n)          {}
inline void deck_right_led_2_set(uint8_t n)          {}
inline void deck_right_led_3_set(uint8_t n)          {}
inline void deck_right_led_set(uint8_t l, uint8_t n) {}
inline void deck_led_all_set(uint8_t n)              {}
#endif

#define LAYOUT( \
    L00, L01, L02, L03, L04, L05, L06, L07,       R00, R01, R02, R03, R04, R05, R06, R07, \
    L10, L11, L12, L13, L14, L15, L16, L17,       R10, R11, R12, R13, R14, R15, R16, R17, \
    L20, L21, L22, L23, L24, L25, L26, L27,       R20, R21, R22, R23, R24, R25, R26, R27, \
    L30, L31, L32, L33, L34, L35, L36, L37,       R30, R31, R32, R33, R34, R35, R36, R37, \
    L40, L41, L42, L43, L44, L45, L46, L47,       R40, R41, R42, R43, R44, R45, R46, R47  \
    ) \
    { \
        { L00, L01, L02, L03, L04, L05, L06, L07 }, \
        { L10, L11, L12, L13, L14, L15, L16, L17 }, \
        { L20, L21, L22, L23, L24, L25, L26, L27 }, \
        { L30, L31, L32, L33, L34, L35, L36, L37 }, \
        { L40, L41, L42, L43, L44, L45, L46, L47 }, \
\
        { R00, R01, R02, R03, R04, R05, R06, R07 }, \
        { R10, R11, R12, R13, R14, R15, R16, R17 }, \
        { R20, R21, R22, R23, R24, R25, R26, R27 }, \
        { R30, R31, R32, R33, R34, R35, R36, R37 }, \
        { R40, R41, R42, R43, R44, R45, R46, R47 }  \
    }

#ifdef __cplusplus
extern "C" {
#endif

void dbg_out_init(void);
void dbg_out(pin_t pin, uint8_t level);

#define dbg_hi(pin)     dbg_out(pin, 1)
#define dbg_lo(pin)     dbg_out(pin, 0)

#ifdef __cplusplus
}
#endif
