#include <avr/io.h> // PIND
#include "action_layer.h"
#include "dipsw.h"

void read_dipsw(dsw_t* pdsw)
{
    //        ON    OFF      action
    // DSW1 
    // DSW2 
    // DSW3 
    // DSW4 : Win   Mac     change keymap

    // lo-active
    pdsw->dsw1_opt  = ((PIND & (1 << 4)) == 0)? DSW_OPT_ON    :DSW_OPT_OFF;    // DSW1 PD4: ON(lo)=ON,   OFF(hi)=OFF
    pdsw->dsw2_opt  = ((PIND & (1 << 6)) == 0)? DSW_OPT_ON    :DSW_OPT_OFF;    // DSW2 PD6: ON(lo)=ON,   OFF(hi)=OFF
    pdsw->dsw3_opt  = ((PIND & (1 << 7)) == 0)? DSW_OPT_ON    :DSW_OPT_OFF;    // DSW3 PD7: ON(lo)=ON,   OFF(hi)=OFF
    pdsw->dsw4_host = ((PINE & (1 << 6)) == 0)? DSW_KEYMAP_WIN:DSW_KEYMAP_MAC; // DSW4 PE6: ON(lo)=Win,  OFF(hi)=Mac

    uint8_t default_layer;
    default_layer = (1<<(pdsw->dsw4_host));
    default_layer_set((uint32_t)default_layer);
}

