#include "asceension.h"

dsw_t DipSW;

void read_dipsw(void)
{
    //          PORT    ON(LO)  OFF(HI)     action
    // DSW1     PD4     
    // DSW2     PD6     
    // DSW3     PD7     
    // DSW4     PE6     Win     Mac         change default layer

    // lo-active
    DipSW.bit.sw1 = ((PIND & (1 << 4)) == 0)? 1 : 0;    // DSW1
    DipSW.bit.sw2 = ((PIND & (1 << 6)) == 0)? 1 : 0;    // DSW2
    DipSW.bit.sw3 = ((PIND & (1 << 7)) == 0)? 1 : 0;    // DSW3
    DipSW.bit.sw4 = ((PINE & (1 << 6)) == 0)? 1 : 0;    // DSW4

    // dprintf("%d\n", DipSW.bit.sw4);

    // DSW4
    uint8_t default_layer;
    default_layer = (1<<(DipSW.bit.sw4));
    default_layer_set((uint32_t)default_layer);
}

