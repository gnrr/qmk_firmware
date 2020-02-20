#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#define PIN_INT D2		// PD2/INT2
#define PIN_DIR E6		// PE6
#define INVERT true

class ScrollSensor
{
public:
    enum Status {
        eSTAT_CONSTRUCT = 0,
        eSTAT_INIT_ENTER,
        eSTAT_INIT_SUCCESS
    };

private:
    Status status_;

public:
    ScrollSensor();
    bool init();
    int8_t get() const;
    Status get_status() const {return status_;};
    void init() const;
    void clear() const;
};

#ifdef __cplusplus
}
#endif
