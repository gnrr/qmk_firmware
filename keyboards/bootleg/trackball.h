#pragma once
#include "adns5050.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_RESET F1
#define PIN_CS    C7
#define PIN_OE    F0

class Trackball
{
    Adns5050    _ball_sensor;
    Adns5050Err _sensor_status;
    int8_t      _x;
    int8_t      _y;
    int8_t      _v;
    int8_t      zero_adjust(int8_t val);
    int8_t      limit(int8_t n, int8_t min, int8_t max);

public:
             Trackball() {}
    bool     init(const uint8_t pin_reset, const uint8_t pin_cs, const uint8_t pin_oe);
    bool     update();
    int8_t   get_x() const { return _x; }
    int8_t   get_y() const { return _y; }
    int8_t   get_v() const { return _v; }
    void     set_v(const uint8_t v) { _v = v; }
};

#ifdef __cplusplus
}
#endif
