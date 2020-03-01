/*
    adns5050.cpp - low level driver for ADNS5050
    Copyright (C) hidsh

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// based on https://github.com/shb/arduino-adns-5050

#include "wait.h"
#include "quantum/quantum.h"        // setPinOutput, writePinHigh
#include "LUFA/SPI.h"
#include "adns5050.h"

Adns5050Err Adns5050::init(const uint8_t pin_reset, const uint8_t pin_cs, const uint8_t pin_oe, const uint8_t spi_opts)
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    _pin_reset = pin_reset;
    _pin_cs = pin_cs;
    _pin_oe = pin_oe;
    
	// dprintf("  Adns5050.RESET: %02X\n", _pin_reset);
	// dprintf("  Adns5050.CS: %02X\n", _pin_cs);
	// dprintf("  Adns5050.OE: %02X\n", _pin_oe);

	// Setup control pins
    setPinOutput(_pin_reset);
    writePinHigh(_pin_reset);           // _pin_reset --> HI

    setPinOutput(_pin_cs);
    writePinHigh(_pin_cs);             // _pin_cs --> HI

    setPinOutput(_pin_oe);
    writePinHigh(_pin_oe);             // _pin_oe --> HI

	// Setup SPI parameters
    SPI_Init(spi_opts);

	// By now the sensor should have powered up, so we can...
	// Test Product_ID2 and Revision_ID
	uint8_t rd;
	rd = read(REG_PRODUCT_ID2);
	if (rd != ADNS_5050_PRODUCT_ID2) {
	  	dprintf("  error: Invalid PRODUCT_ID2: %02X\n", rd);
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return ADNS5050_ERR_INVALID_PRODUCT_ID2;                   // abend
    }

	rd = read(REG_REVISION_ID);
	if (rd != ADNS_5050_REVISION_ID) {
	  	dprintf("  error: Invalid REVISION_ID: %02X\n", rd);
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return ADNS5050_ERR_INVALID_REVISION_ID;                   // abend
    }

    rd = read(REG_INV_REV_ID);
    if ((uint8_t)~rd != ADNS_5050_REVISION_ID) {
    // if (rd != ~ADNS_5050_REVISION_ID) {
        dprintf("  error: Invalid INV_REV_ID: %04X\n", rd);
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return ADNS5050_ERR_INVALID_INV_REV_ID;                   // abend
    }
    dprint("Adns5050::init OK\n");

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
    return ADNS5050_ERR_INIT_SUCCESS;
}

uint8_t Adns5050::read(reg_t addr)
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);
	switch (_last_op)
	{
		case ADNS_50x0_READ:
			wait_us(ADNS_5050_tSRR);
			break;
		case ADNS_50x0_WRITE:
			wait_us(ADNS_5050_tSWR);
			break;
		default:
			wait_us(ADNS_5050_tSRR);
			break;
	}

    // OE enabled before writing address
    writePinLow(_pin_oe);             // _pin_oe --> LO
    writePinLow(_pin_cs);             // _pin_cs --> LO

    // write address
	uint8_t rd = SPI_TransferByte(addr);
	if (rd != addr) {
      writePinHigh(_pin_cs);             // _pin_cs --> HI
      writePinHigh(_pin_oe);             // _pin_oe --> HI

		dprintf("  Adns5050::read error: addr=%02X\n", addr);
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
		return 0xff;                    // abend
	}
	
	wait_us(ADNS_50x0_tSRAD);

    // OE disabled before reading data
    writePinHigh(_pin_oe);             // _pin_oe --> HI

    // read data
	rd = SPI_ReceiveByte();

    writePinHigh(_pin_cs);             // _pin_cs --> HI
    dprintf("  Adns5050::read OK: addr=%02X, data=%02X\n", addr, rd);

	_last_op = ADNS_50x0_READ;
    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
	return rd; 
}

void Adns5050::write(reg_t addr, uint8_t value)
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);
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
      writePinLow(_pin_oe);             // _pin_oe --> LO
      writePinLow(_pin_cs);             // _pin_oe --> LO

	// write address
	uint8_t rd = SPI_TransferByte(addr | ADNS_50x0_WRITE);
	if (rd != (addr | ADNS_50x0_WRITE)) {
        writePinHigh(_pin_cs);             // _pin_cs --> HI
        writePinHigh(_pin_oe);             // _pin_oe --> HI

		dprintf("  Adns5050::write error: addr=%02X, value=%02X\n", addr, value);
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return;	
	}

	// wait_us(ADNS_50x0_tSRAD);

	// write data
	SPI_SendByte(value);

    writePinHigh(_pin_cs);             // _pin_cs --> HI
    writePinHigh(_pin_oe);             // _pin_oe --> HI

	_last_op = ADNS_50x0_WRITE;

    dprintf("  Adns5050::write OK: addr=%02X, data=%02X\n", addr, value);
    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

void Adns5050::power_down_mode()
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    uint8_t rd = read(REG_MOUSE_CONTROL);
    // todo bit-field
    uint8_t wt = rd | 0b00000010;      // goto power down mode
    write(REG_MOUSE_CONTROL, wt);

    dprint("  Adns5050::enter the power-down-mode\n");
    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

void Adns5050::wakeup()
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    reset();
    dprint("  Adns5050::wakeup from the power-down-mode\n");

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

void Adns5050::reset()
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    dprint("  Adns5050::reset ADNS-5050\n");
    writePinLow(_pin_reset);             // _pin_reset --> LO
    wait_us(2);                          // tRESET >= 250 ns
    writePinHigh(_pin_reset);            // _pin_reset --> HI
    wait_ms(55);                         // tWAKEUP >= 55 ms!

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}
