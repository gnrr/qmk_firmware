/*
    ino.cpp - main part of a keyboard Ino
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

// #include "config.h"         // USER_PRINT
// #include "quantum/pointing_device.h"         // report_mouse_t
// #include "quantum.h"        // IS_LAYER_ON
#include "action_layer.h"   // keyrecord_t 
#include "report.h"         // report_mouse_t
// #include "host.h"           // host_mouse_send
#include "print.h"          // dprintf, print
#include "debug.h"
#include "quantum.h"        // pin_t
#include "trackball.h"
#include "scroll_sensor.h"
#include "ino.h"

bool enable_horizontal_scroll;           // true: horizontal scroll, false: vertical scroll

static Trackball tb;
static ScrollSensor scroll;

// Disable name mangling so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_init(void)
{
    // debug_enable = true;
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    bool result = scroll.init();
    if(!result) {
        print("  ScrollSensor error: cannot initialize scroll sensor\n");
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return;                                     // abend
    }

    result = tb.init(PIN_RESET, PIN_CS, PIN_OE);                   // todo pin defines --> this .c
    if(!result) {
        print("  Trackball error: cannot initialize ball sensor\n");
        // dprintf("<< %s\n", __PRETTY_FUNCTION__);
        return;                                     // abend
    }

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

extern "C" report_mouse_t pointing_device_get_report(void);
extern "C" void pointing_device_set_report(report_mouse_t newMouseReport);
extern "C" void pointing_device_send();

// Disable name mangling so that this function can be called from QMK's keyboard_task() written in C.
extern "C"
void pointing_device_task(void)
{
    // debug_enable = true;
    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

    tb.update();
    int8_t x = tb.get_dx();
    int8_t y = tb.get_dy();
    int8_t s;
    if((x == 0) && (y == 0)) {
        // dprintf("stat:%d\t", scroll.get_status());
        s = scroll.get();
        // dprintf("s:%d\n", s);
    }
    else {
        scroll.clear();     // not accept scrolling while moving pointer
        s = 0;
    }

	report_mouse_t currentReport = pointing_device_get_report();

    currentReport.x = x;                                // pointer x -127 .. 127
    currentReport.y = y;                                // pointer y -127 .. 127
    currentReport.v = (enable_horizontal_scroll)? 0:s;  // scroll  v -127 .. 127
    currentReport.h = (enable_horizontal_scroll)? s:0;  // scroll  h -127 .. 127
    debug_enable = true;
    // dprintf("s:%2d, h:%2d, -h:%2d\n", s, currentReport.h, -currentReport.h);
    debug_enable = false;
    enable_horizontal_scroll = false;

	pointing_device_set_report(currentReport);
    pointing_device_send();

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

const uint16_t PROGMEM fn_actions[] = {
    [0] = ACTION_FUNCTION(0),  // mouse button left
    [1] = ACTION_FUNCTION(1),  // mouse button right
    [2] = ACTION_FUNCTION(2),  // mouse button center
};

extern "C"
void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    // Just write mouse-buttons status into currentReport without sending mouse-report.
    // The mouse-report is sent with x, y data by pointing_device_task afterwards.
    // MOUSEKEY sends the mouse-report to the host at the time so it cannot be used in this case. 
    // Because using MOUSEKEY occurs mouse-report sending twice at a main-loop.   

    // dprintf(">> %s\n", __PRETTY_FUNCTION__);

	report_mouse_t currentReport = pointing_device_get_report();

    switch (id) {
        case 0:
            if (record->event.pressed) {
                currentReport.buttons |=  MOUSE_BTN1;
            } else {
                currentReport.buttons &= ~MOUSE_BTN1;
            }
            break;

        case 1:
            if(record->event.pressed) {
                currentReport.buttons |=  MOUSE_BTN2;
            } else {
                currentReport.buttons &= ~MOUSE_BTN2;
            }
            break;

        case 2:
            if(record->event.pressed) {
                currentReport.buttons |=  MOUSE_BTN3;
            } else {
                currentReport.buttons &= ~MOUSE_BTN3;
            }
            break;

        default:
            break;
    }
	pointing_device_set_report(currentReport);
    pointing_device_send();

    // dprintf("<< %s\n", __PRETTY_FUNCTION__);
}

#if 0
pin_t dbg_out_pins[] = {D2, D3};
#define DBG_OUT_PINS_SZ   sizeof(dbg_out_pins)/sizeof(dbg_out_pins[0])

void dbg_out_init(void)
{
    for(uint8_t i=0; i<DBG_OUT_PINS_SZ; i++) {
        pin_t pin = dbg_out_pins[i];
        setPinOutput(pin);              // output
        writePinLow(pin);               // --> LO
    }
}

void dbg_out(pin_t pin, uint8_t level)
{
    // assert(pin == D2 || pin == D3);

    if(level == 0) {
        writePinLow(pin);               // --> LO
    }
    else {
        writePinHigh(pin);              // --> HI
    }
}
#endif

