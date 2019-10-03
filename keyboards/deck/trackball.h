#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"             // USER_PRINT
#include "print.h"
#include "debug.h"              // dprint
#include "adns5050.h"

#define PIN_RESET F1
#define PIN_CS    C7
#define PIN_OE    F0        // provided by 3-state buffer IC

class Trackball
{
    Adns5050    _ball_sensor;
    Adns5050Err _sensor_status;
    int8_t      _dx;
    int8_t      _dy;
    int8_t     _scroll;
    int8_t      zero_adjust(int8_t val);
    int8_t      limit(int8_t n, int8_t min, int8_t max);

public:
             Trackball() {}
    bool     init(const uint8_t pin_reset, const uint8_t pin_cs, const uint8_t pin_oe);
    bool     update();
    int8_t   get_dx() const { return _dx; }
    int8_t   get_dy() const { return _dy; }
    int8_t   get_scroll() const { return _scroll; }
};

#ifdef __cplusplus
}
#endif
