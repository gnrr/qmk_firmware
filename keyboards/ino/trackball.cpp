#include <stdlib.h>              // abs
#include "LUFA/SPI.h"
#include "config.h"
#include "trackball.h"

bool Trackball::init(const uint8_t pin_reset, const uint8_t pin_cs, const uint8_t pin_oe)
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    // 2MHz(@fcpu=16MHz), MODE3
    uint8_t spi_opts = SPI_SPEED_FCPU_DIV_8 | SPI_ORDER_MSB_FIRST | SPI_MODE_MASTER
                     | SPI_SCK_LEAD_FALLING | SPI_SAMPLE_TRAILING;

    _sensor_status = _ball_sensor.init(pin_reset, pin_cs, pin_oe, spi_opts);

    if(_sensor_status != ADNS5050_ERR_INIT_SUCCESS) {
        dprintf("  Trackball::init error: can not connect to trackball sensor! error code: %d\n", _sensor_status);
        return false;                                       // abend
    }

    dprint("  Trackball::init OK: connected\n");

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
    return true;                                            // normal end
}

bool Trackball::update()
{
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    if(_sensor_status != ADNS5050_ERR_INIT_SUCCESS) {
        dprint("  Trackball::update error: can not connect to trackball sensor\n");
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return false;                                            // abend
    }

    uint8_t moving_p = _ball_sensor.read(REG_MOTION);
  
    if(moving_p) {
        int8_t x = _ball_sensor.read(REG_DELTA_X);
        int8_t y = _ball_sensor.read(REG_DELTA_Y);

        const int8_t min = -127;   // values must be in range of -127..127
        if(x < min) x = min;       // limited by specification of the USB report
        if(y < min) y = min;

#if TRACKBALL_AZIMUTH_ADJ != 0
        {
            static_assert(TRACKBALL_AZIMUTH_ADJ >= -45 && TRACKBALL_AZIMUTH_ADJ <= 45,
                          "'TRACKBALL_AZIMUTH_ADJ' must be set in the range -45 to 45");

            int8_t adj[2];
            azimuth_adjust(adj, x, y);
            x = adj[0];
            y = adj[1];
        }
#endif
        _dx = y;
        _dy = -x;

        dprintf("  Trackball::update dx: %4d   dy: %4d\n", _dx, _dy);
    }
    else {
        _dx = 0;
        _dy = 0;
    }

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

void Trackball::azimuth_adjust(int8_t point[], const int8_t x, const int8_t y)
{
    const float theta = atan2(y, x);
    const float r = sqrt(x*x + y*y);

    float theta2 = theta + deg2rad(TRACKBALL_AZIMUTH_ADJ);
    if(theta2 > 2*M_PI) theta2 -= 2*M_PI;

    const float x2 = r * cos(theta2);
    const float y2 = r * sin(theta2);

    point[0] = static_cast<int8_t>(x2);
    point[1] = static_cast<int8_t>(y2);
}
