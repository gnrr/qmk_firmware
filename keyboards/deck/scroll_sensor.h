#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define PIN_INT D2		// PD2/INT2
#define PIN_DIR D3		// PD3
#define MAX 3

typedef enum {
    SCROLL_STAT_CONSTRUCT = 0,
    SCROLL_STAT_INIT_ENTER,
    SCROLL_STAT_INIT_SUCCESS
} ScrollSensorStatus;

class ScrollSensor
{
    ScrollSensorStatus _status;
public:
            ScrollSensor() :_status(SCROLL_STAT_CONSTRUCT){}
    bool    init();
    int8_t  get() const;
    ScrollSensorStatus get_status() const {return _status;}
};

#ifdef __cplusplus
}
#endif
