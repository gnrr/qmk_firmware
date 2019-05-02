#include <stdbool.h>
#include "action.h"
#include "i2cmaster.h"
#include "expander.h"
#include "debug.h"

static uint8_t expander_status = 0;

void expander_config(void);
uint8_t expander_write(uint8_t reg, uint8_t data);
uint8_t expander_read(uint8_t reg, uint8_t *data);

void expander_init(void)
{
  i2c_init();
  expander_scan();
}

void expander_scan(void)
{
  dprintf("expander status: %d ... ", expander_status);
  bool expander_err = i2c_start(EXPANDER_ADDR | I2C_WRITE);

  if (expander_err == false) {
    i2c_stop();
    if (expander_status == 0) {
      dprintf("attached\n");
      expander_status = 1;
      expander_config();
      clear_keyboard();
    }
  }
  else {
    if (expander_status == 1) {
      dprintf("detached\n");
      expander_status = 0;
      clear_keyboard();
    }
  }
  dprintf("%d\n", expander_status);
}

// COLUMS: GPA, INPUT, PULL-UP, NEGATIVE-LOGIC

// read all columns in current row
matrix_row_t expander_read_cols(void)
{
    uint8_t expander_input;
    expander_read(EXPANDER_REG_GPIOA, &expander_input);         // negative-logic (LO => 1)

    return expander_input;
}

void expander_unselect_rows(void)
{
    // all rows: HI
    expander_write(EXPANDER_REG_GPIOB,  0xFF);      // 0*: output LO,       1 : output HI
}

// ROWS: GPB, OUTPUT, ACTIVE-LOW
void expander_select_row(uint8_t row)
{
    assert(row < 8);                                            // GPB0..GPB7
    expander_write(EXPANDER_REG_GPIOB,  ~_BV(row & 0x0F));      // active-lo
}

void expander_config(void)
{
    // GPAx: input, pullup
    expander_write(EXPANDER_REG_IPOLA,  0xFF);      // 0*: read LO => 0,    1 : read LO => 1 (* reset value)
    expander_write(EXPANDER_REG_GPPUA,  0xFF);      // 0*: disable pullup,  1 : enable pullup
    expander_write(EXPANDER_REG_IODIRA, 0xFF);      // 0 : output,          1*: input
 
    // GPBx: output, active-lo
    expander_write(EXPANDER_REG_IPOLB,  0x00);      // 0*: read LO => 0,    1 : read LO => 1 (* reset value)
    expander_write(EXPANDER_REG_GPPUB,  0x00);      // 0*: disable pullup,  1 : enable pullup
    expander_write(EXPANDER_REG_IODIRB, 0x00);      // 0 : output,          1*: input
    expander_write(EXPANDER_REG_GPIOB,  0xFF);      // 0*: output LO,       1 : output HI
}

uint8_t expander_write(uint8_t reg, uint8_t data)
{
  if (expander_status == 0) {
    return 0;
  }

  bool ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE); if (ret) goto stop;
  ret = i2c_write(reg);                       if (ret) goto stop;
  ret = i2c_write(data);
 stop:
  i2c_stop();
  return ret;
}

uint8_t expander_read(uint8_t reg, uint8_t *data)
{
  if (expander_status == 0) {
    return 0;
  }

  uint8_t ret;
  ret = i2c_start(EXPANDER_ADDR | I2C_WRITE);    if (ret) goto stop;
  ret = i2c_write(reg);                          if (ret) goto stop;
  ret = i2c_rep_start(EXPANDER_ADDR | I2C_READ); if (ret) goto stop;
  *data = i2c_readNak();
 stop:
  i2c_stop();
  return ret;
}
