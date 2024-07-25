#include "common_lib.h"

void wr64(u64 addr, u64 data)
{
    #ifdef DPI_USE
        write_64bit(&addr, &data);
    #else
        MEM64(addr)=data;
    #endif
}

u64 rd64(u64 addr)
{
    u64 data;

    #ifdef DPI_USE
        read_64bit(&addr, &data);
    #else
        data=MEM64(addr);
    #endif
    return data;
}

static inline void putreg32(uint32_t val, volatile void *addr)
{
    *(volatile uint32_t *)addr = val;
}

void wr(u64 addr, u32 data)
{
    #ifdef DPI_USE
        write_32bit(&addr, &data);
        printf("[frontdoor write memory done] addr:%0x, data:%0x\n", addr, data);
    #else
        putreg32(data, (void *)addr);
       // MEM32(addr)=data;
    #endif
}

u32 rd(u64 addr)
{
    u32 data;

    #ifdef DPI_USE
        read_32bit(&addr, &data);
        printf("[frontdoor read memory done] addr:%0x, data:%0x\n", addr, data);
    #else
        data=MEM32(addr);
    #endif
    return data;
}

void wr16(u64 addr, u16 data)
{
    #ifdef DPI_USE
        printf("[frontdoor write memory done] addr:%0x, data:%0x\n", addr, data);
        write_16bit(&addr, &data);
    #else
        MEM16(addr)=data;
    #endif
}

u16 rd16(u64 addr)
{
    u16 data;

    #ifdef DPI_USE
        read_16bit(&addr, &data);
        printf("[frontdoor read memory done] addr:%0x, data:%0x\n", addr, data);
    #else
        data=MEM16(addr);
    #endif
    return data;
}

void wr8(u64 addr, u8 data)
{
    #ifdef DPI_USE
        printf("[frontdoor write memory done] addr:%0x, data:%0x\n", addr, data);
        write_8bit(&addr, &data);
    #else
        MEM8(addr)=data;
    #endif
}

u8 rd8(u64 addr)
{
    u8 data;

    #ifdef DPI_USE
        read_8bit(&addr, &data);
        printf("[frontdoor read memory done] addr:%0x, data:%0x\n", addr, data);
    #else
        data=MEM8(addr);
    #endif
    return data;
}

void tb_wr(u64 addr, u32 data)
{
//    #ifdef DPI_USE
//        printf("[tb write] addr:%0x, data:%0x\n", addr, data);
//        tb_write(&addr, &data);
//    #else
//        wr(addr, data);
//    #endif
}

void finish(int val)
{
    while(0);
}
