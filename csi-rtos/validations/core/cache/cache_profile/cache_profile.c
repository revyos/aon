/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include "test_device.h"
#include "dtest.h"

#define __DEBUG 0

#define MAXSIZE 1<<14
#define TIME_ILOCK_LOOPS 10000

#if (__ICACHE_PRESENT == 1U)

volatile char m[MAXSIZE];

void iloop1(unsigned long *start, unsigned long *end, int dummy);

void access_addr(unsigned long start, unsigned long times, unsigned long size)
{
    int i;
    unsigned long reg;

    for (i = 0; i < times; i++) {
        reg = *(unsigned long *)(start + size * i);
        reg++;
    }
}

void tst_iloop0(unsigned long *start, unsigned long *end, int dummy)
{
    int k;

    for (k = 0; k < 10000; k++) {
        csi_dcache_clean_invalid();
        access_addr((unsigned long)m, 1024 * 32, 32);
    }
}

void tst_iloop1(unsigned long *start, unsigned long *end, int dummy)
{
    int k;

    for (k = 0; k < 10000; k++) {
        csi_dcache_clean_invalid();
        access_addr((unsigned long)m, 1024 * 32, 4);
    }
}


void __attribute__((noinline)) cache_profile(void)
{
    uint32_t misscount0, misscount1, misscount2, misscount3;
    uint32_t hitcount0, hitcount1, hitcount2, hitcount3;
    unsigned long start, end;
    csi_dcache_clean_invalid();
    csi_icache_disable();
    csi_icache_invalid();
    csi_cache_enable_profile();
    csi_icache_enable();
    csi_dcache_enable();
    csi_dcache_clean_invalid();
    csi_cache_reset_profile();

    tst_iloop0(&start, &end, 0);
    hitcount0 = csi_cache_get_access_time();
    misscount0 = csi_cache_get_miss_time();

    printf("Instruction cache enable\n");
    printf("cache access: %d, cache miss: %d \n", hitcount0, misscount0);
    ASSERT_TRUE(hitcount0 > 0 && misscount0 > 0);

    csi_cache_reset_profile();
    csi_icache_disable();
    csi_icache_invalid();
    csi_icache_enable();
    csi_dcache_enable();

    csi_dcache_clean_invalid();
    csi_cache_reset_profile();

    tst_iloop0(&start, &end, 0);

    hitcount1 = csi_cache_get_access_time();
    misscount1 = csi_cache_get_miss_time();

    printf("Instruction Cache Invalidate and CacheProfile reset,\n");
    printf("cache access: %d, cache miss: %d \n", hitcount1, misscount1);
    ASSERT_TRUE(hitcount0 == hitcount1  && misscount0 == misscount1);

////////////////////////////////////////////////////////////////////////////
    csi_cache_reset_profile();
    csi_icache_disable();
    csi_icache_invalid();
    csi_icache_enable();
    csi_dcache_enable();

    //    SCB_InvalidateCache();
    csi_dcache_clean_invalid();
    csi_cache_reset_profile();


    csi_icache_invalid();
    csi_cache_reset_profile();

    tst_iloop1(&start, &end, 0);

    hitcount2 = csi_cache_get_access_time();
    misscount2 = csi_cache_get_miss_time();
    printf("Instruction cache Invalidate and CacheProfile reset and more access  on same cache line,\n");
    printf("cache access: %d, cache miss: %d \n", hitcount2, misscount2);
    ASSERT_TRUE(hitcount0 < hitcount2  && misscount0 > misscount2);

    csi_icache_disable();
    csi_icache_invalid();
    csi_dcache_disable();
    csi_dcache_invalid();
    csi_cache_set_range(0, 0, 0x13, 0);
    csi_cache_reset_profile();
    tst_iloop1(&start, &end, 0);

    hitcount3 = csi_cache_get_access_time();
    misscount3 = csi_cache_get_miss_time();

    printf(" Instruction Cache and Data Cache disable,\n");
    printf("cache access: %d, cache miss: %d \n", hitcount3, misscount3);
    ASSERT_TRUE(hitcount3 == 0 && misscount3 == 0);
}

#endif


int test_cache_profile(void)
{
#if 0
    printf("Testing cache api\n");
#ifdef SMARTL_RAM0_BASE
    csi_cache_set_range(0, SMARTL_RAM0_BASE, SMARTL_CACHE_CRCR0, 1);
#endif
#ifdef SMARTL_RAM1_BASE
    csi_cache_set_range(1, SMARTL_RAM1_BASE, SMARTL_CACHE_CRCR1, 1);
#endif
#if (__ICACHE_PRESENT == 1U)
    cache_profile();
#endif
    return 0;
#else
    printf("Unsupport Testing cache api\n");
    return 0;
#endif
}

int main(void)
{
    test_cache_profile();
}

