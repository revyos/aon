#include "stdint.h"
#include "common_lib.h"
#include "drv/io.h"

void tb_wr(u64 addr, uint32_t data)
{

}

void wr(u64 addr, uint32_t data)
{
    putreg32(data, (volatile void *)addr);
}

uint32_t rd(u64 addr)
{
    return *(volatile uint32_t *)addr;
}

void wr16(u64 addr, u16 data)
{
    *(volatile uint16_t *)addr = data;
}

u16 rd16(u64 addr)
{
    return *(volatile uint16_t *)addr;
}

void wr8(u64 addr, u8 data)
{
    *(volatile uint8_t *)addr = data;
}

u8 rd8(u64 addr)
{
    return *(volatile uint8_t *)addr;
}
