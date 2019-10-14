#include <avr/interrupt.h>
#include "print.h"          // dprintf, print
#include "quantum.h"
#include "scroll_sensor.h"

static volatile int8_t cnt;

ISR(INT2_vect)
{
    uint8_t _sreg = SREG;
    bool dir = readPin(PIN_DIR);

    if(dir) {
        if(cnt < 127) ++cnt;
    }
    else {
        if(cnt > -127) --cnt;
    }
    SREG = _sreg;
}

bool ScrollSensor::init()
{
    _status = SCROLL_STAT_INIT_ENTER;

    setPinInput(PIN_INT);
    setPinInput(PIN_DIR);
    cli();                      // clear SREG:I(Global Interrupt Enable)
    uint8_t _eimsk = EIMSK;
    EIMSK = 0;
    EICRA |=  _BV(ISC21);       // INT2 trigger: falling edge
    EICRA &= ~_BV(ISC20);       // INT2 trigger: falling edge
    EIMSK =  _eimsk | _BV(INT2);        // INT2 enable
    sei();                      // set SREG:I(Global Interrupt Enable)
    cnt = 0;
    _status = SCROLL_STAT_INIT_SUCCESS;
    return true;
}

int8_t ScrollSensor::get() const
{
    dprintf(">> %s\n", __PRETTY_FUNCTION__);

    if(_status < SCROLL_STAT_INIT_SUCCESS) {
        dprintf("  ScrollSensor::get error: un-initialized scroll sensor. status:%d\n", _status);
        dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return 0;                                            // abend
    }

    cli();
    int8_t val = cnt;
    cnt = 0;
    sei();

    dprintf("<< %s\n", __PRETTY_FUNCTION__);
    return val;
}

