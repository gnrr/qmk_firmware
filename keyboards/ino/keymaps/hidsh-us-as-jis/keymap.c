#include "keymap_jp.h"
#include "action_layer.h"
#include "config.h"
#include "ino.h"

// Layers
enum {LY_MAC   = 0,
      LY_LINUX,
      LY_WIN,
      LY_SHIFT = 15,
      LY_RAISE,
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

#define _SHIFT MO(LY_SHIFT)         // SHIFT key
#define _RAISE MO(LY_RAISE)         // RAISE Key
#define _LOWER MO(LY_LOWER)         // LOWER Key (Not assigned)

#define RETRO_TAPPING
// #define PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT
#define CTL_ESC RCTL_T(KC_ESC)      // Tap: ESC, Hold: R_CTRL

#define HSCRL KC_LSFT               // Enable horizontal scroll during ring-rotation holding this key

enum user_macro {
    UM_HSCRL                        //  Enable horizontal scroll
};

#define PROCESS_OVERRIDE_BEHAVIOR   (false)
#define PROCESS_USUAL_BEHAVIOR      (true)
bool process_record_user(uint16_t keycode, keyrecord_t *record) 
{
  switch (keycode) {
      case KC_LSFT:
          if (record->event.pressed) {
              layer_on(LY_SHIFT);
              add_key(KC_LSHIFT);
              send_keyboard_report();
          }
          else {
              layer_off(LY_SHIFT);
              del_key(KC_LSHIFT);
              send_keyboard_report();
          }
          return PROCESS_OVERRIDE_BEHAVIOR;
          break;
      case JP_AT:
      case JP_CIRC:
      case JP_COLN:
          if (record->event.pressed) {
              del_key(KC_LSHIFT);
              add_key(keycode);
              send_keyboard_report();
          }
          else {
              add_key(KC_LSHIFT);
              del_key(keycode);
              send_keyboard_report();
          }
          return PROCESS_OVERRIDE_BEHAVIOR;
          break;

      case JP_EXLM:
      case JP_HASH:
      case JP_DLR:
      case JP_TILD:
      case JP_PERC:
      case JP_AMPR:
      case JP_ASTR:
      case JP_LPRN:
      case JP_RPRN:
      case JP_UNDS:
      case JP_PLUS:
      case JP_DQT:
          if (record->event.pressed) {
              add_key(keycode);
              send_keyboard_report();
          }
          else {
              del_key(keycode);
              send_keyboard_report();
          }
          return PROCESS_OVERRIDE_BEHAVIOR;
          break;

      default: 
          break;
  }

  return PROCESS_USUAL_BEHAVIOR;
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch(id) {
        case UM_HSCRL:
            enable_horizontal_scroll = true;
            return MACRO_NONE;
        default:
            return MACRO_NONE;
    };
    return MACRO_NONE;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* 
    C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7              C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |   `  |   1  |   2  |   3  |   4  |   5  |   6  |      |          |      |   7  |   8  |   9  |   0  |   -  |   =  |  bs  | R0
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |  tab |   q  |   w  |   e  |   r  |   t  |      |      |          |   y  |   u  |   i  |   o  |   p  |  [   |   ]  |   \  | R1
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |  Z/H |   a  |   s  |   d  |   f  |   g  |      |      |          |   h  |   j  |   k  |   l  |   ;  |   '  | enter|      | R2
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |      |shift |   z  |   x  |   c  |   v  |   b  |      |          |   b  |   n  |   m  |   ,  |   .  |   /  |  up  |  del | R3
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
  |  esc |  win | LOWER|  cmd | space| MS-M | MS-L |      |          | MB-R | space| ctrl | RAISE|  --- | left | down | right| R4
  |      |      |      |  alt |      |      |      |      |          |      |      |      |      |      |      |      |      | 
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
 */

[LY_MAC] = LAYOUT(
  JP_GRV,   KC_1,    KC_2,    KC_3,  KC_4,    KC_5,   KC_6,  xxx,    xxx,    KC_7,   KC_8,    KC_9,    KC_0,    JP_MINS, JP_EQL, KC_BSPC, \
  KC_TAB,   KC_Q,    KC_W,    KC_E,  KC_R,    KC_T,   xxx,   xxx,    KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    JP_LBRC,  JP_RBRC,  JP_YEN, \
  JP_ZHTG,  KC_A,    KC_S,    KC_D,  KC_F,    KC_G,   xxx,   xxx,    KC_H,   KC_J,   KC_K,    KC_L,    JP_SCLN, JP_QUOT,  KC_ENT,   xxx,     \
  xxx,     KC_LSFT,  KC_Z,    KC_X,  KC_C,    KC_V,   KC_B,  xxx,    KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_UP,    KC_DEL,  \
  KC_ESC,  KC_LALT, _LOWER, KC_LWIN, KC_SPC, KC_MSM, KC_MSL,  xxx,   KC_MSR, KC_SPC, CTL_ESC, _RAISE,  xxx,     KC_LEFT,  KC_DOWN,  KC_RIGHT \
  ),

[LY_LINUX] = LAYOUT(
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,   KC_LWIN,   ____, KC_LALT,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____     \
  ),

[LY_WIN] = LAYOUT(
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____,    \
  ____,   KC_LWIN,   ____, KC_LALT,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____     \
  ),

/* Shift
     C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7             C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |      |         |      |   &  |   *  |   (  |   )  |   _  |   +  |      | R0
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |   |  | R1
   +-------------------------------------------------------+         +-------------------------------------------------------+
   | LSFT |      |      |      |      |      |      |      |         |      |      |      |      |   :  |   "  |      |      | R2
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R3
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R4
   +-------------------------------------------------------+         +-------------------------------------------------------+
 */

[LY_SHIFT] = LAYOUT(
  JP_TILD, JP_EXLM,   JP_AT, JP_HASH,JP_DLR, JP_PERC,JP_CIRC,  ____,   ____, JP_AMPR, JP_ASTR, JP_LPRN,JP_RPRN, JP_UNDS,  JP_PLUS,   ____, \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____, \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    JP_COLN, JP_DQT,   ____,     ____, \
  ____,    KC_LSFT,  ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____, \
  ____,    ____,     ____,    ____,  ____,    ____,   ____,   ____,   ____,   ____,   ____,    ____,    ____,    ____,     ____,     ____  \
  ),

/* Raise
     C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7             C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
   +-------------------------------------------------------+         +-------------------------------------------------------+
   | reset|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |      |         |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      | R0
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R1
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R2
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      | pgup |      | R3
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      | RAISE|      |      | pgdn |      | R4
   +-------------------------------------------------------+         +-------------------------------------------------------+
 */

[LY_RAISE] = LAYOUT(
  RESET,   KC_F1,    KC_F2,   KC_F3, KC_F4,  KC_F5,  KC_F6,  xxx,    xxx,    KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      KC_PGUP,  xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    ____,     xxx,     xxx,      KC_PGDN,  xxx      \
  ),

/* Lower
     C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7             C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R0
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R1
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R2
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R3
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R4
   +-------------------------------------------------------+         +-------------------------------------------------------+
 */

[LY_LOWER] = LAYOUT(
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,     ____,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx      \
  )
};
