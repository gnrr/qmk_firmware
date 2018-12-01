#pragma once

typedef enum {DSW_KEYMAP_MAC=0, DSW_KEYMAP_WIN} dsw_host_t;
typedef enum {DSW_OPT_OFF=0, DSW_OPT_ON} dsw_opt_t;

typedef struct {
    dsw_opt_t dsw0_opt;
    dsw_opt_t dsw1_opt;
    dsw_opt_t dsw2_opt;
    dsw_opt_t dsw3_host;
} dsw_t;

extern void read_dipsw(dsw_t* pdsw);
