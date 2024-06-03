/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sys_clk.c
 * @brief    source file for setting system frequency.
 * @version  V1.0
 * @date     18. July 2018
 ******************************************************************************/

#include <stdint.h>
#include <soc.h>
#include <sys_clk.h>
#include <drv/io.h>

uint32_t g_system_clock = IHS_VALUE;

uint32_t soc_get_coretim_freq(void)
{
    return g_system_clock;
}

uint32_t soc_get_cpu_freq(uint32_t idx)
{
    return g_system_clock;
}

uint32_t soc_get_cur_cpu_freq(void)
{
    return g_system_clock;
}

uint32_t soc_get_usi_freq(uint32_t idx)
{
    return g_system_clock;
}

uint32_t soc_get_uart_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx);
}

uint32_t soc_get_iic_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx);
}

uint32_t soc_get_spi_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx);
}

uint32_t soc_get_qspi_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx);
}

uint32_t soc_get_pwm_freq(uint32_t idx)
{
    return soc_get_apb_freq(0U);
}

uint32_t soc_get_i2s_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx / 2U);
}

uint32_t soc_get_timer_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx / 2U);
}

uint32_t soc_get_rtc_freq(uint32_t idx)
{
    return ILS_VALUE;
}

uint32_t soc_get_wdt_freq(uint32_t idx)
{
    return g_system_clock;
}

uint32_t soc_get_apb_freq(uint32_t idx)
{

    return g_system_clock;
}

uint32_t soc_get_sdio_freq(uint32_t val)
{
    return g_system_clock;
}

void soc_set_sys_freq(uint32_t val)
{
    g_system_clock = val;
}

void soc_clk_enable(int32_t module)
{
;
}

void soc_clk_disable(int32_t module)
{
;
}

