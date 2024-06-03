/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     trap_c.c
 * @brief    source file for the trap process
 * @version  V1.0
 * @date     12. December 2017
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <csi_config.h>
#include <csi_core.h>

void (*trap_c_callback)(void);

void trap_c(uint32_t *regs)
{
    int i;
    uint32_t vec = 0;

    vec = __get_MCAUSE() & 0x3FF;

    printf("CPU Exception: NO.%d", vec);
    printf("\n");

    for (i = 0; i < 15; i++) {
        printf("x%d: 0x%x\t", i + 1, regs[i]);

        if ((i % 4) == 3) {
            printf("\n");
        }
    }

    printf("\n");
    printf("mepc   : 0x%x\n", regs[15]);
    printf("mstatus: 0x%x\n", regs[16]);
    printf("mtval: 0x%x\n", regs[17]);

    if (trap_c_callback) {
        trap_c_callback();
    }

    while (1);
}

