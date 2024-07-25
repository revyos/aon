/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include "dtest.h"
#include "test_device.h"

#define TEST_ASID    0

#define TLB_PAGESIZE_4KB             0x1000       //0x0000
#define TLB_PAGESIZE_16KB            0x4000       //0x3000
#define TLB_PAGESIZE_64KB            0x10000      //0xf000
#define TLB_PAGESIZE_256KB           0x40000      //0x3f000
#define TLB_PAGESIZE_1MB             0x100000     //0xff000
#define TLB_PAGESIZE_4MB             0x400000     //0x3ff000
#define TLB_PAGESIZE_16MB            0x1000000    //0xfff000

#define TEST_SRAM_BASE     (CSKY_SRAM_BASE - 0x80000000)
#define TEST_MMU_PAGEMASK  PAGE_SIZE_4KB  //PAGE_SIZE_16KB PAGE_SIZE_64KB PAGE_SIZE_256KB PAGESIZE__1MB PAGE_SIZE_4MB PAGE_SIZE_16MB
#define TEST_MMU_PAGESIZE  TLB_PAGESIZE_4KB  //TLB_PAGESIZE_16KB TLB_PAGESIZE_64KB TLB_PAGESIZE_256KB TLB_PAGESIZE_1MB TLB_PAGESIZE_4MB TLB_PAGESIZE_16MB
#define TEST_MMU_VIRT_ORIGIN    0x0

int test_mmu(void)
{
    page_attr_t test_attr;
    test_attr.global = 1;
    test_attr.valid = 1;
    test_attr.writeable = 1;
    test_attr.cacheable = 1;

    csi_mmu_set_pagesize(TEST_MMU_PAGEMASK);
    csi_mmu_set_tlb(TEST_MMU_VIRT_ORIGIN, TEST_SRAM_BASE, TEST_ASID, test_attr);
    csi_mmu_enable();

    int i = 0;

    for (; i < 100000; i++) {
    }

    i = 0;

    for (; i < TEST_MMU_PAGESIZE; i += 4) {
        ASSERT_TRUE((*(uint32_t *)(TEST_MMU_VIRT_ORIGIN + i)) == (*(uint32_t *)(0x80000000 + i)));
    }

    csi_mmu_set_tlb(TEST_MMU_VIRT_ORIGIN + 0x1000, TEST_SRAM_BASE + TEST_MMU_PAGESIZE, TEST_ASID, test_attr);
    i = 0;

    for (; i < TEST_MMU_PAGESIZE; i += 4) {
        ASSERT_TRUE((*(uint32_t *)(0x1000 + TEST_MMU_VIRT_ORIGIN + i)) == (*(uint32_t *)(0x80000000 + 0x1000 + i)));
    }

    return 0;
}

int main(void)
{
    return test_mmu();
}
