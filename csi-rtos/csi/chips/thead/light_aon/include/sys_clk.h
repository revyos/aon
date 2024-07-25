/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 */

/******************************************************************************
 * @file     sys_clk.h
 * @brief    header file for setting system frequency.
 * @version  V1.0
 * @date     9. April 2020
 ******************************************************************************/
#ifndef _SYS_CLK_H_
#define _SYS_CLK_H_

#include <stdint.h>
#include <soc.h>
#include <drv/clk.h>
#include <drv/porting.h>
#include <drv/helper.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RTC_PCLK = 0U,
    AOGPIO_PCLK,
    AOI2C_PCLK,
    PVTC_PCLK,
    SHARE_SRAM_CLK,
    AOPAD_PCLK,
    AOAPB_HCLK,
    AOSRAM_HCLK,
    AOAHB_HCLK,
    AOGPIO_DBCLK,
    AOTIMER_PCLK,
    AOTIMER_CCLK,
} clk_module_t;

#define	REG_AONSYS_CLK_BASE		(0xfff46000)
#define REG_AUDIO_PLL_CFG0              (REG_AONSYS_CLK_BASE + 0x00)
#define REG_AUDIO_PLL_CFG1		(REG_AONSYS_CLK_BASE + 0x04)
#define REG_SYS_PLL_CFG0                (REG_AONSYS_CLK_BASE + 0x10)
#define REG_SYS_PLL_CFG1                (REG_AONSYS_CLK_BASE + 0x14)
#define REG_AONSYS_PLL_STS		(REG_AONSYS_CLK_BASE + 0x90)
#define REG_AONSYS_CLK_CFG		(REG_AONSYS_CLK_BASE + 0x100)
#define REG_SHARE_SRAM_CLK_CFG		(REG_AONSYS_CLK_BASE + 0x104)
#define REG_AOI2C_CLK_CFG		(REG_AONSYS_CLK_BASE + 0x11C)
#define REG_AONSYS_CLK_GATE		(REG_AONSYS_CLK_BASE + 0x120)
#define REG_APSYS_CLK_GATE		(REG_AONSYS_CLK_BASE + 0x130)

#define AON_REFDIV_SHIFT		0U
#define AON_FBDIV_SHIFT			8U
#define AON_POSTDIV1_SHIFT		20U
#define AON_POSTDIV2_SHIFT		24U

#define AON_PLL_BYPASS_MASK		BIT(30)
#define AON_DACPD_MASK			BIT(25)
#define AON_DSMPD_MASK			BIT(24)
#define AON_FRAC_MASK			GENMASK(23, 0)
#define AON_FRAC_SHIFT			0U
#define APSYS_CLK_EN			BIT(0)

#define AON_PLL_RST_MASK		BIT(29)

#define AONSYS_CLK_DIV_EN_MASK		BIT(11)
#define AONSYS_CLK_SWITCH_TO_RC_MASK	BIT(5)
#define AONSYS_CLK_SWITCH_TO_PLL_MASK	BIT(4)
#define AONSYS_CLK_CDE_SYNC		BIT(3)
#define AONSYS_CLK_CDE_RATIO_SHIFT	0
#define AONSYS_CLK_CDE_RATIO_MASK	0x7
#define AONSYS_CLK_DIV_NUM_SHIFT	8
#define AONSYS_CLK_DIV_NUM_MASK		0x7

#define SHARE_SRAM_CLK_SWITCH_SEL	BIT(13)
#define SHARE_SRAM_CLK_DIV1_DIV_EN	BIT(12)
#define SHARE_SRAM_CLK_DIV0_DIV_EN      BIT(4)
#define SHARE_SRAM_CLK_DIV1_DIV_NUM_SHIFT	8
#define SHARE_SRAM_CLK_DIV0_DIV_NUM_SHIFT       0
#define SHARE_SRAM_CLK_DIV_NUM_MASK	0xf

/* REG_AOI2C_CLK_CFG*/
#define AOI2C_IC_CLK_MUX_SEL		BIT(0)

/* AOSYS_CLK_GATE */
#define AON_RTC_PCLK_EN			BIT(0)
#define AON_SRAM_AXI_CORE_CLK_EN	BIT(1)
#define AON_SRAM_AXI_ACLK_4_EN		BIT(2)
#define AON_PVTC_PCLK_EN		BIT(3)
#define AON_AOPAD_PCLK_EN		BIT(4)
#define AON_AOSRAM_HCLK_EN		BIT(6)
#define AON_AOGPIO_PCLK_EN		BIT(8)
#define AON_AOGPIO_DBCLK_EN		BIT(9)
#define AON_AOTIMER_PCLK_EN		BIT(10)
#define AON_AOTIMER_CCLK_EN		BIT(11)
#define AON_AOWDT_PCLK_EN		BIT(13)
#define AON_DSPSYS_ACLK_S_EN		BIT(14)
#define AON_ADC_PCLK_EN			BIT(15)
#define AON_AUDGPIO_PCLK_EN		BIT(16)
#define AON_AUDGPIO_DBCLK_EN		BIT(17)
#define AON_AOI2C_IC_CLK_EN		BIT(18)
#define AON_AOI2C_PCLK_EN		BIT(19)
#define AON_AOUART_SCLK_EN		BIT(20)
#define AON_AOUART_PCLK_EN		BIT(21)


#define SYS_PLL_LOCKED		BIT(1)
#define AUDIO_PLL_LOCKED	BIT(0)
#define AON_SYS_RC_CLK_FREQ	(26000000U)
#define LOCK_TIMEOUT_US         30
#define AONSYS_CLK_PLL_SRC_DIV_NUM	4

#define AUDIOPLL_FOUTVCO_VALUE		2064384000U

#define AON_PLL_RATE(_vco, _rate, _fout3, _r, _b, _f, _p, _k, _bp, _m)      \
{                                               \
        .vco_rate       =       (_vco),         \
        .rate           =       (_rate),        \
        .fout3          =       (_fout3),       \
        .refdiv         =       (_r),           \
        .fbdiv          =       (_b),           \
        .frac           =       (_f),           \
        .postdiv1       =       (_p),           \
        .postdiv2       =       (_k),           \
        .is_bypass      =       (_bp),          \
	.mode		=	(_m),		\
}

typedef enum {
    CPU_24000KHZ = 24000000U,
    CPU_24576KHZ = 24576000U,
    CPU_49152KHZ = 49152000U,
    CPU_50000KHZ = 50000000U,
    CPU_73728KHZ = 73728000U,
} sys_freq_t;

enum aon_clktype {
    AON_AUDIO_PLL,
    AON_SYS_PLL,
    AON_CPU_PLL,
    AON_CPU_PLL1,
    AON_GMAC_PLL,
    AON_VIDEO_PLL,
    AON_DDR_PLL,
	AON_DPU_PLL0,
	AON_DPU_PLL1,
	AON_TEE_PLL,
	AON_OSC,
	AON_RTC
};

enum light_pll_mode {
	PLL_MODE_FRAC = 1,
	PLL_MODE_INT,
};

struct aon_pll_rate_table {
	uint32_t vco_rate;
	uint32_t rate;
	uint32_t fout3;
	uint32_t refdiv;
	uint32_t fbdiv;
	uint32_t frac;
	uint32_t postdiv1;
	uint32_t postdiv2;
	uint8_t is_bypass;
	enum light_pll_mode mode;
};

struct aon_pll_clk {
         enum aon_clktype clk_type;
         const struct aon_pll_rate_table *rate_table;
         uint8_t rate_count;
         uint32_t lock_bit;
};

#ifdef __cplusplus
}
#endif

#endif /* _SYS_CLK_H_ */

