#ifndef _DBG_H
#define _DBG_H   1

#include <print.h>

#define DBG 1

#ifdef DBG
 #define DBG_ROW(row) {if (current_row == row) {print("row:"); phex(row); print(":"); phex(cols.word); print("\n");} }
 #define DBG_HEX(str,var) {print(str); print("0x"); phex(var); print("\n");} 
 #define DBG_HEX2(str,var1, var2) {print(str); print("0x"); phex(var1); print(", 0x"); phex(var2); print("\n");} 
#else
 #define DBG_HEX(str,var) 
 #define DBG_HEX2(str,var1, var2)
#endif

#undef DBG
#endif // _DBG_H

