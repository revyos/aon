/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */


/******************************************************************************
 * @file     system.c
 * @brief    CSI Device System Source File
 * @version  V1.0
 * @date     02. Oct 2018
 ******************************************************************************/

#include <csi_config.h>
#include <soc.h>
#include <csi_core.h>
#include <drv/tick.h>
#include <drv/porting.h>
#include <drv/irq.h>
#include <drv/dma.h>

#ifndef CONFIG_SYSTICK_HZ
#define CONFIG_SYSTICK_HZ 100
#endif

extern uint32_t g_system_clock;
void section_data_copy(void);
void section_ram_code_copy(void);
void section_bss_clear(void);
csi_dma_t g_dma;

static void section_init(void)
{
#ifdef CONFIG_XIP
    section_data_copy();
    section_ram_code_copy();
    csi_dcache_clean();
    csi_icache_invalid();
#endif

    section_bss_clear();
}
static void interrupt_init(void)
{
    int i;
    PLIC_Type *plic_base = (PLIC_Type *) PLIC_BASE;
    /* set PLIC_PER */
    plic_base->PLIC_PER = 0x1;

    for (i = 0; i < 32; i++) {
        plic_base->PLIC_IP[i] = 0;
    }

    for (i = 0; i < 1023; i++) {
        plic_base->PLIC_H0_MCLAIM = i;
    }

    for (i = 0; i < 1023; i++) {
        plic_base->PLIC_PRIO[i] = 31;
    }

    /* set hart threshold 0, enable all interrupt */
    plic_base->PLIC_H0_MTH = 0;

    /* enable msoft interrupt */
    uint32_t mie = __get_MIE();
    mie |= (1 << 3);
    __set_MIE(mie);

    /* config csitimer tick irq priority */
    csi_irq_priority(DW_TIMER0_IRQn, 3U);

#ifdef CONFIG_KERNEL_NONE
    __enable_excp_irq();
#endif
}

/**
  * @brief  initialize the system
  *         Initialize the psr and vbr.
  * @param  None
  * @return None
  */

void SystemInit(void)
{
    /* Invalid & clear IBP BTB BHT ICache & DCache */
    __set_MCOR(0x70013);
    /* Enable L2 Cache */
    __set_MCCR2(0xe0010009);
    section_init();
    __set_MHCR(0); ///< disable cache
    /* enable cache */
    csi_dcache_enable();
    csi_icache_enable();
    csi_error_t ret = csi_dma_init(&g_dma, 0);

    if (ret != CSI_OK) {
        while (1);
    }

    interrupt_init();
    csi_tick_init();
}
uint32_t soc_get_cpu_id(void)
{
    return 0U;
}
