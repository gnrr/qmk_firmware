// based on https://github.com/shb/arduino-adns-5050

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"             // USER_PRINT
#include "print.h"
#include "debug.h"              // dprint

#define ADNS_50x0_WRITE 0x80
#define ADNS_50x0_READ  0x00

#define ADNS_5050_PRODUCT_ID  0x12
#define ADNS_5050_PRODUCT_ID2 0x26
#define ADNS_5050_REVISION_ID 0x01

#define ADNS_5050_tSWR       20
#define ADNS_5050_tSRR        1  // 0.5 actually, but keep it integer
#define ADNS_5050_tSRW        1  // 0.5 actually, but keep it integer
#define ADNS_5050_tSWW       30
#define ADNS_50x0_tSRAD       4

typedef enum {
	REG_PRODUCT_ID     = 0,
	REG_REVISION_ID,
	REG_MOTION,
	REG_DELTA_X,
	REG_DELTA_Y,
	REG_SQUAL,
	REG_SHUTTER_UPPER,
	REG_SHUTTER_LOWER,
	REG_MAXIMUMM_PIXEL,
	REG_PIXEL_SUM,
	REG_MINIMUM_PIXEL,
	REG_PIXEL_GRAB,
	REG_MOUSE_CONTROL  = 0x0d,
	REG_MOUSE_CONTROL2 = 0x19,
	REG_LED_DC_MODE    = 0x22,
	REG_CHIP_RESET     = 0x3a,
	REG_PRODUCT_ID2    = 0x3e,
	REG_INV_REV_ID,
	REG_MOTION_BURST   = 0x63
} reg_t;

// todo bit-field
typedef enum {
    PD_Normal     = 0,
    PD_Power_Down = 1
} Mouse_Control_PD;

// todo bit-field
typedef enum {
    RES_500cpi  = 0,
    RES_1000cpi = 1
} Mouse_Control_RES;

typedef enum {
	RES2_125cpi  = 0x01,
	RES2_250cpi  = 0x02,
	RES2_375cpi  = 0x03,
	RES2_500cpi  = 0x04,
	RES2_625cpi  = 0x05,
	RES2_750cpi  = 0x06,
	RES2_850cpi  = 0x07,
	RES2_1000cpi = 0x08,
	RES2_1125cpi = 0x09,
	RES2_1250cpi = 0x0A,
	RES2_1375cpi = 0x0B
} Mouse_Control_RES2;

typedef enum {
    ADNS5050_ERR_INIT_SUCCESS = 0,
    ADNS5050_ERR_INVALID_PRODUCT_ID2,
    ADNS5050_ERR_INVALID_REVISION_ID,
    ADNS5050_ERR_INVALID_INV_REV_ID,
} Adns5050Err;

class Adns5050
{
    uint8_t _pin_reset;
    uint8_t _pin_cs;
    uint8_t _pin_oe;			// SDIO output enable for half duplex communication
                                // provided by 3-state buffer IC
    int8_t  _last_op = -1;

public:
                Adns5050() {}
    Adns5050Err init(const uint8_t pin_reset, const uint8_t pin_cs, const uint8_t pin_oe, const uint8_t spi_opts);
    uint8_t     read(reg_t address);
    void        write(reg_t address, uint8_t value);
    void        power_down_mode();
    void        wakeup();
    void        reset();
};

#ifdef __cplusplus
}
#endif
