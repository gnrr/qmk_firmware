#include "i2cmaster.h"
#include "expander.h"

static EXPANDER_STATUS_t expander_status = ES_DETACHED;

uint8_t expander_write(uint8_t reg, uint8_t data);
uint8_t expander_read(uint8_t reg, uint8_t *data);

void expander_init(void)
{
   // dprintf(">> %s\n", __func__);

    i2c_init();
    _delay_ms(100);

   // dprintf("<< %s\n", __func__);
}

void expander_attach(void)
{
   // dprintf(">> %s\n", __func__);

    bool result = i2c_start(EXPANDER_ADDR, I2C_WRITE);

    if (result) {
        dprintf("  error: i2c_start() returns error. detached. result=0x%02X\n", result);

        expander_status = ES_DETACHED;
        clear_keyboard();

        dprintf("<< expander_attach\n");
        return;
    }
    
    i2c_stop();

    if (expander_status != ES_ATTACHED) {
        expander_status = ES_ATTACHED;
        dprintf("  attached\n");
    }

    // GPAx --> ROWS: output, active-lo (*: default value)
    expander_write(EXPANDER_REG_IPOLA,  0x00);      // 0*: read LO => 0,    1 : read LO => 1 (* reset value)
    expander_write(EXPANDER_REG_GPPUA,  0x00);      // 0*: disable pullup,  1 : enable pullup
    expander_write(EXPANDER_REG_IODIRA, 0x00);      // 0 : output,          1*: input
    expander_write(EXPANDER_REG_GPIOA,  0xFF);      // 0*: output LO,       1 : output HI

    // GPBx --> COLS: input, pullup
    expander_write(EXPANDER_REG_IPOLB,  0xFF);      // 0*: read LO => 0,    1 : read LO => 1 (* reset value)
    expander_write(EXPANDER_REG_GPPUB,  0xFF);      // 0*: disable pullup,  1 : enable pullup
    expander_write(EXPANDER_REG_IODIRB, 0xFF);      // 0 : output,          1*: input

   // dprintf("<< %s\n", __func__);
}

// COLUMNS: GPA, INPUT, PULL-UP, NEGATIVE-LOGIC

// read all columns in current row
matrix_row_t expander_read_cols(void)
{
   // dprintf(">> %s\n", __func__);

    uint8_t read_val;
    expander_read(EXPANDER_REG_GPIOB, &read_val);               // negative-logic (LO = 1)

    // dprintf("  read_val: %02X\n", read_val);

   // dprintf("<< %s\n", __func__);
    return read_val;
}

void expander_unselect_rows(void)
{
   // dprintf(">> %s\n", __func__);

    // all rows --> HI
    expander_write(EXPANDER_REG_GPIOA,  0xFF);      // 0*: output LO,       1 : output HI

   // dprintf("<< %s\n", __func__);
}

// ROWS: GPB, OUTPUT, ACTIVE-LOW
void expander_select_row(uint8_t row)
{
   // dprintf(">> %s\n", __func__);
    // dprintf("  row: %02X\n", row);

    assert(row < 8);                                            // GPx0..GPx7
    expander_write(EXPANDER_REG_GPIOA,  ~_BV(row));             // active-lo

   // dprintf("<< %s\n", __func__);
}

EXPANDER_AVAILABILITY_t expander_write(uint8_t reg, uint8_t data)
{
   // dprintf(">> %s\n", __func__);

    uint8_t ret = EA_UNAVAIL;

    if (expander_status == ES_DETACHED) {
        dprintf("  error: expander is not accessible\n");
        // dprintf("<< %s\n", __func__);
        return ret;
    }

    ret = i2c_start(EXPANDER_ADDR, I2C_WRITE); if (ret) goto stop;
    ret = i2c_write(reg);                      if (ret) goto stop;
    ret = i2c_write(data);
stop:
    i2c_stop();

    // dprintf("  reg:%02X, data:%02X\n", reg, data);
    // dprintf("<< %s\n", __func__);
    return ret;
}

EXPANDER_AVAILABILITY_t expander_read(uint8_t reg, uint8_t *data)
{
   // dprintf(">> %s\n", __func__);

    uint8_t ret = EA_UNAVAIL;

    if (expander_status == ES_DETACHED) {
        dprintf("  error: expander is not accessible\n");
        // dprintf("<< %s\n", __func__);
        return ret;
    }

    ret = i2c_start(EXPANDER_ADDR, I2C_WRITE);    if (ret) goto stop;
    ret = i2c_write(reg);                         if (ret) goto stop;
    ret = i2c_rep_start(EXPANDER_ADDR, I2C_READ); if (ret) goto stop;
    ret = i2c_read_nak(data);
stop:
    i2c_stop();

    // dprintf("  reg:%02X, data:%02X\n", reg, *data);
    // dprintf("<< %s\n", __func__);
    return ret;
}
