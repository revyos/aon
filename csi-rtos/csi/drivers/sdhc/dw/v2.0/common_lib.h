#ifndef COMMON_LIB__H
#define COMMON_LIB__H

#include "stdio.h"
#include "time.h"
#include "string.h"
#include <stdarg.h>

#ifdef DPI_USE
    #include "user_dpi.h"
#else
    #define MEM8(addr) *((volatile unsigned char *)(addr))
    #define MEM16(addr) *((volatile unsigned short *)(addr))
    #define MEM32(addr) *((volatile unsigned int *)(addr))
    #define MEM64(addr) *((volatile unsigned long *)(addr))
#endif


//#define finish(val) while(1);//return;
#define EMMC_MULTI_BLOCK_XFER 1

typedef unsigned long  u64;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
enum result{
    PASS,
    FAIL,
    ABNORMAL
};

void finish(int val);


extern void     wr64(u64 addr, u64 data);
extern u64      rd64(u64 addr);
extern void     wr(u64 addr, u32 data);
extern u32      rd(u64 addr);
extern void     wr16(u64 addr, u16 data);
extern u16      rd16(u64 addr);
extern void     wr8(u64 addr, u8 data);
extern u8       rd8(u64 addr);
extern void     tb_wr(u64 addr, u32 data);
//extern void     finish(enum result rslt);
extern void     wait_ns(const u32 delay);
extern int      get_cup_id();

//#include "interrupt.h"
//#include "base_addr_defines.h"
//#include "offset_addr_defines.h"
//#include "sim_ctrl_regs.h"
//#include "app_lib.h"
//#include "sys_init.h"
//#define _DDR_MC_BADDR _DDR_CTRL_BADDR
#endif
