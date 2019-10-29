#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include "util.h"
#include "config.h"             // USER_PRINT, DEBUG_MATRIX_SCAN_RATE 
#include "print.h"
// #include "debug.h"
#include "deck.h"               // dbg_lo, dbg_hi, dbg_out_init
#include "matrix.h"

#ifdef DEBUG_MATRIX_SCAN_RATE
#include  "timer.h"
#endif

#include "deck.h"
#include "expander.h"

#define EXPANDER_ENABLE
#define DEBUG_MATRIX_SCAN_RATE

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];

// Debouncing: store for each key the number of scans until it's eligible to
// change.  When scanning the matrix, ignore any changes in keys that have
// already changed in the last DEBOUNCE scans.
static uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

#if 1
// __attribute__ ((weak))
void matrix_init_user(void)
{
    dprintf(">> %s\n", __func__);
    // dbg_out_init();
    // dbg_hi(D2);
    
    dprintf("<< %s\n", __func__);
}
#endif

#if 1
// __attribute__ ((weak))
void matrix_init_kb(void)
{
    dprintf(">> %s\n", __func__);
    matrix_init_user();
    dprintf("<< %s\n", __func__);
}
#endif

#if 1
void matrix_scan_kb(void) {}
#endif

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    // disable JTAG
    // MCUCR = (1<<JTD);
    // MCUCR = (1<<JTD);

    // _delay_ms(1000);             // for print(), etc...
    // debug_enable = true;
    dprintf(">> %s\n", __func__);

    // right:  AVR
    unselect_rows();
    init_cols();

#ifdef EXPANDER_ENABLE
    // left: Expander
    expander_init();
#endif

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        for (uint8_t j=0; j < MATRIX_COLS; ++j) {
            debounce_matrix[i * MATRIX_COLS + j] = 0;
        }
    }

    matrix_init_quantum();
    dprintf("<< %s\n", __func__);
}

// Returns a matrix_row_t whose bits are set if the corresponding key should be
// eligible to change in this scan.
matrix_row_t debounce_mask(uint8_t row) {
    matrix_row_t result = 0;
    for (uint8_t j=0; j < MATRIX_COLS; ++j) {
        if (debounce_matrix[row * MATRIX_COLS + j]) {
            --debounce_matrix[row * MATRIX_COLS + j];
        } else {
            result |= (1 << j);
        }
    }
    return result;
}

// Report changed keys in the given row.  Resets the debounce countdowns
// corresponding to each set bit in 'change' to DEBOUNCE.
void debounce_report(matrix_row_t change, uint8_t row) {
    for (uint8_t i = 0; i < MATRIX_COLS; ++i) {
        if (change & (1 << i)) {
            debounce_matrix[row * MATRIX_COLS + i] = DEBOUNCE;
        }
    }
}

void matrix_print(void)
{
#ifdef DEBUG_MATRIX_SCAN_RATE
#ifdef EXPANDER_ENABLE
    const uint8_t ROW_START = ROW_START_EXPANDER;   // Expander + AVR
#else
    const uint8_t ROW_START = ROW_START_AVR;        // AVR only
#endif
    // debug_enable = true;
    static uint32_t timer_old = 0;
    static uint32_t matrix_scan_count = 0;
    matrix_scan_count++;

    uint32_t timer_now = timer_read32();
    if (TIMER_DIFF_32(timer_now, timer_old)>1000) {
        dprintf("matrix scan frequency: %d Hz\n", matrix_scan_count);

        // print matrix
        dprint("r/c 01234567\n");
        for (uint8_t row = ROW_START; row < MATRIX_ROWS; row++) {
            dprintf("%02X: %08b\n", row, matrix_get_row(row));
        }

        timer_old = timer_now;
        matrix_scan_count = 0;
    }
#endif // DEBUG_MATRIX_SCAN_RATE
}

uint8_t matrix_scan(void)
{
    dprintf(">> %s\n", __func__);

// #if 0
#ifdef EXPANDER_ENABLE
    expander_attach();
#endif

#ifdef DEBUG_MATRIX_SCAN_RATE
    matrix_print();
#endif

    for(uint8_t i=0; i<(MATRIX_ROWS / 2); i++) {
        matrix_row_t mask, cols;

        select_row(i);                            // right:  AVR

#ifdef EXPANDER_ENABLE
        expander_select_row(i);                   // left: Expander
#endif
        wait_us(30);  // without this wait read unstable value.

        // Right: AVR
        uint8_t ia = i + ROW_START_AVR;
        mask = debounce_mask(ia);
        cols = (read_cols() & mask) | (matrix[ia] & ~mask);
        debounce_report(cols ^ matrix[ia], ia);
        matrix[ia] = cols;

        unselect_rows();

// #if 0
#ifdef EXPANDER_ENABLE
    // Left: Expander
    uint8_t ie = i + ROW_START_EXPANDER;
    mask = debounce_mask(ie);
    cols = (expander_read_cols() & mask) | (matrix[ie] & ~mask);
    debounce_report(cols ^ matrix[ie], ie);
    matrix[ie] = cols;

    expander_unselect_rows();
#endif
    }

    matrix_scan_quantum();

    dprintf("<< %s\n", __func__);
    // dbg_lo(2);
    return 1;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

// COLUMNS: INPUT, PULL-UP, NEGATIVE-LOGIC
//                          COL-R0  -R1  -R2  -R3  -R4  -R5  -R6  -R7
//const uint8_t COL_PINS_AVR[] = {F4,  F5,  F6,  F7,  B4,  D7,  D7,  B6}; // D4 for dbg_out
const uint8_t COL_PINS_AVR[] = {F4,  F5,  F6,  F7,  B4,  D7,  D4,  B6}; // D4 for dbg_out
#define COUNT_COL_PINS_AVR  (sizeof(COL_PINS_AVR)/sizeof(COL_PINS_AVR[0]))

static void  init_cols(void)
{
    dprintf(">> %s\n", __func__);
    // Right: AVR
    // DDRx
    //   0      INPUT  <-- on reset
    //   1      OUTPUT
    //
    // PORTx    INPUT   OUTPUT
    //   0        -       LO    <-- on reset
    //   1      PULLUP    HI

    assert(COUNT_COL_PINS_AVR <= 8);
    
    // all columns are set as input, pullup
    for(uint8_t i = 0; i < COUNT_COL_PINS_AVR; i++) {
        uint8_t pin = COL_PINS_AVR[i];
        _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // DDRx  --> IN
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // PORTx --> HI (for pullup)
    }
    dprintf("<< %s\n", __func__);
}

static matrix_row_t read_cols(void)
{
    dprintf(">> %s\n", __func__);
    uint8_t cols = 0;

    // Right: AVR
    assert(COUNT_COL_PINS_AVR <= 8);

    for(uint8_t i=0; i<COUNT_COL_PINS_AVR; i++) {
        uint8_t pin = COL_PINS_AVR[i];
        uint8_t val = _SFR_IO8(pin >> 4) & _BV(pin & 0xF);   // PINx (active-lo)
        if(val) cols &= ~(1 << i);
        else    cols |=  (1 << i);
    }

    dprintf("<< %s\n", __func__);
    return cols;
}

// ROWS: OUTPUT, ACTIVE-LOW
//                                 ROW-R0  -R1  -R2  -R3  -R4
static const uint8_t ROW_PINS_AVR[] = {B5,  B7,  E6,  D6,  C6};
#define COUNT_ROW_PINS_AVR  (sizeof(ROW_PINS_AVR)/sizeof(ROW_PINS_AVR[0]))

static void unselect_rows(void)
{
    dprintf(">> %s\n", __func__);
    // Right: AVR
    // DDRx
    //   0      INPUT  <-- on reset
    //   1      OUTPUT
    //
    // PORTx    INPUT   OUTPUT
    //   0        -       LO    <-- on reset
    //   1      PULLUP    HI

    // all rows are set as output, active-lo
    for(uint8_t i = 0; i < COUNT_ROW_PINS_AVR; i++) {
        uint8_t pin = ROW_PINS_AVR[i];
        _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); // DDRx  --> OUT
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // PORTx --> HI
    }
    dprintf("<< %s\n", __func__);
}

static void select_row(uint8_t row)
{
    dprintf(">> %s\n", __func__);
    // Right: AVR
    uint8_t pin = ROW_PINS_AVR[row];
    _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); // DDRx  --> OUT
    _SFR_IO8((pin >> 4) + 2) &= ~_BV(pin & 0xF); // PORTx --> LO

    dprintf("<< %s\n", __func__);
}

