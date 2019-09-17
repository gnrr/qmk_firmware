#pragma once
#include "adns5050.h"

#ifdef __cplusplus
extern "C" {
#endif

class Trackball
{
private:
    Adns5050 _ball_sensor; 
    int8_t   _dx;
    int8_t   _dy;
    int16_t  _scroll;
    bool     _connected;

public:
    Trackball() {}
    bool     init(const uint8_t pin_reset, const uint8_t pin_ncs, const uint8_t pin_noe);
    void     update();
    int8_t   get_dx() { return _dx; }
    int8_t   get_dy() { return _dy; }
    int16_t  get_scroll() { return _scroll; }
    bool     is_connected() { return _connected; }
};

#ifdef __cplusplus
}
#endif
