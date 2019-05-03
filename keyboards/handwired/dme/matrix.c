#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include "wait.h"
#include "action_layer.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "dme.h"
#include "expander.h"
#ifdef DEBUG_MATRIX_SCAN_RATE
#include  "timer.h"
#endif

/*
 * This constant define not debouncing time in msecs, but amount of matrix
 * scan loops which should be made to get stable debounced results.
 *
 * On Ergodox matrix scan rate is relatively low, because of slow I2C.
 * Now it's only 317 scans/second, or about 3.15 msec/scan.
 * According to Cherry specs, debouncing time is 5 msec.
 *
 * And so, there is no sense to have DEBOUNCE higher than 2.
 */

#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif


#define EXPANDER_ENABLE

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

#ifdef DEBUG_MATRIX_SCAN_RATE
uint32_t matrix_timer;
uint32_t matrix_scan_count;
#endif

#if 1
// __attribute__ ((weak))
void matrix_init_user(void)
{
    // dbg_out_init();
    // dbg_hi(3);
    
    dprintf("matrix_init_user\n");
}
#endif

#if 1
// __attribute__ ((weak))
void matrix_init_kb(void)
{
  matrix_init_user();
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
  MCUCR = (1<<JTD);
  MCUCR = (1<<JTD);

  _delay_ms(1000);             // for print(), etc...

  debug_enable   = true;       // for dprintf()
  dprintf(">> matrix_init\n");
  print("matrix_init\n");

  // left:  AVR
  unselect_rows();
  init_cols();

#ifdef EXPANDER_ENABLE
  // right: Expander
  expander_init();
  expander_unselect_rows();
#endif


  // initialize matrix state: all keys off
  for (uint8_t i=0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
    for (uint8_t j=0; j < MATRIX_COLS; ++j) {
      debounce_matrix[i * MATRIX_COLS + j] = 0;
    }
  }

#ifdef DEBUG_MATRIX_SCAN_RATE
  matrix_timer = timer_read32();
  matrix_scan_count = 0;
#endif

  matrix_init_quantum();
  dprintf("<< matrix_init\n");

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

uint8_t matrix_scan(void)
{
    // debug_enable   = true;           // for dprintf()
    dprintf(">> matrix_scan\n");

#ifdef EXPANDER_ENABLE
    expander_scan();
#endif

    matrix_print();

#ifdef DEBUG_MATRIX_SCAN_RATE
  matrix_scan_count++;

  uint32_t timer_now = timer_read32();
  if (TIMER_DIFF_32(timer_now, matrix_timer)>1000) {
    print("matrix scan frequency: ");
    pdec(matrix_scan_count);
    print("\n");
    matrix_print();

    matrix_timer = timer_now;
    matrix_scan_count = 0;
  }
#endif

  for(uint8_t i=0; i<(MATRIX_ROWS / 2); i++) {
      matrix_row_t mask, cols;

      select_row(i);                            // left:  AVR

#ifdef EXPANDER_ENABLE
      expander_select_row(i);                   // right: Expander
#endif
      wait_us(30);  // without this wait read unstable value.

      // left: AVR
      mask = debounce_mask(i);
      cols = (read_cols() & mask) | (matrix[i] & ~mask);
      debounce_report(cols ^ matrix[i], i);
      matrix[i] = cols;

#ifdef EXPANDER_ENABLE
      // right: Expander
      uint8_t ir = i + ROW_START_R;
      mask = debounce_mask(ir);
      cols = (expander_read_cols() & mask) | (matrix[ir] & ~mask);
      debounce_report(cols ^ matrix[ir], ir);
      matrix[ir] = cols;
#endif
      
      unselect_rows();                          // left:  AVR

#ifdef EXPANDER_ENABLE
      expander_unselect_rows();                 // right: Expander
#endif
  }

  matrix_scan_quantum();

  dprintf("<< matrix_scan\n");
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

void matrix_print(void)
{
  print("\nr/c 0123456789ABCDEF\n");
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    phex(row); print(": ");
    pbin_reverse16(matrix_get_row(row));
    print("\n");
  }
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
//                        COL-L0  -L1  -L2  -L3  -L4  -L5  -L6 --> AVR
const uint8_t COL_PINS_L[] = {C6,  E6,  B5,  B6,  B7,  D6,  C7};
#define COUNT_COL_PINS_L    (sizeof(COL_PINS_L)/sizeof(COL_PINS_L[0]))

static void  init_cols(void)
{
    // left: AVR
    // DDRx
    //   0      INPUT  <-- on reset
    //   1      OUTPUT
    //
    // PORTx    INPUT   OUTPUT
    //   0        -       LO    <-- on reset
    //   1      PULLUP    HI

    assert(COUNT_COL_PINS_L <= 8);
    
    // all columns are set as input, pullup
    for(uint8_t i = 0; i < COUNT_COL_PINS_L; i++) {
        uint8_t pin = COL_PINS_L[i];
        _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); // DDRx  --> IN
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // PORTx --> HI (for pullup)
    }
}

static matrix_row_t read_cols(void)
{
    uint8_t cols = 0;

    // left: AVR
    assert(COUNT_COL_PINS_L <= 8);

    for(uint8_t i=0; i<COUNT_COL_PINS_L; i++) {
        uint8_t pin = COL_PINS_L[i];
        uint8_t val = _SFR_IO8(pin >> 4) & _BV(pin & 0xF);   // PINx (active-lo)
        if(val) cols &= ~(1 << i);
        else    cols |=  (1 << i);
    }

    return cols;
}

// ROWS: OUTPUT, ACTIVE-LOW
//                               ROW-L0  -L1  -L2  -L3  -L4  -L5  -L6 --> AVR
static const uint8_t ROW_PINS_L[] = {B1,  F0,  F1,  F4,  F5,  F6,  F7};
#define COUNT_ROW_PINS_L    (sizeof(ROW_PINS_L)/sizeof(ROW_PINS_L[0]))

static void unselect_rows(void)
{
    // left: AVR
    // DDRx
    //   0      INPUT  <-- on reset
    //   1      OUTPUT
    //
    // PORTx    INPUT   OUTPUT
    //   0        -       LO    <-- on reset
    //   1      PULLUP    HI

    // all rows are set as output, active-lo
    for(uint8_t i = 0; i < COUNT_ROW_PINS_L; i++) {
        uint8_t pin = ROW_PINS_L[i];
        _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); // DDRx  --> OUT
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); // PORTx --> HI
    }
}

static void select_row(uint8_t row)
{
    // left: AVR
    uint8_t pin = ROW_PINS_L[row];
    _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); // DDRx  --> OUT
    _SFR_IO8((pin >> 4) + 2) &= ~_BV(pin & 0xF); // PORTx --> LO
}

