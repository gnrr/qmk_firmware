#pragma once

#include "asceension.h"

typedef union {
    struct {
        uint8_t sw1: 1;
        uint8_t sw2: 1;
        uint8_t sw3: 1;
        uint8_t sw4: 1;
        uint8_t dummy: 4;
    } bit;
    uint8_t all;
} dsw_t;

extern void read_dipsw(void);

