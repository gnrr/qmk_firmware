#include <avr/interrupt.h>
#include "print.h"          // dprintf, print
#include "quantum.h"        // readPin, setPinInput
#include "bootleg.h"        // dbg_out
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
