#include <stdlib.h>                // abs
#include "LUFA/SPI.h"
#include "trackball.h"

bool Trackball::init(const uint8_t pin_reset, const uint8_t pin_cs, const uint8_t pin_oe)
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    // 2MHz(@fcpu=16MHz), MODE3
    uint8_t spi_opts = SPI_SPEED_FCPU_DIV_8 | SPI_ORDER_MSB_FIRST | SPI_MODE_MASTER
                     | SPI_SCK_LEAD_FALLING | SPI_SAMPLE_TRAILING;

    _sensor_status = _ball_sensor.init(pin_reset, pin_cs, pin_oe, spi_opts);

    if(_sensor_status != ADNS5050_ERR_INIT_SUCCESS) {
        dprintf("  Trackball::init error: can not connect to ball sensor! error code:%d\n", _sensor_status);
        return false;                                       // abend
    }

    dprint("  Trackball::init OK: connected\n");

    // _ball_sensor.power_down_mode();                   // power down mode

    // todo: initialize scroll sensor

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
    return true;                                            // normal end
}

bool Trackball::update()
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    if(_sensor_status != ADNS5050_ERR_INIT_SUCCESS) {
        dprint("  Trackball::update error: can not connect to ball sensor\n");
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return false;                                            // abend
    }

    // _ball_sensor.wakeup();                   // normal mode
  
    uint8_t moving_p = _ball_sensor.read(REG_MOTION);
  
    if(moving_p) {
        int8_t x = _ball_sensor.read(REG_DELTA_X);
        int8_t y = _ball_sensor.read(REG_DELTA_Y);

        const int8_t min = -127;   // values must be in range of -127..127
        if(x < min) x = min;       // limited by specification of the USB report
        if(y < min) y = min;
        _dx = y;
        _dy = -x;

        dprintf("  Trackball::update dx: %4d   dy: %4d\n", _dx, _dy);
    }
    else {
        _dx = 0;
        _dy = 0;
    }
    // _ball_sensor.power_down_mode();                   // power down mode

    // todo: read scroll sensor

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
    return true;                                            // normal end
}

int8_t Trackball::zero_adjust(const int8_t val)
{
    const int8_t THRESHOLD = 1;

    if(abs(val) < THRESHOLD) return 0;
    if(val > 0) return val - THRESHOLD;      
    return val + THRESHOLD;
}

