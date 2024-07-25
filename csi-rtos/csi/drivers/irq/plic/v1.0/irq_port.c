/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     irq_port.c
 * @brief    CSI Source File for irq port
 * @version  V1.0
 * @date     17. Mar 2020
 * @name     dw_uart
 ******************************************************************************/

#include <csi_config.h>
#include <stdbool.h>
#include <csi_core.h>
#include <soc.h>

#define PLIC    ((PLIC_Type*) PLIC_BASE)

void soc_irq_enable(uint32_t irq_num)
{
#ifdef CONFIG_MMU
    PLIC->PLIC_H0_SIE[irq_num / 32] = PLIC->PLIC_H0_SIE[irq_num / 32] | (0x1 << (irq_num % 32));
#else
    PLIC->PLIC_H0_MIE[irq_num / 32] = PLIC->PLIC_H0_MIE[irq_num / 32] | (0x1 << (irq_num % 32));
#endif
}

void soc_irq_disable(uint32_t irq_num)
{
#ifdef CONFIG_MMU
    PLIC->PLIC_H0_SIE[irq_num / 32] = PLIC->PLIC_H0_SIE[irq_num / 32] & (~(0x1 << (irq_num % 32)));
#else
    PLIC->PLIC_H0_MIE[irq_num / 32] = PLIC->PLIC_H0_MIE[irq_num / 32] & (~(0x1 << (irq_num % 32)));
#endif
}

bool soc_irq_is_enabled(uint32_t irq_num)
{
    bool ret;

#ifdef CONFIG_MMU

    if ((uint32_t)((PLIC->PLIC_H0_SIE[irq_num / 32] >> irq_num % 32) & 0x1)) {
#else

    if ((uint32_t)((PLIC->PLIC_H0_MIE[irq_num / 32] >> irq_num % 32) & 0x1)) {
#endif
        ret = true;
    } else {
        ret = false;
    }

    return ret;
}

void soc_irq_priority(uint32_t irq_num, uint32_t priority)
{
    PLIC->PLIC_PRIO[irq_num] = priority;
}

void soc_irq_enable_wakeup(uint32_t irq_num)
{
}

void soc_irq_disable_wakeup(uint32_t irq_num)
{
}

uint32_t soc_irq_get_irq_num(void)
{
#ifdef CONFIG_MMU
    uint32_t num = PLIC->PLIC_H0_SCLAIM;
#else
    uint32_t num = PLIC->PLIC_H0_MCLAIM;
#endif
    return num;
}

void soc_irq_end(uint32_t irq_num)
{
#ifdef CONFIG_MMU
    PLIC->PLIC_H0_SCLAIM = irq_num;
#else
    PLIC->PLIC_H0_MCLAIM = irq_num;
#endif
}
