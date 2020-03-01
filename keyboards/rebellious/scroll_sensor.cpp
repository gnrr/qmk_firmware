/*
    scroll_sensor.cpp - Scroll sensor using a pair of photo-transistor and IR-LED
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

#include <avr/interrupt.h>
#include "print.h"          // dprintf, print
#include "quantum.h"        // readPin, setPinInput
#include "rebellious.h"     // dbg_out
#include "scroll_sensor.h"

static int8_t cnt = 0;

ISR(INT2_vect)
{
    uint8_t sreg_bak = SREG;
//    dbg_out(D5, 1);
    bool dir = readPin(PIN_DIR);
//    dbg_out(D5, 0);

    if(dir ^ INVERT) {
        if(cnt > -127) --cnt;
    }
    else {
        if(cnt < 127) ++cnt;
    }
    SREG = sreg_bak;
}

ScrollSensor::ScrollSensor() :status_(eSTAT_INIT_ENTER) {}

bool ScrollSensor::init()
{
    setPinInput(PIN_INT);
    setPinInput(PIN_DIR);

    cli();                              // clear SREG:I(Disable Global Interrupt)
    uint8_t eimsk_bak = EIMSK;
    EIMSK = 0;
#if 0
    EICRA |=   _BV(ISC21);              // INT2 trigger: falling edge
    EICRA &=  ~_BV(ISC20);
#else
    EICRA |=   _BV(ISC21);              // INT2 trigger: riging edge
    EICRA |=   _BV(ISC20);
#endif
    EIMSK =  eimsk_bak | _BV(INT2);     // INT2 enable
    sei();                              // set SREG:I(Enable Global Interrupt)

    cnt = 0;
    status_ = eSTAT_INIT_SUCCESS;
    return true;
}

int8_t ScrollSensor::get() const
{
    if(status_ != eSTAT_INIT_SUCCESS) {
        dprintf("  ScrollSensor::get error: un-initialized scroll sensor. status:%d\n", status_);
        return 0;                                            // abend
    }

    cli();
    int8_t val = cnt;
    cnt = 0;
    sei();

    return val;
}

void ScrollSensor::clear() const
{
    cli();
    cnt = 0;
    sei();
}
