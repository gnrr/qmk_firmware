#ifndef DME_H
#define DME_H

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>

#include <assert.h>


#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

#if 1
void init_ergodox(void);

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

#define LAYOUT_4x6( \
    L00, L01, L02, L03, L04, L05,                    R00, R01, R02, R03, R04, R05, \
    L10, L11, L12, L13, L14, L15,                    R10, R11, R12, R13, R14, R15, \
    L20, L21, L22, L23, L24, L25,                    R20, R21, R22, R23, R24, R25, \
              L32, L33,                                        R32, R33,           \
                        L34, L35,                    R30, R31,                     \
                        L44, L45,                    R40, R41,                     \
              L42, L43,                                        R42, R43            \
    ) \
    { \
        { L00,   L01,   L02, L03, L04, L05 }, \
        { L10,   L11,   L12, L13, L14, L15 }, \
        { L20,   L21,   L22, L23, L24, L25 }, \
        { KC_NO, KC_NO, L32, L33, L34, L35 }, \
        { KC_NO, KC_NO, L42, L43, L44, L45 }, \
\
        { R00, R01, R02, R03, R04,   R05   }, \
        { R10, R11, R12, R13, R14,   R15   }, \
        { R20, R21, R22, R23, R24,   R25   }, \
        { R30, R31, R32, R33, KC_NO, KC_NO }, \
        { R40, R41, R42, R43, KC_NO, KC_NO } \
\
   }

#define LAYOUT_5x6( \
	L00, L01, L02, L03, L04, L05,                    R00, R01, R02, R03, R04, R05, \
	L10, L11, L12, L13, L14, L15,                    R10, R11, R12, R13, R14, R15, \
	L20, L21, L22, L23, L24, L25,                    R20, R21, R22, R23, R24, R25, \
	L30, L31, L32, L33, L34, L35,                    R30, R31, R32, R33, R34, R35, \
	          L42, L43,                                        R42, R43,      \
                        L44, L45,                    R40, R41,                \
                        L54, L55,                    R50, R51,                          \
              L52, L53,                                        R52, R53       \
	) \
	{ \
		{ L00, L01, L02, L03, L04, L05 }, \
		{ L10, L11, L12, L13, L14, L15 }, \
		{ L20, L21, L22, L23, L24, L25 }, \
		{ L30, L31, L32, L33, L34, L35 }, \
		{ KC_NO, KC_NO, L42, L43, L44, L45 }, \
		{ KC_NO, KC_NO, L52, L53, L54, L55 }, \
\
		{ R00, R01, R02, R03, R04, R05 },    \
		{ R10, R11, R12, R13, R14, R15 },    \
		{ R20, R21, R22, R23, R24, R25 },    \
		{ R30, R31, R32, R33, R34, R35 },    \
		{ R40, R41, R42, R43, KC_NO, KC_NO },\
		{ R50, R51, R52, R53, KC_NO, KC_NO }, \
   }

#define LAYOUT_5x7( \
		L11, L12, L13, L14, L15, L16, L17, \
		L21, L22, L23, L24, L25, L26, L27, \
		L31, L32, L33, L34, L35, L36, L37, \
		L41, L42, L43, L44, L45, L46, \
		L51, L52, L53, L54,           \
                            L55, L56, \
                            L65, L66, \
		                    L63, L64, \
		R11, R12, R13, R14, R15, R16, R17, \
		R21, R22, R23, R24, R25, R26, R27, \
		R31, R32, R33, R34, R35, R36, R37, \
		     R42, R43, R44, R45, R46, R47, \
		               R54, R55, R56, R57, \
        R52, R53, \
        R62, R63, \
		R64, R65  \
	) \
	{ \
		{ L11, L12, L13, L14, L15, L16, L17 }, \
		{ L21, L22, L23, L24, L25, L26, L27 }, \
		{ L31, L32, L33, L34, L35, L36, L37 }, \
		{ L41, L42, L43, L44, L45, L46, KC_NO }, \
		{ L51, L52, L53, L54, L55, L56, KC_NO }, \
		{ KC_NO, KC_NO, L63, L64, L65, L66, KC_NO }, \
\
		{ R11, R12, R13, R14, R15, R16, R17 }, \
		{ R21, R22, R23, R24, R25, R26, R27 }, \
		{ R31, R32, R33, R34, R35, R36, R37 }, \
		{ KC_NO, R42, R43, R44, R45, R46, R47 }, \
		{ KC_NO, R52, R53, R54, R55, R56, R57 }, \
		{ KC_NO, R62, R63, R64, R65, KC_NO, KC_NO } \
	}

#define LAYOUT_6x6( \
	L00, L01, L02, L03, L04, L05,                    R00, R01, R02, R03, R04, R05, \
	L10, L11, L12, L13, L14, L15,                    R10, R11, R12, R13, R14, R15, \
	L20, L21, L22, L23, L24, L25,                    R20, R21, R22, R23, R24, R25, \
	L30, L31, L32, L33, L34, L35,                    R30, R31, R32, R33, R34, R35, \
	L40, L41, L42, L43, L44, L45,                    R40, R41, R42, R43, R44, R45, \
		          L52, L53,                                        R52, R53,       \
                        L54, L55,                    R50, R51,                \
                        L64, L65,                    R60, R61,                \
              L62, L63,                                        R62, R63       \
	) \
	{ \
		{ L00, L01, L02, L03, L04, L05 }, \
		{ L10, L11, L12, L13, L14, L15 }, \
		{ L20, L21, L22, L23, L24, L25 }, \
		{ L30, L31, L32, L33, L34, L35 }, \
		{ L40, L41, L42, L43, L44, L45 }, \
		{ KC_NO, KC_NO, L52, L53, L54, L55 }, \
		{ KC_NO, KC_NO, L62, L63, L64, L65 }, \
\
		{ R00, R01, R02, R03, R04, R05 },    \
		{ R10, R11, R12, R13, R14, R15 },    \
		{ R20, R21, R22, R23, R24, R25 },    \
		{ R30, R31, R32, R33, R34, R35 },    \
		{ R40, R41, R42, R43, R44, R45 },    \
		{ R50, R51, R52, R53, KC_NO, KC_NO },\
		{ R60, R61, R62, R63, KC_NO, KC_NO }, \
   }

void dbg_out_init(void);
void dbg_out(uint8_t pin, uint8_t level);

#define dbg_hi(pin)     dbg_out(pin, 1)
#define dbg_lo(pin)     dbg_out(pin, 0)

#endif // DME_H
