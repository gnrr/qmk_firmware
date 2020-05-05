/*
    scroll_sensor.h - Scroll sensor using a pair of photo-transistor and IR-LED
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

#pragma once

#define PIN_INT D2          // PD2/INT2
#define PIN_DIR E6          // PE6

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

