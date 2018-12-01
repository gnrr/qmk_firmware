#include <avr/io.h> // PIND
#include "action_layer.h"
#include "dipsw.h"

void read_dipsw(dsw_t* pdsw)
{
    //        OFF   ON      action
    // DSW0 
    // DSW1 
    // DSW2 
    // DSW3 : MAC   WIN     change keymap

    // lo-active
    pdsw->dsw0_opt  = ((PIND & (1 << 4)) == 0)? DSW_OPT_ON    :DSW_OPT_OFF;    // DSW0 PD4: ON(lo)=Mac, OFF(hi)=Win
    pdsw->dsw1_opt  = ((PIND & (1 << 6)) == 0)? DSW_OPT_ON    :DSW_OPT_OFF;    // DSW1 PD6: ON(lo)=ON,  OFF(hi)=OFF
    pdsw->dsw2_opt  = ((PIND & (1 << 7)) == 0)? DSW_OPT_ON    :DSW_OPT_OFF;    // DSW2 PD7: ON(lo)=ON,  OFF(hi)=OFF
    pdsw->dsw3_host = ((PINE & (1 << 6)) == 0)? DSW_KEYMAP_WIN:DSW_KEYMAP_MAC; // DSW3 PE6: ON(lo)=ON,  OFF(hi)=OFF

    uint8_t default_layer;
    default_layer = (1<<(pdsw->dsw3_host));
    default_layer_set((uint32_t)default_layer);
}

