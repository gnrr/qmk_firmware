#include "print.h"
#include "debug.h"
#include "wait.h"
#include "quantum/quantum.h"        // setPinOutput, writePinHigh, etc...
#include "LUFA/SPI.h"
#include "adns5050.h"

// return false:fail, true:success
bool Adns5050::init(const uint8_t pin_reset, const uint8_t pin_ncs, const uint8_t pin_noe, const uint8_t spi_opts)
{
    dprintf(">> %s\n", __func__);

	_pin_reset = pin_reset;
	_pin_ncs   = pin_ncs;
	_pin_noe   = pin_noe;

	dprintf("  ADNS.RESET: %02X\n", _pin_reset);
	dprintf("  ADNS.NCS: %02X\n", _pin_ncs);
	dprintf("  ADNS.NOE: %02X\n", _pin_noe);

	// Setup control pins
    setPinOutput(_pin_reset);
    writePinHigh(_pin_reset);           // _pin_reset --> HI

    setPinOutput(_pin_ncs);
    writePinHigh(_pin_ncs);             // _pin_ncs --> HI

    setPinOutput(_pin_noe);
    writePinHigh(_pin_noe);             // _pin_noe --> HI

	// Setup SPI parameters
    SPI_Init(spi_opts);

	// By now the sensor should have powered up, so we can...
	// Test Product_ID2 and Revision_ID
	uint8_t rd = 0;
	rd = read(REG_PRODUCT_ID2);
	if (rd != ADNS_5050_PRODUCT_ID2) {
	  	dprintf("  error: Invalid PRODUCT_ID2: %02X\n", rd);
        dprintf("<< %s\n", __func__);
	 	return false;
    }

	rd = read(REG_REVISION_ID);
	if (rd != ADNS_5050_REVISION_ID) {
	  	dprintf("  error: Invalid REVISION_ID: %02X\n", rd);
        dprintf("<< %s\n", __func__);
		return false;    
    }

    rd = read(REG_INV_REV_ID);
    // if ((uint8_t)~rd != ADNS_5050_REVISION_ID) {
    if (rd != ~ADNS_5050_REVISION_ID) {
        dprintf("  error: Invalid INV_REV_ID: %02X\n", rd);
        dprintf("<< %s\n", __func__);
        return false;
    }
    dprintf("Adns5050::init OK\n");

    dprintf("<< %s\n", __func__);
    return true;
}

uint8_t Adns5050::read(const reg_t addr)
{
    dprintf(">> %s\n", __func__);
	switch (_last_op)
	{
		// ...a READ
		case ADNS_50x0_READ:
			wait_us(ADNS_5050_tSRR);
			break;
            // ...a WRITE
		case ADNS_50x0_WRITE:
			wait_us(ADNS_5050_tSWR);
			break;
		default:
			wait_us(ADNS_5050_tSRR);
			break;
	}

    // OE enabled before writing address
    writePinLow(_pin_noe);             // _pin_noe --> LO
    writePinLow(_pin_ncs);             // _pin_ncs --> LO

    // write address
	uint8_t rd = SPI_TransferByte(addr);
	if (rd != addr) {
      writePinHigh(_pin_ncs);             // _pin_ncs --> HI
      writePinHigh(_pin_noe);             // _pin_noe --> HI

		dprintf("  Adns5050::read error: addr=%02X\n", addr);
        dprintf("<< %s\n", __func__);
		return 0xff;
	}
	
	wait_us(ADNS_50x0_tSRAD);

    // OE disabled before reading data
    writePinHigh(_pin_noe);             // _pin_noe --> HI

    // read data
	rd = SPI_ReceiveByte();

    writePinHigh(_pin_ncs);             // _pin_ncs --> HI
    dprintf("  Adns5050::read OK: addr=%02X, data=%02X\n", addr, rd);

	_last_op = ADNS_50x0_READ;
    dprintf("<< %s\n", __func__);
	return rd; 
}

void Adns5050::write(const reg_t addr, const uint8_t value)
{
    dprintf(">> %s\n", __func__);
	switch (_last_op)
	{
		case ADNS_50x0_READ:
			wait_us(ADNS_5050_tSRW);
			break;
		case ADNS_50x0_WRITE:
			wait_us(ADNS_5050_tSWW);
			break;
		default:
			wait_us(ADNS_5050_tSRW);
			break;
	}

    // OE enabled before writing address
      writePinLow(_pin_noe);             // _pin_noe --> LO
      writePinLow(_pin_ncs);             // _pin_noe --> LO

	// write address
	uint8_t rd = SPI_TransferByte(addr | ADNS_50x0_WRITE);
	if (rd != (addr | ADNS_50x0_WRITE)) {
        writePinHigh(_pin_ncs);             // _pin_ncs --> HI
        writePinHigh(_pin_noe);             // _pin_noe --> HI

		dprintf("  Adns5050::write error: addr=%02X, value=%02X\n", addr, value);
        dprintf("<< %s\n", __func__);
        return;	
	}

	// wait_us(ADNS_50x0_tSRAD);

	// write data
	SPI_SendByte(value);

    writePinHigh(_pin_ncs);             // _pin_ncs --> HI
    writePinHigh(_pin_noe);             // _pin_noe --> HI

	_last_op = ADNS_50x0_WRITE;

    dprintf("  Adns5050::write OK: addr=%02X, data=%02X\n", addr, value);
    dprintf("<< %s\n", __func__);
}

void Adns5050::power_down_mode(bool mode)
{
    dprintf(">> %s\n", __func__);

    char s[3] = "ON";
    uint8_t rd = read(REG_MOUSE_CONTROL);
    uint8_t wt;
  
    if (mode) {
        wt = rd |  0x02;   // true:  goto power down mode
    }
    else {
        wt = rd & ~0x02;   // false: goto normal mode
        strcpy(s, "OFF");
    }

    write(REG_MOUSE_CONTROL, wt);

    dprintf("  Adns5050::power-down-mode: %s\n", s);
    dprintf("<< %s\n", __func__);
}
