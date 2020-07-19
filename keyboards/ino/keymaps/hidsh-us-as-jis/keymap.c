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

// #define RAISE MO(LY_RAISE)          // RAISE Key
// #define LOWER MO(LY_LOWER)          // LOWER Key (Not assigned)

#define HSCRL KC_LSFT               // Enable horizontal scroll during ring-rotation holding this key

// Switch IME with tapping Raise/Lower, thanks to http://leopardgecko.hatenablog.com/entry/2017/11/09/124103
enum user_macro {
    //                                  Hold      Tap
    UM_L_EIS,                       //  Lower     EISU,MUHENKAN(JP)
    UM_R_KAN,                       //  Raise     KANA,HENKAN(JP)
    UM_L_SPC,                       //  Lower     Space
    UM_R_SPC,                       //  Raise     Space
    UM_HSCRL                        //  Enable horizontal scroll
};
#define L_EISU MACROTAP(UM_L_EIS)   // Lower/EISU
#define R_KANA MACROTAP(UM_R_KAN)   // Raise/KANA
#define L_SPC  MACROTAP(UM_L_SPC)   // Lower/Space
#define R_SPC  MACROTAP(UM_R_SPC)   // Raise/Space

#define US_EISU  LSFT(KC_CAPS)      // EISU on US layout

#if 0
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    switch(id) {
#if 0
        case UM_L_EIS:              // Hold:Lower   Tap:EISU,MUHENKAN
            return MACRO_TAP_HOLD_LAYER( record, MACRO(T(MHEN), T(LANG2), END), LY_LOWER );
        case UM_R_KAN:              // Hold:Raise   Tap:KANA,HENKAN
            return MACRO_TAP_HOLD_LAYER( record, MACRO(T(HENK), T(LANG1), END), LY_RAISE );

        case UM_L_SPC:              // Hold:Lower   Tap:Space
            return MACRO_TAP_HOLD_LAYER( record, MACRO(T(MHEN), T(LANG2), END), LY_LOWER );
        case UM_R_SPC:              // Hold:Raise   Tap:Space
            return MACRO_TAP_HOLD_LAYER( record, MACRO(T(HENK), T(LANG1), END), LY_RAISE );
#endif

        case UM_HSCRL:
            enable_horizontal_scroll = true;
            return MACRO_NONE;
        default:
            return MACRO_NONE;
    };
    return MACRO_NONE;
}
#endif

#if 1 // US layout but act as JIS Layout
// thx to m47ch4n
// https://github.com/m47ch4n/qmk_firmware/blob/add-m47ch4ns-keymap/keyboards/ergodash/rev1/keymaps/m47ch4n/keymap.c

#include QMK_KEYBOARD_H
// #include "keymap_jp.h"

// clang-format off
enum layers { _QWERTY, _LOWER, _RAISE, _ADJUST };

enum converting_keycodes_us {
    JP = SAFE_RANGE, US, UBUNTU, MAC, SHIFT, LOWER, RAISE, ADJUST,
    /* for physical layout: US */
    CNV_GRV, CNV_0, CNV_1, CNV_2, CNV_3, CNV_4, CNV_5, CNV_6, CNV_7, CNV_8, CNV_9, CNV_MINS, CNV_EQL,
    CNV_LBRC, CNV_RBRC, CNV_BSLS, CNV_SCLN, CNV_QUOT,
    /* CNV_COMM, CNV_DOT, CNV_SLSH, */
    AT,
    DESKDN, DESKUP
};

enum converting_keycodes_jis {
    /* for physical layout: JIS or some customized keycodes */
    CNV_TILD, CNV_EXLM, CNV_AT, CNV_HASH, CNV_DLR, CNV_PERC, CNV_CIRC, CNV_AMPR, CNV_ASTR, CNV_LPRN, CNV_RPRN, CNV_DEL,
    CNV_PIPE, CNV_UNDS, CNV_PLUS 
};

#if 0
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Qwerty
     * ,----------------------------------------------------------------------------------------------------------------------.
     * | ESC  |   1  |   2  |   3  |   4  |   5  |      |                    |      |   6  |   7  |   8  |   9  |   0  |  `   |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * | TAB  |   Q  |   W  |   E  |   R  |   T  |      |                    |      |   Y  |   U  |   I  |   O  |   P  |  \   |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * | CTRL |   A  |   S  |   D  |   F  |   G  |DESKDN|                    |DESKUP|   H  |   J  |   K  |   L  |   ;  |  '   |
     * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
     * | SHIFT|   Z  |   X  |   C  |   V  |   B  | SHIFT|                    | SHIFT|   N  |   M  |   ,  |   .  |   /  | SHIFT|
     * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
     * | SHIFT|  ALT |  GUI |ADJUST|||||||| LOWER| SPACE|      ||||||||      | ENTER| RAISE||||||||ADJUST|  GUI |  ALT | SHIFT|
     * ,----------------------------------------------------------------------------------------------------------------------.
     */
    [_QWERTY] = LAYOUT( \
      KC_ESC,  CSTM_1,  CSTM_2,  CSTM_3,  CSTM_4,  CSTM_5,  XXXXXXX,                        XXXXXXX, CSTM_6,  CSTM_7,  CSTM_8,  CSTM_9,  CSTM_0,  GRV    , \
      KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    XXXXXXX,                        XXXXXXX, KC_Y  ,  KC_U  ,  KC_I,    KC_O,    KC_P,    BSLS   , \
      KC_LGUI, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    DESKDN ,                        DESKUP , KC_H  ,  KC_J  ,  KC_K,    KC_L,    SCLN,    QUOT   , \
      SHIFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    SHIFT  ,                        SHIFT  , KC_N  ,  KC_M  ,  KC_COMM, KC_DOT,  KC_SLSH, SHIFT  , \
      SHIFT,   KC_LALT, KC_LCTL, ADJUST,           LOWER,   KC_SPC ,XXXXXXX,        XXXXXXX,KC_ENT , RAISE ,           ADJUST,  KC_LCTL, KC_LALT, SHIFT    \
    ),

    /* Lower
     * ,----------------------------------------------------------------------------------------------------------------------.
     * |  F11 |  F1  |  F2  |  F3  |  F4  |  F5  |      |                    |      |  F6  |  F7  |  F8  |  F9  |  F10 |  F12 |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * |  ~   |   !  |   @  |   #  |   $  |   %  |      |                    |      |   ^  |   &  |   *  |   (  |   )  |  |   |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * |      |   1  |   2  |   3  |   4  |   5  |      |                    |      |   6  |   7  |   8  |   9  |   0  |      |
     * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
     * |      ||||||||||||||||||||||   -  |   _  |      |                    |      |   +  |   =  |   [  |   ]  ||||||||      |
     * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
     * |      |      |      |      ||||||||      |  DEL |      ||||||||      | BKSP |      ||||||||      |      |      |      |
     * ,----------------------------------------------------------------------------------------------------------------------.
     */
    [_LOWER] = LAYOUT(
      KC_F11,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,                        _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F12 , \
      TILD,    EXLM,    AT,      HASH,    DLR,     PERC,    _______,                        _______, CIRC,    AMPR,    ASTR,    LPRN,    RPRN,    PIPE   , \
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______,                        _______, KC_6,    KC_7,    KC_8,    KC_9,    KC_0   , _______, \
      _______, XXXXXXX, XXXXXXX, XXXXXXX, MINS,    UNDS,    _______,                        _______, PLUS,    EQL,     LBRC,    RBRC,    XXXXXXX, _______, \
      _______, _______, _______, _______,          _______, KC_DEL ,_______,        _______,KC_BSPC, _______,          _______, _______, _______, _______  \
    ),

    /* Raise
     * ,----------------------------------------------------------------------------------------------------------------------.
     * |  F11 |  F1  |  F2  |  F3  |  F4  |  F5  |      |                    |      |  F6  |  F7  |  F8  |  F9  |  F10 |  F12 |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * |  ~   |   !  |   @  |   #  |   $  |   %  |      |                    |      |   ^  |   &  |   *  |   (  |   )  |  |   |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * |      |   1  |   2  |   3  |   4  |   5  |      |                    |      |   6  |   7  |   8  |   9  |   0  |      |
     * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
     * |      ||||||||   [  |   ]  |   -  |   _  |      |                    |      |   +  |   =  ||||||||||||||||||||||      |
     * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
     * |      |      |      |      ||||||||      |  DEL |      ||||||||      | BKSP |      ||||||||      |      |      |      |
     * ,----------------------------------------------------------------------------------------------------------------------.
     */
    [_RAISE] = LAYOUT(
      KC_F11,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   _______,                        _______, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F12 , \
      TILD,    EXLM,    AT,      HASH,    DLR,     PERC,    _______,                        _______, CIRC,    AMPR,    ASTR,    LPRN,    RPRN,    PIPE   , \
      _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    _______,                        _______, KC_6,    KC_7,    KC_8,    KC_9,    KC_0   , _______, \
      _______, XXXXXXX, LBRC,    RBRC,    MINS,    UNDS,    _______,                        _______, PLUS,    EQL,     XXXXXXX, XXXXXXX, XXXXXXX, _______, \
      _______, _______, _______, _______,          _______, KC_DEL ,_______,        _______,KC_BSPC, _______,          _______, _______, _______, _______  \
    ),

    /* Adjust
     * ,----------------------------------------------------------------------------------------------------------------------.
     * |      |      |      |      |      |      |  US  |                    |  JP  |      |      |      |      |      |      |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * |      |      |      |      |      |      |UBUNTU|                    |  MAC |      |      |      |      |      |      |
     * |------+------+------+------+------+------+------+--------------------+------+------+------+------+------+------+------|
     * |      | HOME |PAGEDN|PAGEUP|  END |||||||| PSCR |                    | PSCR | LEFT | DOWN |  UP  | RIGHT||||||||      |
     * |------+------+------+------+------+------+---------------------------+------+------+------+------+------+------+------|
     * |      |      |   {  |   }  |      |      |      |                    |      |      |      |   {  |   }  |      |      |
     * |-------------+------+------+------+------+------+------+------+------+------+------+------+------+------+-------------|
     * |      |      |      |      ||||||||      |      |      ||||||||      |      |      ||||||||      |      |      |      |
     * ,----------------------------------------------------------------------------------------------------------------------.
     */
    [_ADJUST] = LAYOUT(
      _______, _______, _______, _______, _______, _______, US     ,                        JP,      _______, _______, _______, _______, _______, _______, \
      _______, _______, _______, _______, _______, _______, UBUNTU ,                        MAC,     _______, _______, _______, _______, _______, _______, \
      _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  XXXXXXX, KC_PSCR,                        KC_PSCR, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX, _______, \
      _______, _______, LCBR,    RCBR,    _______, _______, _______,                        _______, _______, _______, LCBR,    RCBR,    _______, _______, \
      _______, _______, _______, _______,          _______, _______,_______,        _______,_______, _______,          _______, _______, _______, _______  \
    )};

// clang-format on

#ifdef AUDIO_ENABLE
float tone_qwerty[][2] = SONG(QWERTY_SOUND);
#endif

#endif // 0

#define JP_LAYOUT true
#define US_LAYOUT false

#if 1
bool LAYOUT_STATUS = JP_LAYOUT;     // layout setting the host which this keyboard connect to
// bool LAYOUT_STATUS = US_LAYOUT;     // layout setting the host which this keyboard connect to
#else

#ifdef MASTER_LEFT
bool LAYOUT_STATUS = US_LAYOUT;
#else
bool LAYOUT_STATUS = JP_LAYOUT;
#endif

#endif

#define UBUNTU_MODE true
#define MAC_MODE    false

#if 1
bool MACHINE_MODE = UBUNTU_MODE;
#endif

bool SHIFT_PRESSED = false;

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

#define SEND_STRING_RESTORE(STR) (SHIFT_PRESSED ? SEND_STRING(STR SS_DOWN(X_LSHIFT)) : SEND_STRING(STR SS_UP(X_LSHIFT)))

#define KEY(CODE) (record->event.pressed ? SEND_STRING(SS_DOWN(X_##CODE)) : SEND_STRING_RESTORE(SS_UP(X_##CODE)))

#define KEY_SHIFT(CODE) (record->event.pressed ? SEND_STRING(SS_DOWN(X_LSHIFT) SS_DOWN(X_##CODE)) : SEND_STRING_RESTORE(SS_UP(X_##CODE)))

#define KEY_UPSHIFT(CODE) (record->event.pressed ? SEND_STRING(SS_UP(X_LSHIFT) SS_DOWN(X_##CODE)) : SEND_STRING_RESTORE(SS_UP(X_##CODE)))

#define SHIFT_DU(CODE_DOWN, CODE_UP) (SHIFT_PRESSED ? CODE_DOWN : CODE_UP)
#define CASE_US(CODE, US, JP)                   \
    case CODE:                                  \
        (LAYOUT_STATUS == JP_LAYOUT ? JP : US); \
        return false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        // CASE_US(CNV_GRV, KEY(GRAVE), SHIFT_DU(KEY_SHIFT(EQUAL), KEY_SHIFT(LBRACKET)));
        CASE_US(CNV_GRV, KEY(GRAVE), SHIFT_DU(KEY_SHIFT(0), KEY(GRAVE)));
        CASE_US(CNV_1, KEY(1), KEY(1));
        CASE_US(CNV_2, KEY(2), SHIFT_DU(KEY_UPSHIFT(LBRACKET), KEY(2)));
//#if 0
        CASE_US(CNV_3, KEY(3), KEY(3));
        CASE_US(CNV_4, KEY(4), KEY(4));
        CASE_US(CNV_5, KEY(5), KEY(5));
        CASE_US(CNV_6, KEY(6), SHIFT_DU(KEY_UPSHIFT(EQUAL), KEY(6)));
        CASE_US(CNV_7, KEY(7), SHIFT_DU(KEY_SHIFT(6), KEY(7)));
        CASE_US(CNV_8, KEY(8), SHIFT_DU(KEY_SHIFT(QUOTE), KEY(8)));
        CASE_US(CNV_9, KEY(9), SHIFT_DU(KEY_SHIFT(8), KEY(9)));
        CASE_US(CNV_0, KEY(0), SHIFT_DU(KEY_SHIFT(GRAVE), KEY(0)));
        CASE_US(CNV_MINS, KEY(MINUS), SHIFT_DU(KEY_SHIFT(INT1), KEY(MINUS)));
        CASE_US(CNV_EQL, KEY(EQUAL), SHIFT_DU(KEY_SHIFT(SCOLON), KEY_SHIFT(MINUS)));
        CASE_US(CNV_LBRC, KEY(LBRACKET), SHIFT_DU(KEY_SHIFT(RBRACKET), KEY(RBRACKET)));
        CASE_US(CNV_RBRC, KEY(RBRACKET), SHIFT_DU(KEY_SHIFT(NONUS_HASH), KEY(NONUS_HASH)));
        CASE_US(CNV_BSLS, KEY(BSLASH), SHIFT_DU(KEY_SHIFT(INT3), KEY(INT3)));
        CASE_US(CNV_SCLN, KEY(SCOLON), SHIFT_DU(KEY_UPSHIFT(QUOTE), KEY(SCOLON)));
        CASE_US(CNV_QUOT, KEY(QUOTE), SHIFT_DU(KEY_SHIFT(2), KEY_SHIFT(7)));
#if 0
        CASE_US(CNV_TILD, KEY_SHIFT(GRAVE), KEY_SHIFT(EQUAL));
        CASE_US(CNV_EXLM, KEY_SHIFT(1), KEY_SHIFT(1));
        // CASE_US(CNV_AT, KEY_SHIFT(2), KEY(LBRACKET));
        CASE_US(CNV_HASH, KEY_SHIFT(3), KEY_SHIFT(3));
        CASE_US(CNV_DLR, KEY_SHIFT(4), KEY_SHIFT(4));
        CASE_US(CNV_PERC, KEY_SHIFT(5), KEY_SHIFT(5));
        CASE_US(CNV_CIRC, KEY_SHIFT(6), KEY(EQUAL));
        CASE_US(CNV_AMPR, KEY_SHIFT(7), KEY_SHIFT(6));
        CASE_US(CNV_ASTR, KEY_SHIFT(8), KEY_SHIFT(QUOTE));
        CASE_US(CNV_LPRN, KEY_SHIFT(9), KEY_SHIFT(8));
        CASE_US(CNV_RPRN, KEY_SHIFT(0), KEY_SHIFT(9));
        CASE_US(CNV_DEL, KEY(DELETE), KEY_UPSHIFT(BSPACE));
        CASE_US(CNV_PIPE, KEY_SHIFT(BSLASH), KEY_SHIFT(INT3));
        CASE_US(CNV_UNDS, KEY_SHIFT(MINUS), KEY_SHIFT(INT1));
        CASE_US(CNV_PLUS, KEY_SHIFT(EQUAL), KEY_SHIFT(SCOLON));
#endif

#if 0
        case JP:
            if (record->event.pressed) {
                LAYOUT_STATUS = JP_LAYOUT;
            }
            return false;
            break;
        case US:
            if (record->event.pressed) {
                LAYOUT_STATUS = US_LAYOUT;
            }
            return false;
            break;
#endif
        case UBUNTU:
            if (record->event.pressed) {
                MACHINE_MODE = UBUNTU_MODE;
            }
            return false;
            break;
        case MAC:
            if (record->event.pressed) {
                MACHINE_MODE = MAC_MODE;
            }
            return false;
            break;
#if 0
        case DESKDN:
            if (record->event.pressed) {
                if (MACHINE_MODE == UBUNTU_MODE) {
                    register_code(KC_LGUI);
                    tap_code(KC_PGDN);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_LEFT);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
        case DESKUP:
            if (record->event.pressed) {
                if (MACHINE_MODE == UBUNTU_MODE) {
                    register_code(KC_LGUI);
                    tap_code(KC_PGUP);
                    unregister_code(KC_LGUI);
                } else {
                    register_code(KC_LCTL);
                    tap_code(KC_RIGHT);
                    unregister_code(KC_LCTL);
                }
            }
            return false;
            break;
#endif
#if 0
        case SHIFT:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LSHIFT));
                SHIFT_PRESSED = true;
            } else {
                SEND_STRING(SS_UP(X_LSHIFT));
                SHIFT_PRESSED = false;
            }
            return false;
            break;
#endif

#if 0
        case LOWER:
            if (record->event.pressed) {
                // layer_on(_LOWER);
            } else {
                // layer_off(_LOWER);
            }
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                // layer_on(_RAISE);
            } else {
                // layer_off(_RAISE);
            }
            update_tri_layer(_LOWER, _RAISE, _ADJUST);
            return false;
            break;
#endif
#if 0
        case ADJUST:
            if (record->event.pressed) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            return false;
            break;
#endif
    }
    return true;
}
#endif // US layout act as JIS Layout

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
  |  esc | ctrl |  win |  cmd | space| MS-M | MS-L |      |          | MB-R | space| ctrl | RAISE| LOWER| left | down | right| R4
  |      |      |      |  alt |      |      |      |      |          |      |      |      |      |      |      |      |      | 
  -------+------+------+------+------+------+------+------+          +------+------+------+------+------+------+------+------+ 
 */

[LY_MAC] = LAYOUT(
  CNV_GRV, CNV_1,   CNV_2, CNV_3,   CNV_4,  CNV_5,  CNV_6,  xxx,     xxx,    CNV_7,  CNV_8,   CNV_9,   CNV_0,    CNV_MINS, CNV_EQL,  KC_BSPC,  \
  KC_TAB,  KC_Q,    KC_W,  KC_E,    KC_R,   KC_T,   xxx,    xxx,     KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,     CNV_LBRC, CNV_RBRC, CNV_BSLS, \
  US_EISU, KC_A,    KC_S,  KC_D,    KC_F,   KC_G,   xxx,    xxx,     KC_H,   KC_J,   KC_K,    KC_L,    CNV_SCLN, CNV_QUOT, KC_ENT,   xxx,      \
  xxx,     KC_LSFT, KC_Z,  KC_X,    KC_C,   KC_V,   KC_B,   xxx,     KC_B,   KC_N,   KC_M,    KC_COMM, KC_DOT,   KC_SLSH,  KC_UP,    KC_DEL,   \
  KC_ESC,  KC_LALT, ____,  KC_LALT, KC_SPC, KC_MSM, KC_MSL, xxx,     KC_MSR, KC_SPC, KC_RCTL, RAISE,   LOWER,    KC_LEFT,  KC_DOWN,  KC_RIGHT  \
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

/* Raise
     C-L0   C-L1   C-L2   C-L3   C-L4   C-L5   C-L6   C-L7             C-R0   C-R1   C-R2   C-R3   C-R4   C-R5   C-R6   C-R7   
   +-------------------------------------------------------+         +-------------------------------------------------------+
   | reset|  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |      |         |      |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |      | R0
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R1
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R2
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      |      |      |      | R3
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      | RAISE|      |      |      |      | R4
   +-------------------------------------------------------+         +-------------------------------------------------------+
 */

[LY_RAISE] = LAYOUT(
  RESET,   KC_F1,    KC_F2,   KC_F3, KC_F4,  KC_F5,  KC_F6,  xxx,    xxx,    KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,   KC_F12,   xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    ____,     xxx,     xxx,      xxx,      xxx      \
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
   |      |      |      |      |      |      |      |      |         |      |      |      |      |      | home | pgup |  end | R3
   +-------------------------------------------------------+         +-------------------------------------------------------+
   |      |      |      |      |      |      |      |      |         |      |      |      |      | LOWER|      | pgdn |      | R4
   +-------------------------------------------------------+         +-------------------------------------------------------+
 */

[LY_LOWER] = LAYOUT(
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     xxx,      xxx,      xxx,     \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,     xxx,     KC_HOME,  KC_PGUP,  KC_END,  \
  xxx,     xxx,      xxx,     xxx,   xxx,    xxx,    xxx,    xxx,    xxx,    xxx,    xxx,     xxx,    ____,     xxx,      KC_PGDN,  xxx      \
  )
};
