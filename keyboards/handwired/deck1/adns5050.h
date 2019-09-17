#pragma once

#ifdef __cplusplus
extern "C" {
#endif

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

typedef enum {
    PD_Normal     = 0,
    PD_Power_Down = 1
} Mouse_Control_PD;

typedef enum {
    RES_500cpi  = 0,
    RES_1000cpi = 1
} Mouse_Control_RES;

typedef enum{
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

class Adns5050
{
	private:
		uint8_t _pin_reset;
		uint8_t _pin_ncs;
		uint8_t _pin_noe;				// SDIO output enable (provided by 74HC125)
		int8_t  _last_op = -1;

#ifdef ADNS_50x0_DEBUG
		void _dbg_print(const char *s);
		void _dbg_print(const char *fmt, const char ch);
		void _dbg_print(const char *fmt, const char ch1, const char ch2);
#endif

	public:
                Adns5050() {}
		bool    init(const uint8_t pin_reset, const uint8_t pin_ncs, const uint8_t pin_noe, const uint8_t spi_opts);
		uint8_t read(const reg_t address);
		void    write(const reg_t address, const uint8_t value);
        void    power_down_mode(bool mode);
};

#ifdef __cplusplus
}
#endif
