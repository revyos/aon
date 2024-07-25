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
#include <drv/irq.h>
#include <drv/tick.h>

void section_data_copy(void);
void section_ram_code_copy(void);
void section_bss_clear(void);
extern void clk_aon_clk_init(void);

static void section_init(void)
{
    section_bss_clear();
}

static void clic_init(void)
{
    int i;

//    CLIC->CLICCFG = 0x4UL;
    /* get interrupt level from info */
    CLIC->CLICCFG = (((CLIC->CLICINFO & CLIC_INFO_CLICINTCTLBITS_Msk) >> CLIC_INFO_CLICINTCTLBITS_Pos) << CLIC_CLICCFG_NLBIT_Pos);

    for (i = 0; i < 64; i++) {
        CLIC->CLICINT[i].IP = 0;
        CLIC->CLICINT[i].ATTR = 1; /* use vector interrupt */
    }

    /* config csitimer tick irq priority */
    //csi_vic_set_prio(DW_TIMER0_IRQn, 3U);
}

static void interrupt_init(void)
{
    clic_init();
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
    //e902 core within light does not support cache

    section_init();

    interrupt_init();

    clk_aon_clk_init();

    //currently no need to support tick-timer
    //csi_tick_init();
}

uint32_t soc_get_cpu_id(void)
{
#ifdef CONFIG_FM_AON_LOW_POWER
    return 1;
#else
    return 4;
#endif
}
