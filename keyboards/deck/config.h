#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0x1209
#define PRODUCT_ID      0x5502
#define DEVICE_VER      0x0001
#define MANUFACTURER    scum noodle society
#define PRODUCT         DECK1
#define DESCRIPTION     Keyboard with trackball and scroll-ring

/* key matrix size */
#define MATRIX_ROWS        (5 * 2)
#define ROW_START_AVR      5
#define ROW_START_EXPANDER 0
#define MATRIX_COLS        8

#define TRACKBALL_PRESCALE 2    // 1:thru, 2:half-speed, 4:quater-speed

#define ID_MS_BTN1 0
#define ID_MS_BTN2 1

// /* define if matrix has ghost */
// #define MATRIX_HAS_GHOST

// #define TAPPING_TERM    200
// #define IGNORE_MOD_TAP_INTERRUPT // this makes it possible to do rolling combos (zx) with keys that convert to other keys on hold (z becomes ctrl when you hold it, and when this option isn't enabled, z rapidly followed by x actually sends Ctrl-x. That's bad.)

// /* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
// #define LOCKING_SUPPORT_ENABLE
// // Locking resynchronize hack
// #define LOCKING_RESYNC_ENABLE

/* key combination for command */
#define IS_COMMAND() ( \
    get_mods() == (MOD_BIT(KC_LCTL) | MOD_BIT(KC_RCTL)) || \
    get_mods() == (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT)) \
)

// /* number of backlight levels */
// #define BACKLIGHT_LEVELS 3

// #define LED_BRIGHTNESS_LO       15
// #define LED_BRIGHTNESS_HI       255

/* fix space cadet rollover issue */
#define DISABLE_SPACE_CADET_ROLLOVER

/* Set 0 if debouncing isn't needed */
#define DEBOUNCE    5
/*
 * This constant define not debouncing time in msecs, but amount of matrix
 * scan loops which should be made to get stable debounced results.
 *
 * On this keyboard matrix scan rate is relatively low, because of slow I2C.
 * Now it's only 353 scans/sec, or about 2.83 ms/scan.
 * According to Cherry specs, debouncing time is 5 msec.
 *
 * And so, there is no sense to have DEBOUNCE higher than 2.
 */

#define USB_MAX_POWER_CONSUMPTION 500

/* NKRO */
#ifndef FORCE_NKRO
    #define FORCE_NKRO  // Depends on NKRO_ENABLE.
#endif

/* enable user print (uprint) */
// #define USER_PRINT
// #define DEBUG_MATRIX_SCAN_RATE

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */


/* disable print */
// #define NO_PRINT


/* disable action features */
// #define NO_ACTION_LAYER
// #define NO_ACTION_TAPPING
// #define NO_ACTION_ONESHOT
// #define NO_ACTION_MACRO
// #define NO_ACTION_FUNCTION


