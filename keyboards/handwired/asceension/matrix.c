/*
Copyright 2012-2017 Jun Wako, Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdint.h>
#include <stdbool.h>
#if defined(__AVR__)
#include <avr/io.h>
#endif
#include "wait.h"
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "timer.h"
#include "asceension.h"

#include "debug.h"

/* Set 0 if debouncing isn't needed */

#ifndef DEBOUNCING_DELAY
#   define DEBOUNCING_DELAY 5
#endif

#if (DEBOUNCING_DELAY > 0)
    static uint16_t debouncing_time;
    static bool debouncing = false;
#endif

#if (MATRIX_COLS <= 8)
#    define print_matrix_header()  print("\nr/c 01234567\n")
#    define print_matrix_row(row)  print_bin_reverse8(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop(matrix[i])
#    define ROW_SHIFTER ((uint8_t)1)
#elif (MATRIX_COLS <= 16)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse16(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop16(matrix[i])
#    define ROW_SHIFTER ((uint16_t)1)
#elif (MATRIX_COLS <= 32)
#    define print_matrix_header()  print("\nr/c 0123456789ABCDEF0123456789ABCDEF\n")
#    define print_matrix_row(row)  print_bin_reverse32(matrix_get_row(row))
#    define matrix_bitpop(i)       bitpop32(matrix[i])
#    define ROW_SHIFTER  ((uint32_t)1)
#endif

#ifdef MATRIX_MASKED
    extern const matrix_row_t matrix_mask[];
#endif

static const uint8_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];
static uint8_t mcp23018_reset_loop;

static void init_cols(void);
static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row);
static void unselect_rows(void);
static void select_row(uint8_t row);

__attribute__ ((weak))
void matrix_init_quantum(void) {
    matrix_init_kb();
}

__attribute__ ((weak))
void matrix_scan_quantum(void) {
    matrix_scan_kb();
}

__attribute__ ((weak))
void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__ ((weak))
void matrix_scan_kb(void) {
    matrix_scan_user();
}

__attribute__ ((weak))
void matrix_init_user(void) {
}

__attribute__ ((weak))
void matrix_scan_user(void) {
}

inline
uint8_t matrix_rows(void) {
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void) {
    return MATRIX_COLS;
}

void matrix_init(void) {

    /* To use PORTF disable JTAG with writing JTD bit twice within four cycles. */
#if  (defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__) || defined(__AVR_ATmega32U4__))
    MCUCR |= _BV(JTD);
    MCUCR |= _BV(JTD);
#endif

    mcp23018_status = true;

    /* initialize row and col */
    unselect_rows();
    init_cols();                                            // all cols is scan via MCP23018

    /* initialize matrix state: all keys off */
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }

    matrix_init_quantum();
}

uint8_t matrix_scan(void)
{
    // init MCP2301 8
    if (mcp23018_status) { 
        /* if there was an error */
        if (++mcp23018_reset_loop == 0) {
            /* since mcp23018_reset_loop is 8 bit - we'll try to reset once in 255 matrix scans
               this will be approx bit more frequent than once per second */
            print("trying to initalize MCP23017...");
            mcp23018_status = init_mcp23018();

            if (mcp23018_status) {
                print("failed. NOT responding\n");
                return 1;
            } else {
                print("done.\n");
            }
        }
    }

    /* Set row, read cols */
    for (uint8_t current_row = 0; current_row < MATRIX_ROWS; current_row++) {
#if (DEBOUNCING_DELAY > 0)
        bool matrix_changed = read_cols_on_row(matrix_debouncing, current_row);

        if (matrix_changed) {
            debouncing = true;
            debouncing_time = timer_read();
        }
#else
        read_cols_on_row(matrix, current_row);
#endif
    }

#if (DEBOUNCING_DELAY > 0)
    if (debouncing && (timer_elapsed(debouncing_time) > DEBOUNCING_DELAY)) {
        for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
            matrix[i] = matrix_debouncing[i];
        }
        debouncing = false;
    }
#endif

    matrix_scan_quantum();
    return 1;
}

bool matrix_is_modified(void)
{
#if (DEBOUNCING_DELAY > 0)
    if (debouncing) return false;
#endif
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1 << col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    /* Matrix mask lets you disable switches in the returned matrix data. For example, if you have a
       switch blocker installed and the switch is always pressed. */
#ifdef MATRIX_MASKED
    return matrix[row] & matrix_mask[row];
#else
    return matrix[row];
#endif
}

void matrix_print(void)
{
    print_matrix_header();

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row); print(": ");
        print_matrix_row(row);
        print("\n");
    }
}

uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += matrix_bitpop(i);
    }
    return count;
}

static void init_cols(void)
{
#if 1 // all cols uses MCP23018
    // do nothing because MCP23018 is initialized every cycles 
#else
    for(uint8_t x = 0; x < ATMEGA_COLS; x++) {
        uint8_t pin = col_pins[x];
        _SFR_IO8((pin >> 4) + 1) &= ~_BV(pin & 0xF); /* IN */
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); /* HI */
    }
#endif
}

static bool read_cols_on_row(matrix_row_t current_matrix[], uint8_t current_row)
{
    cols_t cols;

    /* Store last value of row prior to reading */
    matrix_row_t last_row_value = current_matrix[current_row];

    cols.word = 0;

    /* Select row and wait for row selecton to stabilize */
    select_row(current_row);
    wait_us(30);

    /* read all cols from MCP23018 */
    mcp23018_status = i2c_start(I2C_ADDR, I2C_WRITE);    if(mcp23018_status) goto out;
    mcp23018_status = i2c_write(GPIOA);                  if(mcp23018_status) goto out;
    mcp23018_status = i2c_start(I2C_ADDR, I2C_READ);     if(mcp23018_status) goto out;
    mcp23018_status = i2c_read_ack(&cols.byte.lo);       if(mcp23018_status) goto out;
    /* Now read to IODIRB */
    mcp23018_status = i2c_read_nak(&cols.byte.hi);       if(mcp23018_status) goto out;

    current_matrix[current_row] = ~(cols.word);       // update row

out:
    i2c_stop();

#if 0    // dbg
    if(current_row == 6) dprintf("cols.word:0x%X", cols.word);
#endif
 
    /* Unselect row */
    unselect_rows();

    return (last_row_value != current_matrix[current_row]);
}

static void select_row(uint8_t row)
{
#if 0 // all rows use ATMEGA32u4 GPIOs
    if (mcp23018_status) { 
        /* if there was an error do nothing */
    } else {
        /* set active row low  : 0
           set active row output : 1
           set other rows hi-Z : 1 */
        mcp23018_status = i2c_start(I2C_ADDR_WRITE);   if (mcp23018_status) goto out;
        mcp23018_status = i2c_write(GPIOB);            if (mcp23018_status) goto out;
        mcp23018_status = i2c_write(0xFF & ~(1<<abs(row-4))); if (mcp23018_status) goto out;
    out:
        i2c_stop();
    }
#endif
    
    uint8_t pin = row_pins[row];
    _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); /* OUT  */
    _SFR_IO8((pin >> 4) + 2) &= ~_BV(pin & 0xF); /* LOW  */
}

static void unselect_rows(void)
{
    for(uint8_t x = 0; x < MATRIX_ROWS; x++) {
        uint8_t pin = row_pins[x];
        _SFR_IO8((pin >> 4) + 1) |=  _BV(pin & 0xF); /* OUT */
        _SFR_IO8((pin >> 4) + 2) |=  _BV(pin & 0xF); /* HI  */
    }
}
