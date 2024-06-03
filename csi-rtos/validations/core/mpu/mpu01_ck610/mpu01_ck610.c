/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


#include <stdio.h>
#include "dtest.h"
#include "test_device.h"

extern uint32_t __Vectors[];

static uint8_t __attribute__((aligned(4096))) test_buf[4096];

void print_test_info(void)
{
    ASSERT_TRUE(1);
}

void __attribute__((naked)) test_mpu_handler(void)
{
    __asm__ __volatile__("subi    sp, 32\n\r"
                         "stw     r1, (sp, 0)\n\r"
                         "stw     r2, (sp, 4)\n\r"
                         "stw     r3, (sp, 8)\n\r"
                         "stw     r4, (sp, 12)\n\r"
                         "stw     r5, (sp, 16)\n\r"
                         "stw     r6, (sp, 20)\n\r"
                         "stw     r7, (sp, 24)\n\r"
                         "stw     r15, (sp, 28)\n\r"
                         "mfcr    r2, epc\n\r"
                         "addi    r2, 2\n\r"
                         "mtcr    r2, epc\n\r"
                         "jbsr    print_test_info\n\r"
                         "ldw     r1, (sp, 0)\n\r"
                         "ldw     r2, (sp, 4)\n\r"
                         "ldw     r3, (sp, 8)\n\r"
                         "ldw     r4, (sp, 12)\n\r"
                         "ldw     r5, (sp, 16)\n\r"
                         "ldw     r6, (sp, 20)\n\r"
                         "ldw     r7, (sp, 24)\n\r"
                         "ldw     r15, (sp, 28)\n\r"
                         "addi    sp, 32\n\r"
                         "rte\n\r");
}

int test_mpu(void)
{
    mpu_region_attr_t attr;

    __Vectors[2] = (uint32_t)test_mpu_handler;

    attr.ap = AP_BOTH_RW;
    attr.c = 1;

    printf("region 3: 0x%x--0x%x\n", (uint32_t)&test_buf, (uint32_t)&test_buf + 4096);

    csi_mpu_config_region(3, (uint32_t)&test_buf, REGION_SIZE_4KB, attr, 1);
    csi_mpu_enable_region(3);

    test_buf[0] = 1;

    attr.ap = AP_BOTH_INACCESSIBLE;
    csi_mpu_config_region(3, (uint32_t)&test_buf, REGION_SIZE_4KB, attr, 1);

    test_buf[0] = 1; /* Trigger bus error exception */

    csi_mpu_disable_region(3);

    return 0;
}

int main(void)
{
    return test_mpu();
}
