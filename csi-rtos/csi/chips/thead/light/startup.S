/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

 /******************************************************************************
 * @file     startup.S
 * @brief    startup file. Should use with
 *           GCC for CSKY Embedded Processors
 * @version  V1.0
 * @date     29. July 2019
 ******************************************************************************/

#include <csi_config.h>

.section .vectors
    .align  6
    .globl  __Vectors
    .type   __Vectors, @object
__Vectors:
    j   Default_Handler       /* 0 */
    j   Stspend_Handler       /* 1 */
    j   Default_Handler       /* 2 */
    j   Mtspend_Handler       /* 3 */
    j   Default_Handler       /* 4 */
    j   Scoret_Handler        /* 5 */
    j   Default_Handler       /* 6 */
    j   Mcoret_Handler        /* 7 */
    j   Default_Handler       /* 8 */
    j   Sirq_Handler          /* 9 */
    j   Default_Handler       /* 10 */
    j   Default_IRQHandler    /* 11 */
    j   Default_Handler
    j   Default_Handler
    j   Default_Handler
    j   Default_Handler

    .text
    .align  2
_start:
    .text
    .align  2
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    /* disable ie and clear all interrupts */
    csrw    mie, zero
    csrw    mip, zero
    csrc    mstatus, 8
.option push
.option norelax
    la      gp, __global_pointer$
.option pop

    /* initialize general registers */
    li      x0, 0
    li      x1, 0
    li      x2, 0
    li      x4, 0
    li      x5, 0
    li      x6, 0
    li      x7, 0
    li      x8, 0
    li      x9, 0
    li      x10, 0
    li      x11, 0
    li      x12, 0
    li      x13, 0
    li      x14, 0
    li      x15, 0
    li      x16, 0
    li      x17, 0
    li      x18, 0
    li      x19, 0
    li      x20, 0
    li      x21, 0
    li      x22, 0
    li      x23, 0
    li      x24, 0
    li      x25, 0
    li      x26, 0
    li      x27, 0
    li      x28, 0
    li      x29, 0
    li      x30, 0
    li      x31, 0

    /* pmp config */
    /* 0xff d8000000 ~ 0xff e0000000 plic, NAPOT rw */
    /* 0xff e0000000 ~ 0x100 00000000 peripherals, NAPOT rw */
    /* 0xff ffd00000 ~ 0xff ffe00000 BROM, NAPOT rx */
    /* 0x00000000 ~ 0x10000000 NAPOT no access */
    /* 0x10000000 ~ 0x10000000000 TOR no access */
    li      a0, 0x3ff6fffe00
    csrw    pmpaddr1, a0
    li      a0, 0x3ffbfffe00
    csrw    pmpaddr2, a0
    li      a0, 0x3ffff5fe00
    csrw    pmpaddr3, a0
    li      a0, 0x1fffe00
    csrw    pmpaddr6, a0
    li      a0, 0x3ffffffe00
    csrw    pmpaddr7, a0
    li      a0, 0x889800001d1b1b00
    csrw    pmpcfg0, a0

    /* enable theadisaee */
    csrr    a0, mxstatus
    li      a1, 1 << 22
    or      a0, a0, a1
    csrw    mxstatus, a0

    /* enable cpu fs support */
    csrr    a0, mstatus
    li      a1, 1 << 13
    li      a2, 1 << 23
    or      a0, a0, a1
    or      a0, a0, a2
    csrw    mstatus, a0

    la      a0, __Vectors
    li      a1, 0x1
    or      a0, a0,a1
    csrw    mtvec, a0

    /* initialize all float registers */
    fmv.d.x f0, x0
    fmv.d.x f1, x0
    fmv.d.x f2, x0
    fmv.d.x f3, x0
    fmv.d.x f4, x0
    fmv.d.x f5, x0
    fmv.d.x f6, x0
    fmv.d.x f7, x0
    fmv.d.x f8, x0
    fmv.d.x f9, x0
    fmv.d.x f10, x0
    fmv.d.x f11, x0
    fmv.d.x f12, x0
    fmv.d.x f13, x0
    fmv.d.x f14, x0
    fmv.d.x f15, x0
    fmv.d.x f16, x0
    fmv.d.x f17, x0
    fmv.d.x f18, x0
    fmv.d.x f19, x0
    fmv.d.x f20, x0
    fmv.d.x f21, x0
    fmv.d.x f22, x0
    fmv.d.x f23, x0
    fmv.d.x f24, x0
    fmv.d.x f25, x0
    fmv.d.x f26, x0
    fmv.d.x f27, x0
    fmv.d.x f28, x0
    fmv.d.x f29, x0
    fmv.d.x f30, x0
    fmv.d.x f31, x0

    /* init vtype */
    li      x31, 0x1
    vsetvli x31, x0, e8, m1
    vmv.v.x v0, x0
    vmv.v.x v1, x0
    vmv.v.x v2, x0
    vmv.v.x v3, x0
    vmv.v.x v4, x0
    vmv.v.x v5, x0
    vmv.v.x v6, x0
    vmv.v.x v7, x0
    vmv.v.x v8, x0
    vmv.v.x v9, x0
    vmv.v.x v10, x0
    vmv.v.x v11, x0
    vmv.v.x v12, x0
    vmv.v.x v13, x0
    vmv.v.x v14, x0
    vmv.v.x v15, x0
    vmv.v.x v16, x0
    vmv.v.x v17, x0
    vmv.v.x v18, x0
    vmv.v.x v19, x0
    vmv.v.x v20, x0
    vmv.v.x v21, x0
    vmv.v.x v22, x0
    vmv.v.x v23, x0
    vmv.v.x v24, x0
    vmv.v.x v25, x0
    vmv.v.x v26, x0
    vmv.v.x v27, x0
    vmv.v.x v28, x0
    vmv.v.x v29, x0
    vmv.v.x v30, x0
    vmv.v.x v31, x0
##restore csr to initial state
    vsetvli x0, x0, e128
##reset x31
    li      x31, 0x0

    la      sp, g_top_irqstack

#ifndef __NO_SYSTEM_INIT
    jal     SystemInit
#endif

#ifndef __NO_BOARD_INIT
    jal     pre_main
#endif

    .size   Reset_Handler, . - Reset_Handler

__exit:
    j      __exit

.section .stack
    .align  3
    .global g_base_irqstack
    .global g_top_irqstack
g_base_irqstack:
    .space CONFIG_ARCH_INTERRUPTSTACK
g_top_irqstack:
