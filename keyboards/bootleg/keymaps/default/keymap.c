#include "action_layer.h"
#include "config.h"
#include "bootleg.h"

// Fillers to make layering more clear

#define xxx  KC_NO
#define ____ KC_TRANSPARENT

// Layers
enum {LY_MAC = 0,
      LY_LINUX,
      LY_WIN,
      LY_RAISE = 14,
      LY_LOWER
};

//                                     Hold     Tap
#define S_ESC  SFT_T(KC_ESC)        // SHIFT    ESC
#define S_ENT  SFT_T(KC_ENT)        // SHIFT    ENTER
#define C_SPC  CTL_T(KC_SPC)        // CTRL     SPACE
#define C_BSPC CTL_T(KC_BSPC)       // CTRL     BACKSPACE
#define A_SPC  ALT_T(KC_SPC)        // ALT      SPACE
#define W_SPC  WIN_T(KC_SPC)        // WIN      SPACE

#define KC_IME KC_F18               // Toggle input method (also needs setting of the ime)
#define KC_MBL F(0)                 // MOUSE BUTTON L
#define KC_MBR F(1)                 // MOUSE BUTTON R
// #define KC_MBL ACTION_FUNCTION(ID_MS_BTN1)
// #define KC_MBR ACTION_FUNCTION(ID_MS_BTN2)
#define LOWER MO(LY_LOWER)          // LOWER Key
#define RAISE MO(LY_RAISE)          // RAISE Key

// Switch IME with tapping Raise/Lower, thanks to http://leopardgecko.hatenablog.com/entry/2017/11/09/124103
enum user_macro {
    //                                  Hold      Tap
    UM_EISUL,                       //  Lower     EISU,MUHENKAN(JP)
    UM_KANAR                        //  Raise     KANA,HENKAN(JP)
};
#define LWR_EI MACROTAP(UM_EISUL)   // Lower/EISU
#define RIS_KA MACROTAP(UM_KANAR)   // Raise/KANA

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch(id) {
        case UM_EISUL: // Tap:英数,無変換       Hold:Lower
            return MACRO_TAP_HOLD_LAYER( record, MACRO(T(MHEN), T(LANG2), END), LY_LOWER );
        case UM_KANAR: // Tap:かな,変換         Hold:Raise
            return MACRO_TAP_HOLD_LAYER( record, MACRO(T(HENK), T(LANG1), END), LY_RAISE );
    };
    return MACRO_NONE;
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* 
    C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7                  C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
  +-------------------------------------------------------+              +-------------------------------------------------------+ 
  |  esc |   `  |   1  |   2  |   3  |   4  |   5  |   6  |              |      |   7  |   8  |   9  |   0  |   -  |   =  |  bs  | R0
  +-------------------------------------------------------+              +-------------------------------------------------------+
  | pgup |  tab |   q  |   w  |   e  |   r  |   t  |      |              |   y  |   u  |   i  |   o  |   p  |  [   |   ]  |   \  | R1
  +-------------------------------------------------------+              +-------------------------------------------------------+
  | pgdn | caps |   a  |   s  |   d  |   f  |   g  |      |              |   h  |   j  |   k  |   l  |   ;  |   '  | enter| MB-R | R2
  +-------------------------------------------------------+              +-------------------------------------------------------+
  | MB-L |      |shift |   z  |   x  |   c  |   v  |      |              |   b  |   n  |   m  |   ,  |   .  |   /  |  up  |  del | R3
  +-------------------------------------------------------+              +-------------------------------------------------------+
  |      | ctrl |      |      | win  |LOWER |  alt |      |              | shift| ctrl | RAISE|      |      | left | down | right| R4
  +-------------------------------------------------------+              +-------------------------------------------------------+
 */

[LY_MAC] = LAYOUT(
    KC_ESC,  KC_GRV,   KC_1,    KC_2,  KC_3,  KC_4,    KC_5,    KC_6,      xxx,    KC_7,   KC_8,     KC_9,    KC_0,    KC_MINUS, KC_EQUAL, KC_BSPC, \
    KC_PGUP, KC_TAB,   KC_Q,    KC_W,  KC_E,  KC_R,    KC_T,    xxx,       KC_Y,   KC_U,   KC_I,     KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS, \
    KC_PGDN, KC_IME,   KC_A,    KC_S,  KC_D,  KC_F,    KC_G,    xxx,       KC_H,   KC_J,   KC_K,     KC_L,    KC_SCLN, KC_QUOT,  KC_ENT,   KC_MBR,  \
    KC_MBL,  xxx,      KC_LSFT, KC_Z,  KC_X,  KC_C,    KC_V,    xxx,       KC_B,   KC_N,   KC_M,     KC_COMM, KC_DOT,  KC_SLSH,  KC_UP,    KC_DEL,  \
    xxx,     KC_LCTL,  xxx,     xxx,  KC_LALT,LWR_EI,  W_SPC,   xxx,       S_ESC,  C_SPC,  RIS_KA,   xxx,     xxx,     KC_LEFT,  KC_DOWN,  KC_RIGHT \
    ),

[LY_LINUX] = LAYOUT(
    ____,    ____,     ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    ____,     ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    KC_IME,   ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    ____,     ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    ____,     ____,   ____,  KC_LWIN,LOWER,    C_SPC,  ____,      ____,   A_SPC,  RAISE,    ____,    ____,    ____,     ____,     ____  \
    ),

[LY_WIN] = LAYOUT(
    ____,    ____,     ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    ____,     ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    KC_IME,   ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    ____,     ____,   ____,   ____,  ____,     ____,   ____,      ____,   ____,   ____,     ____,    ____,    ____,     ____,     ____, \
    ____,    ____,     ____,   ____,  KC_LWIN,LOWER,    C_SPC,  ____,      ____,   A_SPC,  RAISE,    ____,    ____,    ____,     ____,     ____  \
    ),

/* Raise
    C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7                  C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
   +-------------------------------------------------------+             +-------------------------------------------------------+
   |reset |      |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |             |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      | R0
   +-------------------------------------------------------+             +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |             |      |      |      |      |      |      |      |      | R1
   +-------------------------------------------------------+             +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |             |      |      |      |      |      |      |      |      | R2
   +-------------------------------------------------------+             +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |             |      |      |      |      |      |      |      |      | R3
   +-------------------------------------------------------+             +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |             |      |      |      |      |      |      |      |      | R4
   +-------------------------------------------------------+             +-------------------------------------------------------+
 */

[LY_RAISE] = LAYOUT(
    RESET,   xxx,      KC_F1,  KC_F2,  KC_F3, KC_F4,    KC_F5,  KC_F6,     xxx,    KC_F7,  KC_F8,    KC_F9,   KC_F10,  KC_F11,   KC_F12,   xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   xxx,      xxx,    xxx,       xxx,    xxx,    xxx,      xxx,     xxx,     xxx,      xxx,      xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   xxx,      xxx,    xxx,       xxx,    xxx,    xxx,      xxx,     xxx,     xxx,      xxx,      xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   xxx,      xxx,    xxx,       xxx,    xxx,    xxx,      xxx,     xxx,     xxx,      xxx,      xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   ____,     xxx,    xxx,       xxx,    xxx,    ____,     xxx,     xxx,     xxx,      xxx,      xxx   \
    ),

[LY_LOWER] = LAYOUT(
    xxx,     xxx,      xxx,    xxx,    xxx,   xxx,      xxx,    xxx,       xxx,    xxx,    xxx,      xxx,     xxx,     xxx,      xxx,      xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   xxx,      xxx,    xxx,       xxx,    xxx,    xxx,      xxx,     xxx,     xxx,      xxx,      xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   xxx,      xxx,    xxx,       xxx,    xxx,    xxx,      xxx,     xxx,     xxx,      xxx,      xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   xxx,      xxx,    xxx,       xxx,    xxx,    xxx,      xxx,     xxx,     xxx,      xxx,      xxx,  \
    xxx,     xxx,      xxx,    xxx,    xxx,   ____,     xxx,    xxx,       xxx,    xxx,    ____,     xxx,     xxx,     xxx,      xxx,      xxx   \
    )
};

#undef xxx
#undef ____
