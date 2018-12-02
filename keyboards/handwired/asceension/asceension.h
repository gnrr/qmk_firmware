#pragma once

#include <stdint.h>
#include <stdbool.h>

#if defined(__AVR__)
  #include <avr/pgmspace.h>     // PROGMEM
  #include <avr/io.h>           // PIND
#endif

#include "i2cmaster.h"

#include "keycode.h"            // KC_xxx
#include "action_layer.h"       // default_layer_set
#include "dipsw.h"
#include "analogpad.h"

#include "debug.h"

#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00

/* MCP23018 register addresses (see "mcp23018.md") */
#define I2C_ADDR        0b0100000
#define IODIRA          0x00            /* i/o direction register (BANK0) */
#define IODIRB          0x01
#define GPPUA           0x0C            /* GPIO pull-up resistor register (BANK0) */
#define GPPUB           0x0D
#define GPIOA           0x12            /* general purpose i/o port register (write modifies OLAT) (BANK0) */
#define GPIOB           0x13
#define OLATA           0x14            /* output latch register */
#define OLATB           0x15

#define RESET           0x5C00          /* RESET keycode */

extern uint8_t mcp23018_status;

uint8_t init_mcp23018(void);

