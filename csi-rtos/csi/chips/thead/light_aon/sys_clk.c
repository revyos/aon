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
#include "syslog.h"

//#define CONFIG_AON_EMU
#define USE_AOUART	1

static const struct aon_pll_rate_table fm_aon_audiopll_tbl[] = {
	AON_PLL_RATE(24000000U, 24000000U, 24000000U, 1U, 1U, 1U,  1U, 1U, 1U, 0U),
	AON_PLL_RATE(864000000U, 288000000U, 48000000U, 1U, 36U, 0U, 3U, 1U, 0U, PLL_MODE_INT),	/* HW default */
	AON_PLL_RATE(884736000U, 294912000U, 49152000U, 1U, 36U, 14495600U, 3U, 1U, 0U, PLL_MODE_FRAC),
	AON_PLL_RATE(2064384000U, 294912000U, 49152000U, 1U, 86U, 268500U, 7U, 1U, 0U, PLL_MODE_FRAC),	/* uboot increse freq to 49.152MHz */
};

static const struct aon_pll_rate_table fm_aon_syspll_tbl[] = {
	AON_PLL_RATE(2438553600U, 135475200U, 22579200U, 1U, 101U, 10173704U, 6U, 3U, 0U, PLL_MODE_FRAC),
	AON_PLL_RATE(2424000000U, 134667000U, 22444500U, 1U, 101U, 0U, 6U, 3U, 0U, PLL_MODE_INT),
	AON_PLL_RATE(884736000U, 294912000U, 49152000U, 1U, 36U, 14495600U, 3U, 1U, 0U, PLL_MODE_FRAC),
};

struct aon_pll_clk fm_aon_audiopll_div = {
	.clk_type = AON_AUDIO_PLL,
	.rate_table = fm_aon_audiopll_tbl,
	.rate_count = ARRAY_SIZE(fm_aon_audiopll_tbl),
	.lock_bit = AUDIO_PLL_LOCKED,
};

struct aon_pll_clk fm_aon_syspll_div = {
	.clk_type = AON_SYS_PLL,
	.rate_table = fm_aon_syspll_tbl,
	.rate_count = ARRAY_SIZE(fm_aon_syspll_tbl),
	.lock_bit = SYS_PLL_LOCKED,
};

uint32_t g_system_clock;
uint32_t g_share_srame_clock;
uint32_t g_sram_clk_div0_num = 3;
uint32_t g_sram_clk_div1_num = 3;
uint32_t g_aoi2c_ic_clk = 24000000U;
uint32_t g_aonsys_clk_src_pll_div = 4; /* default */

static void aon_iowrite32(uint32_t val, uint32_t addr)
{
	(*(volatile uint32_t*) addr) = val;
}

static uint32_t aon_ioread32(uint32_t addr)
{
	return (uint32_t)(* ((volatile uint32_t*)addr));
}

static void aonreg_update_bits(uint32_t reg, uint32_t mask, uint32_t val)
{
	uint32_t data = aon_ioread32(reg);

	data &= ~mask;
	data |= val & mask;

	aon_iowrite32(data, reg);
}

/* aon clock get */
uint32_t soc_get_rtc_pclk()
{
	return g_system_clock;
}

uint32_t soc_get_aogpio_pclk()
{
	return g_system_clock;
}

uint32_t soc_get_gpio4_pclk()
{
	return g_system_clock;
}

uint32_t soc_get_aoi2c_pclk()
{
	return g_system_clock;
}

uint32_t soc_get_pvtc_pclk(uint32_t idx)
{
	return g_system_clock;
}

uint32_t soc_get_e902_cclk()
{
	return g_system_clock;
}

uint32_t soc_get_aoahb_clk()
{
	return g_system_clock;
}

uint32_t soc_get_aosram_hclk()
{
	return g_system_clock;
}

uint32_t soc_get_aoapb_hclk()
{
	return g_system_clock;
}

uint32_t soc_get_aopad_pclk()
{
	return g_system_clock;
}

uint32_t soc_get_aotimer_pclk()
{
	return g_system_clock;
}

static uint32_t soc_get_aotimer_cclk()
{
	return 24000000U;
}

uint32_t soc_get_wdt_freq(uint32_t idx)
{
	return g_system_clock;
}

uint32_t soc_get_adc_pclk()
{
	return g_system_clock;
}

uint32_t soc_get_aouart_pclk()
{
	return g_system_clock;
}

static uint32_t soc_get_aouart_sclk(uint32_t idx)
{
#ifndef USE_AOUART /* AP UART */
	return 50000000U;
#else
	return 24000000U;
#endif
}

uint32_t soc_get_sram_axi_core_clk()
{
	return g_share_srame_clock;
}

uint32_t soc_get_dspsys_aclk_s_clk()
{
	return g_share_srame_clock;
}

/* the clock shall be configured by audio subsys, select div0/1 */
uint32_t soc_get_sram_axi_aclk_4_clk()
{
	/*
	 * sys_pll_foutvco/default =>	audio_subsys_aclk_div_0 => 812.8512M
	 *									audio_subsys_aclk => sram_axi_aclk_4
	 * audio_pll_foutvco	   =>	audio_subsys_aclk_div_1 => 688.128M
	 *
	 */

	return 0;

}

uint32_t soc_get_aogpio_dbclk()
{
	/* pad_rtc_clk */
	return 32768U;
}

static uint32_t soc_get_audgpio_dbclk()
{
	/* pad_rtc_clk */
	return 32768U;
}

uint32_t soc_get_gpio4_dbclk()
{
	return soc_get_audgpio_dbclk();
}

static uint32_t soc_get_aoi2c_ic_clk()
{
	return g_aoi2c_ic_clk;
}

/* aon clock gate */
void soc_rtc_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_RTC_PCLK_EN, AON_RTC_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_RTC_PCLK_EN, 0);
}

void soc_aogpio_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOGPIO_PCLK_EN, AON_AOGPIO_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOGPIO_PCLK_EN, 0);
}

/* AUDGPIO */
void soc_gpio4_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AUDGPIO_PCLK_EN, AON_AUDGPIO_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AUDGPIO_PCLK_EN, 0);
}

void soc_aoi2c_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOI2C_PCLK_EN, AON_AOI2C_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOI2C_PCLK_EN, 0);
}

void soc_pvtc_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_PVTC_PCLK_EN, AON_PVTC_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_PVTC_PCLK_EN, 0);

}

void soc_e902_cclk_enable(bool en)
{

}

void soc_get_aoahb_clk_enable(bool en)
{

}

void soc_get_aosram_hclk_enable(bool en)
{

}

void soc_get_aoapb_hclk_enable(bool en)
{

}

void soc_aopad_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOPAD_PCLK_EN, AON_AOPAD_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOPAD_PCLK_EN, 0);
}

void soc_aotimer_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOTIMER_PCLK_EN, AON_AOTIMER_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOTIMER_PCLK_EN, 0);
}

void soc_aowdt_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOWDT_PCLK_EN, AON_AOWDT_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOWDT_PCLK_EN, 0);
}

void soc_adc_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_ADC_PCLK_EN, AON_ADC_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_ADC_PCLK_EN, 0);
}

void soc_aouart_pclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOUART_PCLK_EN, AON_AOUART_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOUART_PCLK_EN, 0);
}

void soc_sram_axi_core_clk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_SRAM_AXI_CORE_CLK_EN, AON_SRAM_AXI_CORE_CLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_SRAM_AXI_CORE_CLK_EN, 0);
}

void soc_dspsys_aclk_s_clk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_DSPSYS_ACLK_S_EN, AON_DSPSYS_ACLK_S_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_DSPSYS_ACLK_S_EN, 0);
}

void soc_sram_axi_aclk_4_clk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_SRAM_AXI_ACLK_4_EN, AON_SRAM_AXI_ACLK_4_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_SRAM_AXI_ACLK_4_EN, 0);
}

void soc_aogpio_dbclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOGPIO_DBCLK_EN, AON_AOGPIO_DBCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOGPIO_DBCLK_EN, 0);
}

void soc_gpio4_dbclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AUDGPIO_DBCLK_EN, AON_AUDGPIO_DBCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AUDGPIO_DBCLK_EN, 0);
}

void soc_aotimer_cclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOTIMER_CCLK_EN, AON_AOTIMER_CCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOTIMER_CCLK_EN, 0);
}

void soc_rtc_clk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_RTC_PCLK_EN, AON_RTC_PCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_RTC_PCLK_EN, 0);
}

void soc_aoi2c_ic_clk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOI2C_IC_CLK_EN, AON_AOI2C_IC_CLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOI2C_IC_CLK_EN, 0);
}

void soc_aouart_sclk_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOUART_SCLK_EN, AON_AOUART_SCLK_EN);
	else
		aonreg_update_bits(REG_AONSYS_CLK_GATE, AON_AOUART_SCLK_EN, 0);
}

/* Becarefull shared clock */
void soc_peri_i2s_src_clk_0_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_APSYS_CLK_GATE, APSYS_CLK_EN, APSYS_CLK_EN);
	else
		aonreg_update_bits(REG_APSYS_CLK_GATE, APSYS_CLK_EN, 0);
}

void soc_peri_i2s_src_clk_1_enable(bool en)
{
	if (en)
		aonreg_update_bits(REG_APSYS_CLK_GATE, APSYS_CLK_EN, APSYS_CLK_EN);
	else
		aonreg_update_bits(REG_APSYS_CLK_GATE, APSYS_CLK_EN, 0);
}

/* glitch free mux */
static csi_error_t share_sram_clk_switch(enum aon_clktype pll)
{
	uint32_t data = aon_ioread32(REG_SHARE_SRAM_CLK_CFG);

	switch(pll) {
	case AON_AUDIO_PLL:
		data |= SHARE_SRAM_CLK_SWITCH_SEL;
		break;
	case AON_SYS_PLL: /* default */
		data &= ~SHARE_SRAM_CLK_SWITCH_SEL;
		break;
	default:
		LOG_D("invalid share sram clock source\n");
		return CSI_ERROR;
	}

	aon_iowrite32(data, REG_SHARE_SRAM_CLK_CFG);

	return CSI_OK;
}

static csi_error_t aoi2c_ic_clk_switch(enum aon_clktype clk)
{
	uint32_t data = aon_ioread32(REG_AOI2C_CLK_CFG);

	switch(clk) {
	case AON_AUDIO_PLL:
		data &= ~AOI2C_IC_CLK_MUX_SEL;
		break;
	case AON_OSC:
		data |= AOI2C_IC_CLK_MUX_SEL;
		break;
	default:
		LOG_D("invalid aoi2c ic clock source\n");
		return CSI_ERROR;
	}

	aon_iowrite32(data, REG_AOI2C_CLK_CFG);

	return CSI_OK;
}

/* counter divider */
csi_error_t share_sram_clk_div_set(uint32_t div_idx, uint32_t div_num)
{
	uint32_t data = aon_ioread32(REG_SHARE_SRAM_CLK_CFG);

	if (div_idx == 0) {/* parent clock: sys_pll_foutvco */
		data &= ~(SHARE_SRAM_CLK_DIV_NUM_MASK << SHARE_SRAM_CLK_DIV0_DIV_NUM_SHIFT);
		data |= (div_num & SHARE_SRAM_CLK_DIV_NUM_MASK) << SHARE_SRAM_CLK_DIV0_DIV_NUM_SHIFT;
		g_sram_clk_div0_num = div_num & SHARE_SRAM_CLK_DIV_NUM_MASK;
	} else if(div_idx == 1) {/* parent clock: audio_pll_foutvco */
		data &= ~(SHARE_SRAM_CLK_DIV_NUM_MASK << SHARE_SRAM_CLK_DIV1_DIV_NUM_SHIFT);
		data |= (div_num & SHARE_SRAM_CLK_DIV_NUM_MASK) << SHARE_SRAM_CLK_DIV1_DIV_NUM_SHIFT;
		g_sram_clk_div1_num = div_num & SHARE_SRAM_CLK_DIV_NUM_MASK;
	} else {
		LOG_D("invalid share sram clock divider index\n");
		return CSI_ERROR;
	}

	aon_iowrite32(data, REG_SHARE_SRAM_CLK_CFG);

	data &= ~SHARE_SRAM_CLK_DIV0_DIV_EN;
	aon_iowrite32(data, REG_SHARE_SRAM_CLK_CFG);

	udelay(1);

	data |= SHARE_SRAM_CLK_DIV0_DIV_EN;
	aon_iowrite32(data, REG_SHARE_SRAM_CLK_CFG);

	return CSI_OK;
}

static void sys_clk_switch_to_rc(bool en)
{
	uint32_t data = aon_ioread32(REG_AONSYS_CLK_CFG);

	if (en)
		data |= AONSYS_CLK_SWITCH_TO_RC_MASK;
	else
		data &= ~AONSYS_CLK_SWITCH_TO_RC_MASK;

	aon_iowrite32(data, REG_AONSYS_CLK_CFG);
}

static void sys_clk_switch_to_pll(bool en)
{
	uint32_t data = aon_ioread32(REG_AONSYS_CLK_CFG);

	if (en)
		data &= ~AONSYS_CLK_SWITCH_TO_PLL_MASK;
	else
		data |= AONSYS_CLK_SWITCH_TO_PLL_MASK;

	 aon_iowrite32(data, REG_AONSYS_CLK_CFG);
}

static csi_error_t aon_pll_wait_lock(struct aon_pll_clk *div)
{
	uint32_t data, delay_cnt = 0;
	csi_error_t ret = CSI_OK;

	do {
		data = aon_ioread32(REG_AONSYS_PLL_STS);
		udelay(1);
		delay_cnt++;
		if (delay_cnt >= LOCK_TIMEOUT_US) {
			ret = CSI_TIMEOUT;
			break;
		}
	} while (!(data & div->lock_bit));

	return ret;
}

int clk_aon_pll_set_rate(struct aon_pll_clk *div, int idx)
{
	const struct aon_pll_rate_table *rate;
	uint32_t data, div_val;
	int32_t ret;
	uint32_t pll_cfg0, pll_cfg1;

	rate = &div->rate_table[idx];

	switch(div->clk_type) {
	case AON_AUDIO_PLL:
		pll_cfg0 = REG_AUDIO_PLL_CFG0;
		pll_cfg1 = REG_AUDIO_PLL_CFG1;
		break;
	case AON_SYS_PLL:
		pll_cfg0 = REG_SYS_PLL_CFG0;
		pll_cfg1 = REG_SYS_PLL_CFG1;
		break;
	default:
		LOG_D("invalid aon subsys clock source\n");
		return CSI_ERROR;
	}

	/* Enable RST */
	data = aon_ioread32(pll_cfg1);
	data |= AON_PLL_RST_MASK;
	aon_iowrite32(data, pll_cfg1);

	div_val = (rate->refdiv << AON_REFDIV_SHIFT) |
			(rate->fbdiv << AON_FBDIV_SHIFT) |
			(rate->postdiv1 << AON_POSTDIV1_SHIFT) |
			(rate->postdiv2 << AON_POSTDIV2_SHIFT);

	aon_iowrite32(div_val, pll_cfg0);

	data = aon_ioread32(pll_cfg1);
	data &= ~AON_PLL_BYPASS_MASK;
	data &= ~AON_DACPD_MASK;
	data |= AON_DSMPD_MASK;
	if (rate->mode == PLL_MODE_FRAC) {
		data &= ~(AON_FRAC_MASK << AON_FRAC_SHIFT);
		data |= rate->frac;
		data &= ~AON_DSMPD_MASK;
		data |= AON_DACPD_MASK;
	}

	aon_iowrite32(data, pll_cfg1);

	udelay(3);

	/* Disable RST */
	data = aon_ioread32(pll_cfg1);
	data &= ~AON_PLL_RST_MASK;
	aon_iowrite32(data, pll_cfg1);

	/* Wait Lock, ~20us cost */
	ret = aon_pll_wait_lock(div);
	if (ret)
		return ret;

	/* HW requires 30us for pll stable */
	udelay(30);

	return ret;
}

/* counter divider */
void aonsys_clk_pll_src_div_set(uint32_t div_num)
{
	uint32_t data = aon_ioread32(REG_AONSYS_CLK_CFG);

	data &= ~(AONSYS_CLK_DIV_NUM_MASK << AONSYS_CLK_DIV_NUM_SHIFT);
	data |= (div_num & AONSYS_CLK_DIV_NUM_MASK) << AONSYS_CLK_DIV_NUM_SHIFT;
	aon_iowrite32(data, REG_AONSYS_CLK_CFG);

	data &= ~AONSYS_CLK_DIV_EN_MASK;
	aon_iowrite32(data, REG_AONSYS_CLK_CFG);

	udelay(1);

	data |= AONSYS_CLK_DIV_EN_MASK;
	aon_iowrite32(data, REG_AONSYS_CLK_CFG);

	g_aonsys_clk_src_pll_div = div_num & AONSYS_CLK_DIV_NUM_MASK;
}

/* CDE divider */
void aonsys_clk_ratio_set(uint32_t ratio)
{
	uint32_t data = aon_ioread32(REG_AONSYS_CLK_CFG);

	data &= ~(AONSYS_CLK_CDE_RATIO_MASK << AONSYS_CLK_DIV_NUM_SHIFT);
	data |= (ratio & AONSYS_CLK_CDE_RATIO_MASK) << AONSYS_CLK_DIV_NUM_SHIFT;
	aon_iowrite32(REG_AONSYS_CLK_CFG, data);

	data &= ~AONSYS_CLK_CDE_SYNC;
	aon_iowrite32(data, REG_AONSYS_CLK_CFG);

	udelay(1);

	data |= AONSYS_CLK_CDE_SYNC;
	aon_iowrite32(data, REG_AONSYS_CLK_CFG);
}

/* aoi2c_ic_freq: 49152000HZ/24000000HZ */
int soc_set_aoi2c_ic_freq(uint32_t freq)
{
	if (freq != 49152000U && freq != 24000000U) {
		LOG_D("invald aoi2c ic freq to set\n");
		return CSI_ERROR;
	}

	if (freq == 49152000U)
		aoi2c_ic_clk_switch(AON_AUDIO_PLL);
	else
		aoi2c_ic_clk_switch(AON_OSC);

	g_aoi2c_ic_clk = freq;

	return CSI_OK;
}

uint32_t soc_get_rtc_freq(uint32_t idx)
{
	return 32768U;
}

uint32_t soc_get_timer_freq(uint32_t idx)
{
	return soc_get_aotimer_cclk();
}

uint32_t soc_get_iic_freq(uint32_t idx)
{
	return soc_get_aoi2c_ic_clk();
}

uint32_t soc_get_uart_freq(uint32_t idx)
{
	if (idx == 1 )
		return 100000000;
	else
		return soc_get_aouart_sclk(idx);
}

uint32_t soc_get_cur_cpu_freq(void)
{
    return g_system_clock;
}

/*
 * share_sram_freq:812851200HZ/688128000HZ
 */
int soc_set_share_sram_freq(uint32_t freq)
{
	if (g_share_srame_clock == freq)
		return CSI_OK;

	if (freq == 812851200U)
		share_sram_clk_switch(AON_SYS_PLL);
	else if (freq == 688128000U)
		share_sram_clk_switch(AON_AUDIO_PLL);
	else {
		LOG_D("invalid share sram frequency\n");
		return CSI_ERROR;
	}

	g_share_srame_clock = freq;

	return CSI_OK;
}

/*
 * aon_sys_freq: 73728000HZ/24000000HZ/26000000HZ
 *
 */
void soc_set_sys_freq(uint32_t freq)
{

#ifdef CONFIG_AON_EMU
	return 24000000U;
#endif

	if (freq != 73728000U && freq != 24000000U && freq != 26000000U) {
		LOG_D("invalid aon sys freq\n");
		return;
	}

	if (g_system_clock == freq)
		return;

	switch(freq) {
	case 73728000U:
		if (g_system_clock == 24000000U)
			sys_clk_switch_to_pll(true);
		else if (g_system_clock == 26000000U) {
			sys_clk_switch_to_pll(true);
			sys_clk_switch_to_rc(false);
		}
		break;
	case 24000000U:
		if (g_system_clock == 73728000U)
			sys_clk_switch_to_pll(false);
		else if (g_system_clock == 26000000U) {
			sys_clk_switch_to_pll(false); /* switch0 */
			sys_clk_switch_to_rc(false);  /* switch1 */
		}
		break;
	case 26000000U:
		sys_clk_switch_to_rc(true);
		break;
	}

	g_system_clock = freq;
}

uint32_t soc_get_coretim_freq(void)
{
	return 3000000;
}

void clk_aon_clk_init(void)
{
	/*
	 * aonsys_clk is set to audio pll in u-boot stage, the frequency value is 73.728MHZ
	 * share_sram_clk is set to sys pll in u-boot stage. the frequency value is 812.8512MHz
	 */

#ifdef CONFIG_AON_EMU
	g_system_clock = CPU_24000KHZ;
#else

 #ifndef CONFIG_BOOT_FR_UBOOT
	g_system_clock = CPU_24000KHZ;
	g_share_srame_clock = 808000000U;
	g_aoi2c_ic_clk = 24000000U;
 #else

	g_system_clock = CPU_73728KHZ;
	g_share_srame_clock = 812851200U;
	g_aoi2c_ic_clk = 49152000U;
 #endif
#endif

	/* Keep AP clock enable */
	aon_iowrite32(APSYS_CLK_EN, REG_APSYS_CLK_GATE);
}
