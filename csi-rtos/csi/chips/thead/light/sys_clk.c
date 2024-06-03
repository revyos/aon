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

uint32_t g_apb_clock = IHS_VALUE;
uint32_t g_ahb_clock = IHS_VALUE;
uint32_t g_spi_clock = IHS_VALUE;
uint32_t g_cpu_clock = IHS_VALUE;

const perisys_clk_config_t perisys_config[] = {
    {APB_62_5MHZ,   AHB_250MHZ,    2, 3},
    {APB_50MHZ,     AHB_250MHZ,    2, 4},
    {APB_41_7MHZ,   AHB_250MHZ,    2, 5},
    {APB_35_7MHZ,   AHB_250MHZ,    2, 6},
    {APB_31_25MHZ,  AHB_250MHZ,    2, 7},
    {APB_33_33MHZ,  AHB_166_67MHZ, 3, 4},
    {APB_27_78MHZ,  AHB_166_67MHZ, 3, 5},
    {APB_23_81MHZ,  AHB_166_67MHZ, 3, 6},
    {APB_20_83MHZ,  AHB_166_67MHZ, 3, 7},
    {APB_25MHZ,     AHB_125MHZ,    4, 4},
    {APB_17_86MHZ,  AHB_125MHZ,    4, 6},
    {APB_15_625MHZ, AHB_125MHZ,    4, 7},
    {APB_20MHZ,     AHB_100MHZ,    5, 4},
    {APB_16_67MHZ,  AHB_100MHZ,    5, 5},
    {APB_14_29MHZ,  AHB_100MHZ,    5, 6},
    {APB_12_5MHZ,   AHB_100MHZ,    5, 7},
    {APB_13_89MHZ,  AHB_83_33MHZ,  6, 5},
    {APB_11_90MHZ,  AHB_83_33MHZ,  6, 6},
    {APB_10_42MHZ,  AHB_83_33MHZ,  6, 7},
    {APB_10_20MHZ,  AHB_71_43MHZ,  7, 6},
    {APB_8_93MHZ,   AHB_71_43MHZ,  7, 7},
    {APB_7_8125MHZ, AHB_62_5MHZ,   8, 7},
};

uint32_t soc_get_coretim_freq(void)
{
    return g_cpu_clock;
}

uint32_t soc_get_cpu_freq(uint32_t idx)
{
    return g_apb_clock;
}

uint32_t soc_get_cur_cpu_freq(void)
{
    return g_cpu_clock;
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
    return g_spi_clock;
}

uint32_t soc_get_qspi_freq(uint32_t idx)
{
    return g_spi_clock;
}

uint32_t soc_get_pwm_freq(uint32_t idx)
{
    return soc_get_apb_freq(0U);
}

uint32_t soc_get_i2s_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx);
}

uint32_t soc_get_timer_freq(uint32_t idx)
{
    return soc_get_apb_freq(idx);
}

uint32_t soc_get_rtc_freq(uint32_t idx)
{
    return ILS_VALUE;
}

uint32_t soc_get_wdt_freq(uint32_t idx)
{
    return g_apb_clock;
}

uint32_t soc_get_apb_freq(uint32_t idx)
{
    return g_apb_clock;
}

uint32_t soc_get_ahb_freq(uint32_t idx)
{
    return g_ahb_clock;
}

uint32_t soc_get_sdio_freq(uint32_t val)
{
    return g_apb_clock;
}

uint32_t soc_get_emmc_freq(uint32_t idx)
{
    return soc_get_ahb_freq(idx);
}

uint32_t soc_get_usb_freq(uint32_t idx)
{
    return soc_get_ahb_freq(idx);
}

csi_error_t soc_perisys_clk_config(perisys_clk_config_t *config)
{
    wj_perisys_ahb_hclk_cfg_clock_sel_pll(CLKGEN_REG_BASE);
    wj_perisys_ahb_hclk_cfg_div_num(CLKGEN_REG_BASE, config->perisys_ahb_hclk_radio);
    wj_perisys_ahb_hclk_cfg_div_en(CLKGEN_REG_BASE);

    wj_perisys_apb_pclk_cde_radio(CLKGEN_REG_BASE, config->perisys_apb_pclk_radio);
    wj_perisys_apb_pclk_cde_sync(CLKGEN_REG_BASE);

    return CSI_OK;
}

void soc_set_sys_freq(uint32_t val)
{
#ifdef BOOTROM_TEST_NOT
    uint16_t i;
    perisys_clk_config_t *config = NULL;

    for (i = 0U; i < (sizeof(perisys_config) / sizeof(perisys_clk_config_t)); i++) {
        if (perisys_config[i].apb_clk == val) {
            config = (perisys_clk_config_t *)&perisys_config[i];
            break;
        }
    }

    if (i >= (sizeof(perisys_config) / sizeof(perisys_clk_config_t))) {
        config = (perisys_clk_config_t *)&perisys_config[0];
    }

    soc_perisys_clk_config(config);
    g_apb_clock = config->apb_clk;
    g_ahb_clock = config->ahb_clk;
    g_spi_clock = 400000000;
#endif
    g_apb_clock = IHS_VALUE;
    g_ahb_clock = IHS_VALUE;
    g_spi_clock = IHS_VALUE;
}

void soc_clk_enable(int32_t module)
{
    if ((uint32_t)module < 32U) {
        wj_sw_clk_gate_en_0_on(CLKGEN_REG_BASE, (uint32_t)module);
    } else if ((uint32_t)module < 62U) {
        wj_sw_clk_gate_en_1_on(CLKGEN_REG_BASE, (uint32_t)module - 32U);
    } else {
        wj_sw_clk_gate_en_2_on(CLKGEN_REG_BASE, (uint32_t)module - 62U);
    }
}

void soc_clk_disable(int32_t module)
{
    if ((uint32_t)module < 32U) {
        wj_sw_clk_gate_en_0_off(CLKGEN_REG_BASE, (uint32_t)module);
    } else if ((uint32_t)module < 62U) {
        wj_sw_clk_gate_en_1_off(CLKGEN_REG_BASE, (uint32_t)module - 32U);
    } else {
        wj_sw_clk_gate_en_2_off(CLKGEN_REG_BASE, (uint32_t)module - 62U);
    }
}

