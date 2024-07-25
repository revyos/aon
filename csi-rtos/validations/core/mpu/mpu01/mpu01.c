/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include "dtest.h"
#include "test_device.h"

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
    __asm__ __volatile__("ipush\n\r"
                         "subi    sp, 4\n\r"
                         "stw     r15, (sp, 0)\n\r"
                         "mfcr    r2, epc\n\r"
                         "addi    r2, 4\n\r"
                         "mtcr    r2, epc\n\r"
                         "jbsr    print_test_info\n\r"
                         "ldw     r15, (sp, 0)\n\r"
                         "addi    sp, 4\n\r"
                         "ipop\n\r"
                         "rte\n\r");
}

int test_mpu(void)
{
    mpu_region_attr_t attr;

    __Vectors[2] = (uint32_t)test_mpu_handler;

    attr.nx = 0;
    attr.ap = AP_BOTH_RW;
    attr.s = 0;

    printf("region 3: 0x%x--0x%x\n", (uint32_t)&test_buf, (uint32_t)&test_buf + 4096);

    csi_mpu_config_region(2, (uint32_t)0x0, REGION_SIZE_4GB, attr, 1);
    csi_mpu_config_region(3, (uint32_t)&test_buf, REGION_SIZE_4KB, attr, 1);
    csi_mpu_enable();

    test_buf[0] = 1;

    attr.ap = AP_BOTH_INACCESSIBLE;
    csi_mpu_config_region(3, (uint32_t)&test_buf, REGION_SIZE_4KB, attr, 1);

    test_buf[0] = 1; /* Trigger bus error exception */

    csi_mpu_disable_region(3);

    return 0;
}

int main(void)
{
    test_mpu();

    return 0;
}
