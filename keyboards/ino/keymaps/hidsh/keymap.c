#include "action_layer.h"
#include "config.h"
#include "ino.h"

// Layers
enum {LY_MAC   = 0,
      LY_LINUX,
      LY_WIN,
      LY_RAISE = 14,
      LY_LOWER
};

#define xxx  KC_NO
#define ____ KC_TRANSPARENT

#define ID_MS_BTN_L 0               // Mouse buttons
#define ID_MS_BTN_R 1
#define ID_MS_BTN_M 2
#define KC_MSL F(ID_MS_BTN_L)
#define KC_MSR F(ID_MS_BTN_R)
#define KC_MSM F(ID_MS_BTN_M)

#define RAISE MO(LY_RAISE)          // RAISE Key
#define LOWER MO(LY_LOWER)          // LOWER Key (Not assigned)

#define HSCRL KC_LSFT               // Enable horizontal scroll during ring-rotation holding this key

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* 
    C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7              C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |   `  |   1  |   2  |   3  |   4  |   5  |   6  |      |          |      |   7  |   8  |   9  |   0  |   -  |   =  |  bs  | R0
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |  tab |   q  |   w  |   e  |   r  |   t  |      |      |          |   y  |   u  |   i  |   o  |   p  |  [   |   ]  |   \  | R1
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  | caps |   a  |   s  |   d  |   f  |   g  |      |      |          |   h  |   j  |   k  |   l  |   ;  |   '  | enter|      | R2
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |      |shift |   z  |   x  |   c  |   v  |   b  |      |          |   b  |   n  |   m  |   ,  |   .  |   /  |  up  |  del | R3
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |  esc | ctrl |  win |  alt | space| MS-M | MS-L |      |          | MB-R | space|  alt | ctrl | RAISE| left | down | right| R4
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
 */

[LY_MAC] = LAYOUT(
  KC_GRV,   KC_1,    KC_2,    KC_3,  KC_4,    KC_5,   KC_6,  xxx,    xxx,    KC_7,   KC_8,    KC_9,    KC_0,    KC_MINUS, KC_EQUAL, KC_BSPC, \
  KC_TAB,   KC_Q,    KC_W,    KC_E,  KC_R,    KC_T,   xxx,   xxx,    KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC,  KC_RBRC,  KC_BSLS, \
  KC_CAPS,  KC_A,    KC_S,    KC_D,  KC_F,    KC_G,   xxx,   xxx,    KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,  KC_ENT,   xxx,     \
  xxx,      KC_LSFT, KC_Z,    KC_X,  KC_C,    KC_V,   KC_B,  xxx,    KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_UP,    KC_DEL,  \
  KC_ESC,  KC_LCTL, KC_LALT, KC_LWIN,KC_SPC, KC_MSM, KC_MSL, xxx,    KC_MSR, KC_SPC, KC_RWIN, KC_RCTL, RAISE,   KC_LEFT,  KC_DOWN,  KC_RIGHT \
  ),

[LY_LINUX] = LAYOUT(
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,   KC_LWIN, KC_LALT, ____,    ____,   ____,   ____,   ____,   ____,   KC_RCTL, KC_RWIN, ____,    ____,     ____,     ____     \
  ),

[LY_WIN] = LAYOUT(
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,   KC_LWIN, KC_LALT, ____,    ____,   ____,   ____,   ____,   ____,   KC_RCTL, KC_RWIN, ____,    ____,     ____,     ____     \
  ),

/* Raise
     C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7          C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
   +-------------------------------------------------------+      +-------------------------------------------------------+
   | reset|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |      |      |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      | R0
   +-------------------------------------------------------+      +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      | R1
   +-------------------------------------------------------+      +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      | R2
   +-------------------------------------------------------+      +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      | pgup |      | R3
   +-------------------------------------------------------+      +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |      |      |      |      |      | RAISE|      | pgdn |      | R4
   +-------------------------------------------------------+      +-------------------------------------------------------+
 */

[LY_RAISE] = LAYOUT(
  RESET,   KC_F1,    KC_F2,   KC_F3, KC_F4,  KC_F5,  KC_F6,  xxx,    xxx,    KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      KC_PGUP,  xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,    ____,     xxx,      KC_PGDN,  xxx      \
  ),

[LY_LOWER] = LAYOUT(
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,    ____,     xxx,      xxx,      xxx      \
  )
};
